====================top_mm_dvfs_current_volt@0010c174====================
// signature: undefined top_mm_dvfs_current_volt(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void top_mm_dvfs_current_volt(void)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_1c;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  local_1c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_1c);
  if (iVar1 == 0) {
    local_1c = local_1c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_1c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  if (___stack_chk_guard == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


