/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* ip6tables module for matching the Hop Limit value
 * Maciej Soltysiak <solt@dns.toxicfilms.tv>
 * Based on HW's ttl module */

#ifndef _IP6T_HL_H
#define _IP6T_HL_H

#include <linux/types.h>

enum {
	IP6T_HL_SET = 0,	/* set hop limit to value */
	IP6T_HL_INC,		/* increment hop limit by 1 */
	IP6T_HL_DEC,		/* decrement hop limit by 1 */
};

#define IP6T_HL_MAXMODE	IP6T_HL_DEC


struct ip6t_hl_info {
	__u8	mode;
	__u8	hop_limit;
};


#endif
