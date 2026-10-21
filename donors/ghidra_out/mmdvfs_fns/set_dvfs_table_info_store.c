====================set_dvfs_table_info_store@0010e22c====================
// signature: undefined set_dvfs_table_info_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_table_info_store(long param_1,undefined8 param_2,char *param_3,undefined8 *param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  code *extraout_x8;
  undefined8 *in_x18;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined8 unaff_x30;
  ulong local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  lVar5 = param_1 + -0x20;
  local_68 = ___stack_chk_guard;
  local_78 = 0;
  uStack_70 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_a0 = 0;
  lVar6 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  puVar4 = &local_78;
  mutex_lock(lVar5);
  iVar1 = sscanf(param_3," %d,%lu,%d,%d,%d,%d,%d,%d,\n",&local_a0,&local_98,&uStack_90,&local_88,
                 (long)&local_88 + 4,&uStack_80,(long)&uStack_80 + 4,puVar4);
  if (iVar1 != 8) {
    mutex_unlock(lVar5);
    lVar5 = -0x16;
    goto LAB_0010e368;
  }
  if ((uint)local_a0 < 9) {
    lVar2 = lVar6 + (local_a0 & 0xffffffff) * 0x38;
    *(undefined8 *)(lVar2 + 0x130) = local_98;
    *(undefined8 *)(lVar2 + 0x138) = uStack_90;
    *(ulong *)(lVar2 + 0x128) = local_a0;
    *(undefined8 *)(lVar2 + 0x150) = local_78;
    *(undefined8 *)(lVar2 + 0x158) = uStack_70;
    *(undefined8 *)(lVar2 + 0x140) = local_88;
    *(undefined8 *)(lVar2 + 0x148) = uStack_80;
    lVar6 = *(long *)(lVar6 + 0x60);
    iVar1 = (int)param_4;
    if ((lVar6 != 0) && (pcVar3 = *(code **)(lVar6 + 0x68), pcVar3 != (code *)0x0)) {
      param_1 = param_1 + -0x30;
      puVar4 = param_4;
      if (0xd < ((ulong)(pcVar3 + -0x1175c0) >> 2 | (long)pcVar3 << 0x3e)) goto LAB_0010e3c8;
      goto LAB_0010e340;
    }
    printk(&DAT_001059ba,"set_dvfs_table_info_store");
  }
  else {
    __ubsan_handle_cfi_check_fail_abort(0x12);
    pcVar3 = extraout_x8;
LAB_0010e3c8:
    iVar1 = (int)puVar4;
    __cfi_slowpath(0xe5c7695a0ac14b08,pcVar3);
LAB_0010e340:
    (*pcVar3)(param_1,&local_a0);
  }
  mutex_unlock(lVar5);
  lVar5 = (long)iVar1;
LAB_0010e368:
  if (___stack_chk_guard == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar5);
}


