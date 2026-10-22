/* SPDX-License-Identifier: GPL-2.0 */
//
// Spreadtrum clock infrastructure
//
// Copyright (C) 2017 Spreadtrum, Inc.
// Author: Chunyan Zhang <chunyan.zhang@spreadtrum.com>

#ifndef _SPRD_CLK_COMMON_H_
#define _SPRD_CLK_COMMON_H_

#include <linux/clk-provider.h>
#include <linux/of_platform.h>
#include <linux/regmap.h>

#include "reset.h"

struct device_node;

struct sprd_clk_common {
	struct regmap	*regmap;
	u32		reg;
	struct clk_hw	hw;
};

/*
 * struct sprd_clk_desc - one clock controller instance
 *
 * @clk_clks:	the clock hws this controller owns
 * @num_clk_clks:how many entries @clk_clks has
 * @hw_clks:	the one-cell table handed to the clock provider
 * @resets:	the reset lines this controller also owns, or NULL
 * @num_resets:how many entries @resets has
 *
 * @resets / @num_resets are the stock blob's layout: the stock
 * ums512-clk.ko registers a reset controller on every clock node that
 * owns soft-reset bits, and the stock DTB points the i2c nodes'
 * "resets" property at those clock nodes (apapb-gate ids 3, 8..12 and
 * aonapb-gate ids 17, 41). The stock struct sprd_clk_desc is 40 bytes
 * - {clk_clks, num_clk_clks, hw_clks, resets, num_resets} - and the
 * stock blob's 13 descs carry 0..67 reset entries each.
 */
struct sprd_clk_desc {
	struct sprd_clk_common		**clk_clks;
	unsigned long			num_clk_clks;
	struct clk_hw_onecell_data      *hw_clks;
	const struct sprd_reset_map	*resets;
	unsigned long			num_resets;
};

static inline struct sprd_clk_common *
	hw_to_sprd_clk_common(const struct clk_hw *hw)
{
	return container_of(hw, struct sprd_clk_common, hw);
}
int sprd_clk_regmap_init(struct platform_device *pdev,
			 const struct sprd_clk_desc *desc);
int sprd_clk_reset_register(struct device *dev, struct regmap *regmap,
			     const struct sprd_clk_desc *desc);
int sprd_clk_probe(struct device *dev, struct clk_hw_onecell_data *clkhw);

#endif /* _SPRD_CLK_COMMON_H_ */
