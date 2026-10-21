// get_dvfs_table_info_show @ 00109000
// signature: undefined get_dvfs_table_info_show(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_dvfs_table_info_show(long param_1,undefined8 param_2,void *param_3)

{
  int iVar1;
  ulong uVar2;
  undefined8 *in_x18;
  code *pcVar3;
  long lVar4;
  long lVar5;
  uint *puVar6;
  undefined8 unaff_x30;
  undefined1 local_118 [8];
  uint auStack_110 [46];
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  memset(local_118,0,0xc0);
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_0010291a);
    lVar4 = -0x16;
  }
  else if ((*(long *)(lVar5 + 1000) == 0) ||
          (pcVar3 = *(code **)(*(long *)(lVar5 + 1000) + 0x38), pcVar3 == (code *)0x0)) {
    printk(&DAT_0010230e,"get_dvfs_table_info_show");
    lVar4 = 0;
  }
  else {
    if (0xd < ((ulong)(pcVar3 + -0x1181ec) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x3dd94311f7d88ad,pcVar3);
    }
    (*pcVar3)(local_118);
    memcpy(param_3,"map_index\tvolt_level\tclk_level\tclk_rate\n",0x29);
    if (*(int *)(lVar5 + 0x4a0) == 0) {
      lVar4 = 0x28;
    }
    else {
      uVar2 = 0;
      lVar4 = 0x28;
      puVar6 = auStack_110;
      do {
        if (uVar2 == 8) {
          __ubsan_handle_cfi_check_fail_abort(0x12);
          goto LAB_00109160;
        }
        iVar1 = sprintf((char *)((long)param_3 + lVar4),"%d\t\t%d (%s)\t%d\t\t%d\t\t\n",
                        (ulong)puVar6[-2],(ulong)puVar6[-1],*(undefined8 *)(puVar6 + 2),
                        (ulong)*puVar6,(ulong)puVar6[1]);
        uVar2 = uVar2 + 1;
        lVar4 = lVar4 + iVar1;
        puVar6 = puVar6 + 6;
      } while (uVar2 < *(uint *)(lVar5 + 0x4a0));
    }
  }
  if (___stack_chk_guard == local_58) {
    return lVar4;
  }
LAB_00109160:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


