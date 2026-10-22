// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Spreadtrum Communications Inc.
 *
 * sprd_sipc_virt_bus - the virtual SIPC bus.
 *
 * A platform driver that owns the "unisoc,sipc-virt-bus" DT node and
 * populates the platform devices hanging off it.  On the UMS512 board
 * (arch/arm64/boot/dts/sprd/ums512-1h10.dts) the bus node carries the
 * time correction device as its only child:
 *
 *	sipc_virt: sipc-virt {
 *		compatible = "unisoc,sipc-virt-bus";
 *		#address-cells = <1>;
 *		#size-cells = <0>;
 *
 *		time_sync: time_sync {
 *			compatible = "sprd,time-sync";
 *			status = "okay";
 *		};
 *	};
 *
 * Reimplementation of the stock vendor blob sprd-sipc-virt-bus.ko
 * (stock kernel 5.4.210, aarch64).  The stock module was a pure
 * relocator: its whole probe body was
 *
 *	ret = devm_of_platform_populate(&pdev->dev);
 *	if (!ret)
 *		dev_info(&pdev->dev, "Populate sub sipc-virt-bus success\n");
 *	else
 *		dev_err(&pdev->dev, "Failed to populate sub sipc-virt-bus\n");
 *	return ret;
 *
 * which is reproduced verbatim below.
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>

#define SPRD_SIPC_VIRT_BUS_DRV_NAME	"unisoc-sipc-virt-bus"

static const struct of_device_id sprd_sipc_virt_bus_of_match[] = {
	{ .compatible = "unisoc,sipc-virt-bus" },
	{ }
};
MODULE_DEVICE_TABLE(of, sprd_sipc_virt_bus_of_match);

/*
 * The stock 5.4 blob exported this alias under the 5.4 spelling of
 * MODULE_DEVICE_TABLE().  6.18 spells the same alias differently
 * (__mod_device_table__kmod_<mod>__of__<name>), so the old name is
 * declared by hand as well to keep the module's symbol surface identical
 * to the vendor blob it replaces.
 */
extern const struct of_device_id __mod_of__sprd_sipc_virt_bus_of_match_device_table
	__attribute__((unused, alias("sprd_sipc_virt_bus_of_match")));

static int sprd_sipc_virt_bus_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	ret = devm_of_platform_populate(dev);
	if (!ret) {
		dev_info(dev, "Populate sub sipc-virt-bus success\n");
	} else {
		dev_err(dev, "Failed to populate sub sipc-virt-bus\n");
	}

	return ret;
}

static struct platform_driver sprd_sipc_virt_bus_driver = {
	.probe		= sprd_sipc_virt_bus_probe,
	.driver		= {
		.name		= SPRD_SIPC_VIRT_BUS_DRV_NAME,
		.of_match_table	= sprd_sipc_virt_bus_of_match,
	},
};

module_platform_driver(sprd_sipc_virt_bus_driver);

MODULE_AUTHOR("Haidong Yao <haidong.yao@unisoc.com>");
MODULE_DESCRIPTION("Spreadtrum sipc virt bus driver");
MODULE_LICENSE("GPL v2");
