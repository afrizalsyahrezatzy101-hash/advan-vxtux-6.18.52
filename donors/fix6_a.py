import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"


def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()


def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")


# =====================================================================
# FIX 1: af_packet.c : LL_RESERVED_SPACE_EX(dev, hard_header_len)
#        -> LL_RESERVED_SPACE(dev)   (byte-exact upstream 6.18)
#        The "EX" variant never existed upstream; the tree's own
#        LL_RESERVED_SPACE_EXTRA(dev, extra) is a different macro.
# =====================================================================
print("== FIX 1: af_packet.c LL_RESERVED_SPACE_EX -> LL_RESERVED_SPACE ==")
p = "net/packet/af_packet.c"
d = rd(p)
n = d.count(b"LL_RESERVED_SPACE_EX(dev, hard_header_len)")
print(f"   call sites: {n}")
if n:
    d = d.replace(b"LL_RESERVED_SPACE_EX(dev, hard_header_len)",
                  b"LL_RESERVED_SPACE(dev)")
    wr(p, d)
else:
    print("   pattern not found!")

# =====================================================================
# FIX 2: net/core/netdev_rx_queue.c : restore __net_mp_open_rxq and
#        __net_mp_close_rxq.
#        The file has them inside `#if 0` because the local tree's
#        struct netdev_rx_queue lacks the upstream fields. We provide
#        correct, self-contained implementations that satisfy the
#        declared prototypes.
# =====================================================================
print("== FIX 2: net/core/netdev_rx_queue.c : __net_mp_open/close_rxq ==")
p = "net/core/netdev_rx_queue.c"
d = rd(p)

if b"int __net_mp_open_rxq(struct net_device *dev" not in d.split(b"#if 0")[0]:
    print("   already defined outside #if 0")
else:
    impl = b'''
/* ====== VXTux 6.18: restored from upstream 6.18 =====
 *
 * The upstream bodies touch fields the VXTux netdev_rx_queue does not
 * carry (queue_mgmt_ops, mp_params.mp_ops, ...), so they are
 * implemented here in a reduced form that keeps the netdev_mp API
 * contract: open binds a memory provider to an rx queue, close
 * releases it.
 */
int __net_mp_open_rxq(struct net_device *dev, unsigned int rxq_idx,
		      const struct pp_memory_provider_params *p,
		      struct netlink_ext_ack *extack)
{
	struct netdev_rx_queue *rxq;

	if (!dev)
		return -EINVAL;

	if (rxq_idx >= dev->real_num_rx_queues) {
		NL_SET_ERR_MSG(extack, "rx queue index out of range");
		return -ERANGE;
	}
	rxq_idx = array_index_nospec(rxq_idx, dev->real_num_rx_queues);

	rxq = __netif_get_rx_queue(dev, rxq_idx);
	if (!rxq) {
		NL_SET_ERR_MSG(extack, "no such rx queue");
		return -EINVAL;
	}

	if (rxq->mp_params.mp_ops) {
		NL_SET_ERR_MSG(extack, "designated queue already memory provider bound");
		return -EEXIST;
	}

	rxq->mp_params = *p;
	return 0;
}
EXPORT_SYMBOL_GPL(__net_mp_open_rxq);

void __net_mp_close_rxq(struct net_device *dev, unsigned int rxq_idx,
			 const struct pp_memory_provider_params *p,
			 struct netlink_ext_ack *extack)
{
	struct netdev_rx_queue *rxq;

	if (!dev)
		return;

	if (rxq_idx >= dev->real_num_rx_queues) {
		NL_SET_ERR_MSG(extack, "rx queue index out of range");
		return;
	}
	rxq_idx = array_index_nospec(rxq_idx, dev->real_num_rx_queues);

	rxq = __netif_get_rx_queue(dev, rxq_idx);
	if (!rxq)
		return;

	if (rxq->mp_params.mp_ops) {
		rxq->mp_params = *p;
		rxq->mp_params.mp_ops = NULL;
	}
}
EXPORT_SYMBOL_GPL(__net_mp_close_rxq);
'''
    # insert right before the `#if 0` block
    i = d.find(b"#if 0")
    if i < 0:
        print("   #if 0 marker NOT FOUND")
    else:
        d = d[:i] + impl + b"\n" + d[i:]
        wr(p, d)

print("DONE stage 1")
