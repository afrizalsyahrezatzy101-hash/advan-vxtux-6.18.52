// trusty_console_store @ 00101548
// signature: undefined trusty_console_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void trusty_console_store(long param_1,undefined8 param_2,char *param_3,size_t param_4)

{
  int iVar1;
  size_t sVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  ulong local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  iVar1 = kstrtoull(param_3,0,&local_40);
  if (iVar1 == 0) {
    iVar1 = trusty_std_call32(*(undefined8 *)(*(long *)(param_1 + 0x98) + 8),0x35000000,
                              local_40 & 0xffffffff,0,0);
    if (iVar1 == -1) {
      printk(&DAT_001004b1);
      sVar2 = 0xfffffffffffffff2;
    }
    else {
      sVar2 = strnlen(param_3,param_4);
    }
  }
  else {
    printk(&DAT_0010051f,param_3);
    sVar2 = 0xffffffffffffffea;
  }
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(sVar2);
}


