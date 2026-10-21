====================set_mmsys_power_store@0010ab74====================
// signature: undefined set_mmsys_power_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_mmsys_power_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  code *pcVar2;
  undefined8 *in_x18;
  ulong *puVar3;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  printk(&DAT_0010519c,"set_mmsys_power_store");
  puVar3 = *(ulong **)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar1 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar1 == 1) {
    if (puVar3 != (ulong *)0x0) {
      *puVar3 = local_50;
      if ((puVar3[4] != 0) && (pcVar2 = *(code **)(puVar3[4] + 0x128), pcVar2 != (code *)0x0)) {
        if (0xe < ((ulong)(pcVar2 + -0x11782c) >> 2 | (long)pcVar2 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar2);
        }
        (*pcVar2)(param_1 + -0x30,local_50 & 0xffffffff);
        goto LAB_0010ac3c;
      }
    }
    printk(&DAT_001047b2,"set_mmsys_power_store");
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
LAB_0010ac3c:
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


