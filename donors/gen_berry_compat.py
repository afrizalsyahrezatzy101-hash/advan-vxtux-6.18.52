import os

TARGET = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512/kernel/berry_compat_link.c"

SRC = r'''// SPDX-License-Identifier: GPL-2.0
/* V18: Berry compat - real (non-inline) stubs for symbols that the
 * VXTux 6.18 tree references but does not provide.
 *
 * These are link-time symbols, so they cannot live in the header-only
 * include/linux/berry_compat.h shim.
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/ip_tunnels.h>
#include <net/secure_seq.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/bluetooth/hci.h>

/* ====== ip_tunnel_init: VXTux 6.18 renamed it to __ip_tunnel_init ====== */
int ip_tunnel_init(struct net_device *dev)
{
	return __ip_tunnel_init(dev);
}
EXPORT_SYMBOL_GPL(ip_tunnel_init);

/* ====== ip_tunnel_limit_headroom: dropped upstream, still referenced ====== */
int ip_tunnel_limit_headroom(struct net_device *dev, unsigned int headroom)
{
	(void)dev;
	(void)headroom;
	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_limit_headroom);

/* ====== tcp_chrono_start / tcp_chrono_stop ======
 * The chrono tracking body was dropped from net/ipv4/tcp.c but the
 * declaration and its callers remain. No-op stubs keep the link happy.
 */
void tcp_chrono_start(struct sock *sk, const enum tcp_chrono type)
{
	(void)sk;
	(void)type;
}
EXPORT_SYMBOL_GPL(tcp_chrono_start);

void tcp_chrono_stop(struct sock *sk, const enum tcp_chrono type)
{
	(void)sk;
	(void)type;
}
EXPORT_SYMBOL_GPL(tcp_chrono_stop);

/* ====== kzalloc_flex: upstream 6.18 helper, missing here ====== */
void *kzalloc_flex(size_t size, gfp_t flags)
{
	return kzalloc(size, flags);
}
EXPORT_SYMBOL_GPL(kzalloc_flex);
'''

with open(TARGET, "w", newline="\n") as f:
    f.write(SRC)

with open(TARGET, "rb") as f:
    d = f.read()
print("bytes:", len(d))
print("first line:", d.split(b"\n")[0])
print("has ip_tunnel_init:", b"ip_tunnel_init" in d)
print("has kzalloc_flex:", b"kzalloc_flex" in d)
