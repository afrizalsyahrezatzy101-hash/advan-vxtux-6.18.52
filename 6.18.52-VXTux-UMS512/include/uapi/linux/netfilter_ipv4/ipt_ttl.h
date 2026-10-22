/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* IP tables module for matching the value of the TTL
 * (C) 2000 by Harald Welte <laforge@gnumonks.org> */

#ifndef _IPT_TTL_H
#define _IPT_TTL_H

#include <linux/types.h>

enum {
	IPT_TTL_SET = 0,	/* set TTL to value */
	IPT_TTL_INC,		/* increment TTL by 1 */
	IPT_TTL_DEC,		/* decrement TTL by 1 */
};

#define IPT_TTL_MAXMODE	IPT_TTL_DEC


struct ipt_ttl_info {
	__u8	mode;
	__u8	ttl;
};


#endif
