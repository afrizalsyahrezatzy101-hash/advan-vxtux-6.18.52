====================get_dvfs_table_info_show@0010e024====================
// signature: undefined get_dvfs_table_info_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_dvfs_table_info_show(long param_1,undefined8 param_2,long param_3)

{
  int iVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  code *pcVar4;
  long lVar5;
  undefined8 unaff_x30;
  uint local_230 [2];
  undefined8 local_228;
  uint local_21c [7];
  undefined8 local_200 [50];
  long local_70;
  
  *in_x18 = unaff_x30;
  local_70 = ___stack_chk_guard;
  memset(local_230,0,0x1c0);
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  lVar3 = *(long *)(lVar3 + 0x60);
  if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x60), pcVar4 == (code *)0x0)) {
    printk(&DAT_001059ba,"get_dvfs_table_info_show");
  }
  else {
    if (0xd < ((ulong)(pcVar4 + -0x1175c0) >> 2 | (long)pcVar4 << 0x3e)) {
      __cfi_slowpath(0xe5c7695a0ac14b08,pcVar4);
    }
    (*pcVar4)(param_1 + -0x30,local_230);
  }
  lVar5 = 0;
  lVar3 = 0;
  do {
    iVar1 = sprintf((char *)(param_3 + lVar3),"volt=%s\n",*(undefined8 *)((long)local_200 + lVar5));
    iVar2 = sprintf((char *)(param_3 + lVar3 + iVar1),"clk=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5));
    lVar3 = lVar3 + iVar1 + (long)iVar2;
    iVar1 = sprintf((char *)(param_3 + lVar3),"map_index=%d\n",
                    (ulong)*(uint *)((long)local_230 + lVar5));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"reg_add=%lu\n",
                    *(undefined8 *)((long)&local_228 + lVar5));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"fdiv_denom=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 8));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"fdiv_num=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0xc));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"axi_index=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0x10));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"mtx_index=%d\n\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0x14));
    lVar5 = lVar5 + 0x38;
    lVar3 = lVar3 + iVar1;
  } while (lVar5 != 0x1c0);
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_70) {
    return lVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


