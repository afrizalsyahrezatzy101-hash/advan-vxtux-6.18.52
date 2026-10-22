====================mm_current_volt.1009@00114df0====================
// signature: undefined mm_current_volt.1009(void)

undefined8 mm_current_volt_1009(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}


