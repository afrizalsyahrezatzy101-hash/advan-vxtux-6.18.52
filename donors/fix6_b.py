import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"


def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()


def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")


# =====================================================================
# FIX 2: net/core/netdev_rx_queue.c
#   Restore __net_mp_open_rxq / __net_mp_close_rxq.
#   The upstream bodies are inside `#if 0` in this tree. The local
#   struct netdev_rx_queue has mp_params and the local struct
#   pp_memory_provider_params has mp_ops/mp_priv, so the upstream
#   logic is usable. The only unsupported field is dev->cfg
#   (hds_config / hds_thresh) - the local net_device has no ->cfg,
#   so those two checks are dropped; netdev_need_ops_lock() and
#   dev_xdp_prog_count() exist and are kept.
# =====================================================================
print("== FIX 2: netdev_rx_queue.c restore __net_mp_open/close_rxq ==")
p = "net/core/netdev_rx_queue.c"
d = rd(p)

IMPL = b'''
/* ====== VXTux 6.18: restored from upstream 6.18 =====
 *
 * The upstream bodies live in the `#if 0` block below because they
 * referenced net_device::cfg (hds_config / hds_thresh), which this
 * tree's struct net_device does not carry. Everything else the
 * upstream code needs is present:
 *   - netdev_need_ops_lock()  include/net/netdev_lock.h
 *   - dev_xdp_prog_count()    include/linux/netdevice.h
 *   - __netif_get_rx_queue()  include/net/netdev_rx_queue.h
 *   - rxq->mp_params.mp_ops   include/net/page_pool/types.h
 * so the two functions are re-enabled here with only the cfg-based
 * tcp-data-split checks removed.
 */
int __net_mp_open_rxq(struct net_device *dev, unsigned int rxq_idx,
		      const struct pp_memory_provider_params *p,
		      struct netlink_ext_ack *extack)
{
	struct netdev_rx_queue *rxq;
	int ret;

	if (!netdev_need_ops_lock(dev))
		return -EOPNOTSUPP;

	if (rxq_idx >= dev->real_num_rx_queues) {
		NL_SET_ERR_MSG(extack, "rx queue index out of range");
		return -ERANGE;
	}
	rxq_idx = array_index_nospec(rxq_idx, dev->real_num_rx_queues);

	if (dev_xdp_prog_count(dev)) {
		NL_SET_ERR_MSG(extack, "unable to custom memory provider to device with XDP program attached");
		return -EEXIST;
	}

	rxq = __netif_get_rx_queue(dev, rxq_idx);
	if (rxq->mp_params.mp_ops) {
		NL_SET_ERR_MSG(extack, "designated queue already memory provider bound");
		return -EEXIST;
	}
#ifdef CONFIG_XDP_SOCKETS
	if (rxq->pool) {
		NL_SET_ERR_MSG(extack, "designated queue already in use by AF_XDP");
		return -EBUSY;
	}
#endif

	rxq->mp_params = *p;
	ret = netdev_rx_queue_restart(dev, rxq_idx);
	if (ret) {
		rxq->mp_params.mp_ops = NULL;
		rxq->mp_params.mp_priv = NULL;
	}
	return ret;
}
EXPORT_SYMBOL_GPL(__net_mp_open_rxq);

int net_mp_open_rxq(struct net_device *dev, unsigned int rxq_idx,
		    struct pp_memory_provider_params *p)
{
	int ret;

	netdev_lock(dev);
	ret = __net_mp_open_rxq(dev, rxq_idx, p, NULL);
	netdev_unlock(dev);
	return ret;
}
EXPORT_SYMBOL_GPL(net_mp_open_rxq);

void __net_mp_close_rxq(struct net_device *dev, unsigned int ifq_idx,
			const struct pp_memory_provider_params *old_p)
{
	struct netdev_rx_queue *rxq;
	int err;

	if (WARN_ON_ONCE(ifq_idx >= dev->real_num_rx_queues))
		return;

	rxq = __netif_get_rx_queue(dev, ifq_idx);

	/* Callers holding a netdev ref may get here after we already
	 * went thru shutdown via dev_memory_provider_uninstall().
	 */
	if (dev->reg_state > NETREG_REGISTERED &&
	    !rxq->mp_params.mp_ops)
		return;

	if (WARN_ON_ONCE(rxq->mp_params.mp_ops != old_p->mp_ops ||
			 rxq->mp_params.mp_priv != old_p->mp_priv))
		return;

	rxq->mp_params.mp_ops = NULL;
	rxq->mp_params.mp_priv = NULL;
	err = netdev_rx_queue_restart(dev, ifq_idx);
	WARN_ON(err && err != -ENETDOWN);
}
EXPORT_SYMBOL_GPL(__net_mp_close_rxq);

void net_mp_close_rxq(struct net_device *dev, unsigned ifq_idx,
		      struct pp_memory_provider_params *old_p)
{
	netdev_lock(dev);
	__net_mp_close_rxq(dev, ifq_idx, old_p);
	netdev_unlock(dev);
}
EXPORT_SYMBOL_GPL(net_mp_close_rxq);
'''

if b"EXPORT_SYMBOL_GPL(__net_mp_open_rxq);" not in d:
    i = d.find(b"#if 0")
    if i < 0:
        print("   !! #if 0 marker NOT FOUND")
    else:
        d = d[:i] + IMPL + b"\n" + d[i:]
        wr(p, d)
        print("   inserted restored implementations")
else:
    print("   already present")

# =====================================================================
# FIX 3: drivers/usb/phy/Kconfig -> allow CONFIG_USB_PHY=y
#   drivers/usb/musb/musb_sprd_vxtux.c calls usb_add_phy()/
#   usb_remove_phy() which live in drivers/usb/phy/phy.o, and that
#   object is only built when CONFIG_USB_PHY is set. The Kconfig
#   entry is `def_bool n` with no user-visible prompt, so it can
#   never be enabled - that is why the symbol was missing.
# =====================================================================
print("== FIX 3: drivers/usb/phy/Kconfig enable CONFIG_USB_PHY ==")
p = "drivers/usb/phy/Kconfig"
d = rd(p)
old = b"config USB_PHY\n\tselect EXTCON\n\tdef_bool n\n"
new = (b"config USB_PHY\n"
       b"\tselect EXTCON\n"
       b"\tdefault y\n")
if old in d:
    d = d.replace(old, new)
    wr(p, d)
else:
    print("   Kconfig pattern not found; current state:")
    for i, l in enumerate(d.split(b"\n")[:20], 1):
        print(f"     {i}: {l.decode('utf-8','replace')}")
