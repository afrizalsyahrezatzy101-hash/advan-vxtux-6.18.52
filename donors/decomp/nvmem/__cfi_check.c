// signature: undefined __cfi_check(void)

void __cfi_check(long param_1,long param_2,undefined8 param_3)

{
  undefined1 *puVar1;
  long lVar2;
  
  if (param_1 < 0x2b3a43e29242445) {
    if (param_1 == -0x60ced7460c6fa41e) {
      puVar1 = &LAB_001020dc;
    }
    else {
      if (param_1 != -0x3557318fb8d07b71) goto LAB_001020bc;
      puVar1 = &LAB_001020d4;
    }
    if (1 < ((ulong)(param_2 - (long)puVar1) >> 2 | param_2 << 0x3e)) {
LAB_001020bc:
      __cfi_check_fail(param_3);
      return;
    }
  }
  else {
    if (param_1 == 0x2b3a43e29242445) {
      lVar2 = 0x1020d0;
    }
    else {
      if (param_1 != 0x7e04a0fb7ad8bcd5) goto LAB_001020bc;
      lVar2 = 0x1020e4;
    }
    if (param_2 != lVar2) goto LAB_001020bc;
  }
  return;
}



