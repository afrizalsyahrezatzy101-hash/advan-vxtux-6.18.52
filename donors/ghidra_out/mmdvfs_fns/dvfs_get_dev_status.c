====================dvfs_get_dev_status@0010cbec====================
// signature: undefined dvfs_get_dev_status(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void dvfs_get_dev_status(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  long *plVar3;
  undefined8 unaff_x30;
  undefined8 local_38;
  undefined8 local_30;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  plVar3 = *(long **)(param_1 + 0x98);
  local_38 = 0;
  local_30 = 0;
  printk(&DAT_00105589);
  if (*plVar3 == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = devfreq_event_get_event(plVar3[0xb],&local_38);
    if (-1 < (int)uVar1) {
      lVar2 = plVar3[4];
      param_2[1] = local_38;
      param_2[2] = lVar2;
      *param_2 = local_30;
    }
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}


