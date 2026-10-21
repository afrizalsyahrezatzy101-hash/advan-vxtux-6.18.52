#include <linux/of.h>
#include <linux/platform_device.h>
#define IP_DRIVER(ip, compatible)					\
static const struct of_device_id ip##_dvfs_of_match[] = {		\
	{ .compatible = { compatible } },				\
	{ }								\
};									\
									\
static struct platform_driver ip##_dvfs_driver = {		\
	.driver		= {					\
		.name	= #ip "-dvfs",				\
		.of_match_table = ip##_dvfs_of_match,		\
	},							\
};

IP_DRIVER(fd, "sprd,hwdvfs-mtx");
