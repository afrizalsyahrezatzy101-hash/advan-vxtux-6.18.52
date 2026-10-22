// set_hw_dfs_store.890 @ 00116948
// signature: undefined set_hw_dfs_store.890(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_hw_dfs_store_890(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  ulong uVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  int local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_3c = 0;
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_001027d0);
  }
  else {
    iVar1 = sscanf(param_3,"%d\n",&local_3c);
    if (iVar1 != 0) {
      if ((*(long *)(lVar4 + 0x480) == 0) ||
         (pcVar3 = *(code **)(*(long *)(lVar4 + 0x480) + 0x10), pcVar3 == (code *)0x0)) {
        printk(&DAT_0010230e,"set_hw_dfs_store");
      }
      else {
        if (4 < ((ulong)(pcVar3 + -0x118234) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0x488f92cb4d3438f1,pcVar3);
        }
        (*pcVar3)(lVar4,local_3c != 0);
        *(int *)(lVar4 + 0x400) = local_3c;
      }
      goto LAB_00116a10;
    }
  }
  param_4 = 0xffffffffffffffea;
LAB_00116a10:
  if (___stack_chk_guard == local_38) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


