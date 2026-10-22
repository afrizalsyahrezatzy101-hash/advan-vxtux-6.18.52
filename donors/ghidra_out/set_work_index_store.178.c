// set_work_index_store.178 @ 0010a84c
// signature: undefined set_work_index_store.178(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_work_index_store_178(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined4 local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_2c = 0;
  iVar1 = sscanf(param_3,"%d\n",&local_2c);
  if (iVar1 == 0) {
    param_4 = 0xffffffffffffffea;
  }
  else {
    lVar2 = *(long *)(lVar4 + 0x480);
    if ((lVar2 == 0) || (pcVar3 = *(code **)(lVar2 + 0x20), pcVar3 == (code *)0x0)) {
      printk(&DAT_0010230e,"set_work_index_store");
    }
    else {
      if (1 < ((ulong)(pcVar3 + -0x117f80) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0x20b272794e04135f,pcVar3);
      }
      iVar1 = (*pcVar3)(lVar4,local_2c);
      if (iVar1 != 0) {
        printk(&DAT_001035b4,"set_work_index_store",iVar1,local_2c);
        param_4 = 0xfffffffffffffff2;
      }
    }
  }
  if (___stack_chk_guard == local_28) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


