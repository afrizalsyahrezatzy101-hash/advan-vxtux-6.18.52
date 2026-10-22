/*
 * The per-IP platform drivers and governors.
 *
 * Each IP registers a devfreq governor and a platform driver whose
 * of_match_table carries the IP's device-tree compatible string.
 */

/* the devfreq profile shared by every IP */
static struct devfreq_profile dvfs_profile = {
	.get_target_freq	= dvfs_target,
	.get_dev_status		= dvfs_get_dev_status,
	.get_cur_freq		= dvfs_get_cur_freq,
};

static int dvfs_probe(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev;
	struct device *dev = &pdev->dev;
	int ret;

	ip_dev = devm_kmalloc(dev, sizeof(*ip_dev), GFP_KERNEL);
	if (!ip_dev)
		return -ENOMEM;

	mutex_init(&ip_dev->lock);
	ip_dev->ops = get_ip_dvfs_ops(dev_name(dev));
	if (!ip_dev->ops)
		return -EINVAL;

	platform_set_drvdata(pdev, ip_dev);
	dev_set_drvdata(dev, ip_dev);

	ret = dev_pm_opp_of_add_table(dev);
	if (ret) {
		dev_err(dev, "dvfs: Invalid operating-points in device tree.\n");
		return -EINVAL;
	}

	ip_dev->devfreq = devm_devfreq_add_device(dev, &dvfs_profile,
						  dev_name(dev));
	if (IS_ERR(ip_dev->devfreq)) {
		dev_err(dev, "failed to add devfreq dev with module-dvfs governor\n");
		dev_pm_opp_of_remove_table(dev);
		return -EINVAL;
	}

	return 0;
}

static int dvfs_remove(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev = platform_get_drvdata(pdev);

	if (ip_dev) {
		if (ip_dev->devfreq)
			devfreq_remove_device(ip_dev->devfreq);
		dev_pm_opp_of_remove_table(&pdev->dev);
	}

	return 0;
}

#define IP_DRIVER(ip, compatible)					\
static const struct of_device_id ip##_dvfs_of_match[] = {		\
	{ .compatible = compatible },				\
	{ }								\
};								\
									\
static struct platform_driver ip##_dvfs_driver = {		\
	.probe		= dvfs_probe,				\
	.remove		= dvfs_remove,				\
	.driver		= {					\
		.name	= #ip "-dvfs",				\
		.of_match_table = ip##_dvfs_of_match,		\
	},							\
};								\
									\
static struct devfreq_governor ip##_dvfs_gov = {			\
	.name		= #ip "_governor",			\
	.get_target_freq = dvfs_gov_get_target,			\
	.event_handler	= dvfs_gov_event_handler,			\
};

IP_DRIVER(fd, "sprd,hwdvfs-mtx");
IP_DRIVER(isp, "sprd,hwdvfs-isp");
IP_DRIVER(dcam_axi, "sprd,hwdvfs-dcam-axi");
IP_DRIVER(dcam_if, "sprd,hwdvfs-dcam-if");
IP_DRIVER(jpg, "sprd,hwdvfs-jpg");
IP_DRIVER(cpp, "sprd,hwdvfs-cpp");
IP_DRIVER(mtx, "sprd,hwdvfs-mtx");
IP_DRIVER(mmsys, "sprd,hwdvfs-mmsys");

/*
 * The init and exit of one IP: register the ops, the governor and the
 * platform driver, and undo that on the way out.
 */
#define IP_INIT(ip)							\
int ip##_dvfs_init(void)						\
{									\
	int ret;								\
									\
	ret = register_ip_dvfs_ops(&ip##_dvfs_ops);			\
	if (ret) {							\
		pr_err("%s: failed to add ops: %d\n", #ip "_dvfs_init", ret); \
		return ret;						\
	}								\
									\
	ret = devfreq_add_governor(&ip##_dvfs_gov);			\
	if (ret) {							\
		pr_err("%s: failed to add governor: %d\n",		\
		       #ip "_dvfs_init", ret);				\
		unregister_ip_dvfs_ops(&ip##_dvfs_ops);			\
		return ret;						\
	}								\
									\
	ret = platform_driver_register(&ip##_dvfs_driver);		\
	if (ret) {							\
		pr_err("%s: failed to add driver: %d\n",			\
		       #ip "_dvfs_init", ret);				\
		devfreq_remove_governor(&ip##_dvfs_gov);			\
		unregister_ip_dvfs_ops(&ip##_dvfs_ops);			\
		return ret;						\
	}								\
									\
	return 0;							\
}									\
EXPORT_SYMBOL_GPL(ip##_dvfs_init);					\
									\
void ip##_dvfs_exit(void)						\
{									\
	platform_driver_unregister(&ip##_dvfs_driver);			\
	devfreq_remove_governor(&ip##_dvfs_gov);				\
	unregister_ip_dvfs_ops(&ip##_dvfs_ops);				\
}									\
EXPORT_SYMBOL_GPL(ip##_dvfs_exit);

IP_INIT(fd);
IP_INIT(isp);
IP_INIT(dcam_axi);
IP_INIT(dcam_if);
IP_INIT(jpg);
IP_INIT(cpp);
IP_INIT(mtx);
IP_INIT(mmsys);