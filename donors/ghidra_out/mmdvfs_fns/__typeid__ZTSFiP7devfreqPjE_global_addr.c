====================__typeid__ZTSFiP7devfreqPjE_global_addr@00117744====================
// signature: undefined __typeid__ZTSFiP7devfreqPjE_global_addr(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 __typeid__ZTSFiP7devfreqPjE_global_addr(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint uStack_2c;
  long lStack_28;
  
  *in_x18 = unaff_x30;
  lStack_28 = ___stack_chk_guard;
  uStack_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&uStack_2c);
  if (iVar1 == 0) {
    uStack_2c = uStack_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = uStack_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == lStack_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


