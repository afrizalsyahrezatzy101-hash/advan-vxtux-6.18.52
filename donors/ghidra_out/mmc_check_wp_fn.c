// mmc_check_wp_fn @ 001088cc
// signature: undefined mmc_check_wp_fn(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int mmc_check_wp_fn(long *param_1)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  char *local_30;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_30 = (char *)0x0;
  iVar1 = of_property_read_string(*(undefined8 *)(*param_1 + 0x300),"sprd,wp_fn",&local_30);
  if (iVar1 == 0) {
    iVar1 = strcmp(local_30,"enable");
    iVar1 = -(uint)(iVar1 != 0);
  }
  else {
    lVar2 = param_1[0xf];
    if (lVar2 == 0) {
      lVar2 = param_1[1];
    }
    printk(&DAT_0010176c,lVar2);
  }
  if (___stack_chk_guard == local_28) {
    return iVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


