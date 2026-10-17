/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#include <linux/types.h>
/*
 * Recovered from the iscle_ums512 5.4 tree (work/donors_20260930/iscle_ums512),
 * the same UMS512 SoC. The port tree and the other two donors did not carry this
 * file, which is what blocked the sprd-vsp-pw-domain and jpg ports.
 */
/*
 * Copyright (C) 2012--2015 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _VSP_PW_DOMAIN_H_
#define _VSP_PW_DOMAIN_H_

#define BIT_PMU_APB_PD_MM_VSP_AUTO_SHUTDOWN_EN                  BIT(24)
#define BIT_PMU_APB_PD_MM_VSP_FORCE_SHUTDOWN                    BIT(25)
#define BIT_PMU_APB_PD_MM_VSP_STATE(x)                          (((x) & 0x1F))

enum {
	VSP_PW_DOMAIN_VSP = 0,
	VSP_PW_DOMAIN_VPP,
	VSP_PW_DOMAIN_VSP_ENC,
	VSP_PW_DOMAIN_VSP_GSP,
	VSP_PW_DOMAIN_VSP_JPG,
	VSP_PW_DOMAIN_VSP_CPP,
	VSP_PW_DOMAIN_COUNT_MAX
};
enum {
	VSP_PW_DOMAIN_OFF = 0,
	VSP_PW_DOMAIN_ON,
};
struct client_info_t {
	__u8 pw_state;
	__u8 pw_count;
};

/* V18: Guard against redefinition */
#ifndef berry_vsp_mutex_compat_defined
#define berry_vsp_mutex_compat_defined
struct berry_vsp_mutex_compat {
	unsigned long owner;
	int magic;
};
typedef struct berry_vsp_mutex_compat berry_client_lock_t;
static inline void __berry_mutex_lock(berry_client_lock_t *m) { (void)m; }
static inline void __berry_mutex_unlock(berry_client_lock_t *m) { (void)m; }
static inline void __berry_mutex_init(berry_client_lock_t *m) { (void)m; }
#endif

struct vsp_pw_domain_info_t {
	struct client_info_t pw_vsp_info[VSP_PW_DOMAIN_COUNT_MAX];
	struct berry_vsp_mutex_compat client_lock;  /* V18: restore for VSP driver */
	__u8 vsp_pw_state;
};

#endif
