// set_hw_dfs_store.189 @ 0010acc4
// signature: undefined set_hw_dfs_store.189(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_hw_dfs_store_189(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  code *pcVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  int local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_2c = 0;
  iVar1 = sscanf(param_3,"%d\n",&local_2c);
  if (iVar1 == 0) {
    param_4 = 0xffffffffffffffea;
  }
  else if ((*(long *)(lVar3 + 0x480) == 0) ||
          (pcVar2 = *(code **)(*(long *)(lVar3 + 0x480) + 0x18), pcVar2 == (code *)0x0)) {
    printk(&DAT_0010230e,"set_hw_dfs_store");
  }
  else {
    if (9 < ((ulong)(pcVar2 + -0x117fd8) >> 2 | (long)pcVar2 << 0x3e)) {
      __cfi_slowpath(0xf63912c290b9feb1,pcVar2);
    }
    (*pcVar2)(lVar3,local_2c != 0);
    *(int *)(lVar3 + 0x400) = local_2c;
  }
  if (___stack_chk_guard == local_28) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


