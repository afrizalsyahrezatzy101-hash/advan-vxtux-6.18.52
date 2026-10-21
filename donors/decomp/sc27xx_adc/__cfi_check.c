// signature: undefined __cfi_check(void)

void __cfi_check(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  undefined1 *puVar2;
  
  if (param_1 < 0x2b3a43e29242445) {
    if (param_1 < -0x3557318fb8d07b71) {
      if (param_1 != -0x57be845bca9148ef) {
        if (param_1 != -0x4c69443ff61e2fc2) goto LAB_00105184;
        lVar1 = 0x105218;
        goto LAB_001051ac;
      }
      puVar2 = &LAB_001051e0;
    }
    else {
      if (param_1 != -0x3557318fb8d07b71) {
        if ((param_1 == -0x137bcfca45220ff5) && ((param_2 - 0x1051e8U >> 2 | param_2 << 0x3e) < 4))
        {
          return;
        }
        goto LAB_00105184;
      }
      puVar2 = &LAB_001051d8;
    }
LAB_001051c0:
    if (1 < ((ulong)(param_2 - (long)puVar2) >> 2 | param_2 << 0x3e)) goto LAB_00105184;
  }
  else {
    if (param_1 < 0x4c550309df1cf4c1) {
      if (param_1 == 0x2b3a43e29242445) {
        lVar1 = 0x1051d4;
      }
      else {
        if (param_1 != 0xb6cdfc3c9d38873) goto LAB_00105184;
        lVar1 = 0x105214;
      }
    }
    else {
      if (param_1 == 0x4c550309df1cf4c1) {
        puVar2 = &LAB_0010520c;
        goto LAB_001051c0;
      }
      if (param_1 == 0x6cf58e448911dfd5) {
        if ((param_2 - 0x1051f8U >> 2 | param_2 << 0x3e) < 5) {
          return;
        }
        goto LAB_00105184;
      }
      if (param_1 != 0x7e04a0fb7ad8bcd5) goto LAB_00105184;
      lVar1 = 0x10521c;
    }
LAB_001051ac:
    if (param_2 != lVar1) {
LAB_00105184:
      __cfi_check_fail(param_3);
      return;
    }
  }
  return;
}



