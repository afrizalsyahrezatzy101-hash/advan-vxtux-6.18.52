/* SPDX-License-Identifier: GPL-2.0 */
/* GSP uapi shared with the sprd DRM driver, from the T618 5.4 fork. */
/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
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

#ifndef _SPRD_DRM_GSP_H
#define _SPRD_DRM_GSP_H

/*
 * When CONFIG_DRM_SPRD_GSP=m the kernel's autoconf.h defines
 * CONFIG_DRM_SPRD_GSP_MODULE instead of CONFIG_DRM_SPRD_GSP (module builds
 * remap tristate symbols). Guarding only on the =y spelling left the static
 * inline stubs below active while gsp_dev.c also compiled the real bodies ->
 * "redefinition of sprd_gsp_get_capability_ioctl". Accept both spellings.
 */
#if defined(CONFIG_DRM_SPRD_GSP) || defined(CONFIG_DRM_SPRD_GSP_MODULE)
extern int sprd_gsp_get_capability_ioctl(
			struct drm_device *dev, void *data,
			struct drm_file *file_priv);
extern int sprd_gsp_trigger_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file_priv);
#else
static inline int sprd_gsp_get_capability_ioctl(
	struct drm_device *dev, void *data,
					   struct drm_file *file_priv)
{
	pr_err("gsp get cap not implement\n");
	return -ENODEV;
}

static inline int sprd_gsp_trigger_ioctl(struct drm_device *dev,
					       void *data,
					       struct drm_file *file_priv)
{
	pr_err("gsp trigger not implement\n");
	return -ENODEV;
}
#endif

#endif
