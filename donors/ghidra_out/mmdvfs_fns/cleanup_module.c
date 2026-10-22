====================cleanup_module@001178c8====================
// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105530);
  fd_dvfs_exit();
  cpp_dvfs_exit();
  isp_dvfs_exit();
  jpg_dvfs_exit();
  dcam_if_dvfs_exit();
  dcam_axi_dvfs_exit();
  mtx_dvfs_exit();
  mmsys_dvfs_exit();
  printk(&DAT_00104caa);
  return;
}


