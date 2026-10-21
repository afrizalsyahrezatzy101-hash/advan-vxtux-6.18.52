====================top_current_volt.931@00113c9c====================
// signature: undefined top_current_volt.931(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_931(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


