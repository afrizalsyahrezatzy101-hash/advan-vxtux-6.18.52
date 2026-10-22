====================init_module@00117868====================
// signature: undefined init_module(void)

undefined4 init_module(void)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105e9e);
  mmsys_dvfs_init();
  cpp_dvfs_init();
  fd_dvfs_init();
  isp_dvfs_init();
  jpg_dvfs_init();
  dcam_if_dvfs_init();
  dcam_axi_dvfs_init();
  uVar1 = mtx_dvfs_init();
  printk(&DAT_00105db7);
  return uVar1;
}


