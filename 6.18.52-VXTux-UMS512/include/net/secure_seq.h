/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NET_SECURE_SEQ
#define _NET_SECURE_SEQ

#include <linux/types.h>

struct net;

struct tcp_seq_and_ts_off_fields {
	u32 seq;
	u32 ts_off;
};
union tcp_seq_and_ts_off {
	struct tcp_seq_and_ts_off_fields off;
	u64 both;
	u64 hash64;
};

u64 secure_ipv4_port_ephemeral(__be32 saddr, __be32 daddr, __be16 dport);
u64 secure_ipv6_port_ephemeral(const __be32 *saddr, const __be32 *daddr,
			       __be16 dport);
u32 secure_tcp_seq(__be32 saddr, __be32 daddr,
		   __be16 sport, __be16 dport);
u32 secure_tcp_ts_off(const struct net *net, __be32 saddr, __be32 daddr);
union tcp_seq_and_ts_off secure_tcp_seq_and_ts_off(const struct net *net,
						   __be32 saddr, __be32 daddr,
						   __be16 sport, __be16 dport);
u32 secure_tcpv6_seq(const __be32 *saddr, const __be32 *daddr,
		     __be16 sport, __be16 dport);
u32 secure_tcpv6_ts_off(const struct net *net,
			const __be32 *saddr, const __be32 *daddr);
union tcp_seq_and_ts_off secure_tcpv6_seq_and_ts_off(const struct net *net,
						    const __be32 *saddr,
						    const __be32 *daddr,
						    __be16 sport,
						    __be16 dport);

#endif /* _NET_SECURE_SEQ */
