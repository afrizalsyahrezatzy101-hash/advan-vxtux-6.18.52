// apsys_hold_en_store @ 00106a60
// signature: undefined apsys_hold_en_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 apsys_hold_en_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  code *pcVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined4 local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  iVar1 = sscanf(param_3,"%d\n",&local_2c);
  if (iVar1 == 0) {
    param_4 = 0xffffffffffffffea;
  }
  else if ((*(long *)(param_1 + 0x3d0) == 0) ||
          (pcVar2 = *(code **)(*(long *)(param_1 + 0x3d0) + 0x10), pcVar2 == (code *)0x0)) {
    printk(&DAT_0010264a,"apsys_hold_en_store");
  }
  else {
    if (0x13 < ((ulong)(pcVar2 + -0x117d38) >> 2 | (long)pcVar2 << 0x3e)) {
      __cfi_slowpath(0xbd49649486242f1e,pcVar2);
    }
    (*pcVar2)(param_1,local_2c);
  }
  if (___stack_chk_guard == local_28) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


