// SPDX-License-Identifier: GPL-2.0
/* V18: Berry compat - nf_conntrack_count stub.
 *
 * The upstream nf_conntrack_count.c is not present in the VXTux tree but
 * CONFIG_NF_CONNTRACK_COUNT is enabled, so nf_conncount_count() is
 * referenced without a definition. Provide a minimal implementation.
 */
#include <linux/types.h>
#include <linux/slab.h>
#include <net/netfilter/nf_conntrack_count.h>

unsigned int nf_conncount_count(struct net *net,
				struct nf_conncount_data *data,
				const u32 *key,
				const struct nf_conntrack_tuple *tuple,
				const struct nf_conntrack_zone *zone)
{
	(void)net;
	(void)data;
	(void)key;
	(void)tuple;
	(void)zone;
	return 0;
}
EXPORT_SYMBOL_GPL(nf_conncount_count);
