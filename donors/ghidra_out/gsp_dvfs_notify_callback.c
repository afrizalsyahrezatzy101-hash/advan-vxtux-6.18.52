// gsp_dvfs_notify_callback @ 00116f5c
// signature: undefined gsp_dvfs_notify_callback(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void gsp_dvfs_notify_callback(long param_1,undefined8 param_2,uint *param_3)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  code *pcVar4;
  undefined8 *in_x18;
  ulong uVar5;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  uVar1 = *param_3;
  uVar5 = (ulong)uVar1;
  local_2c = 0;
  regmap_read(regmap_aon_base,0,&local_2c);
  if ((local_2c >> 0x15 & 1) == 0) {
    printk(&DAT_00103627,"gsp_dvfs_notify_callback");
  }
  else if (*(int *)(param_1 + -0x3c8) != 0) {
    if ((*(long *)(param_1 + 0xb8) != 0) &&
       (pcVar4 = *(code **)(*(long *)(param_1 + 0xb8) + 0x38), pcVar4 != (code *)0x0)) {
      if (2 < ((ulong)(pcVar4 + -0x118228) >> 2 | (long)pcVar4 << 0x3e)) {
        __cfi_slowpath(0xccb9aad54a2c9f6d,pcVar4);
      }
      (*pcVar4)((int *)(param_1 + -0x3c8),uVar5);
    }
    lVar2 = *(long *)(param_1 + -0x18);
    if (*(long *)(*(long *)(lVar2 + 0x3d8) + 0x30) != 0) {
      devfreq_update_status(lVar2,uVar5);
      lVar2 = *(long *)(param_1 + -0x18);
    }
    *(uint *)(param_1 + 0x18) = uVar1;
    *(undefined4 *)(param_1 + 0x20) = 0;
    *(ulong *)(lVar2 + 0x498) = uVar5;
    uVar3 = 1;
    goto LAB_00117030;
  }
  uVar3 = 0;
LAB_00117030:
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}


