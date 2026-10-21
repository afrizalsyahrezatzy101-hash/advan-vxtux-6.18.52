// n6pro_soc_ver_id_check @ 001060cc
// signature: undefined n6pro_soc_ver_id_check(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void n6pro_soc_ver_id_check(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  int local_1c;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  local_1c = 0;
  iVar1 = sprd_get_soc_id(4,&local_1c,1);
  if (iVar1 == 0) {
    if (local_1c == 1) {
      printk(&DAT_00102a7e);
      iVar1 = local_1c;
    }
    else if (local_1c == 0) {
      printk(&DAT_001023ef);
      iVar1 = local_1c;
    }
    else {
      printk(&DAT_00102820);
      local_1c = 0;
      iVar1 = local_1c;
    }
  }
  else {
    printk(&DAT_00102af4);
    iVar1 = 0;
  }
  if (___stack_chk_guard != local_18) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}


