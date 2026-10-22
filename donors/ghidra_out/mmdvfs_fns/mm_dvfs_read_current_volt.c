====================mm_dvfs_read_current_volt@0010c21c====================
// signature: undefined mm_dvfs_read_current_volt(void)

uint mm_dvfs_read_current_volt(void)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  return uVar1 >> 0x18 & 7;
}


