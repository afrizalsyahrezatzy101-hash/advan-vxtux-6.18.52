// trusty_dump_logs @ 00101d1c
// signature: undefined trusty_dump_logs(void)
// calling convention: unknown


void trusty_dump_logs(long param_1,long param_2)

{
  undefined *puVar1;
  char cVar2;
  code *pcVar3;
  int iVar4;
  uint uVar5;
  ulong uVar6;
  ulong uVar7;
  long lVar8;
  undefined8 *in_x18;
  int *piVar9;
  int iVar10;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(int *)(param_1 + 0x3c) == 0) {
    if ((trusty_dump_logs___print_once & 1) == 0) {
      trusty_dump_logs___print_once = 1;
      printk(&DAT_00100558);
    }
  }
  else {
    piVar9 = *(int **)(param_1 + 0x18);
    uVar6 = (ulong)(uint)piVar9[2] - ((ulong)((uint)piVar9[2] >> 1) & 0x5555555555555555);
    uVar6 = (uVar6 & 0x3333333333333333) + (uVar6 >> 2 & 0x3333333333333333);
    if ((uVar6 + (uVar6 >> 4) & 0xf0f0f0f0f0f0f0f) * 0x101010101010101 >> 0x38 != 1) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0x800,0x101ecc);
      (*pcVar3)();
    }
    iVar10 = *(int *)(param_1 + 0x20);
    iVar4 = piVar9[1];
    if (iVar4 != iVar10) {
      puVar1 = &UNK_001003fc;
      if (param_2 != 2) {
        puVar1 = &DAT_001003dd;
      }
      do {
        DataSynchronizationBarrier(3,1,0);
        uVar6 = (ulong)(iVar4 - iVar10);
        if (0xfe < uVar6) {
          uVar6 = 0xff;
        }
        if (uVar6 == 0) {
          uVar7 = 0;
          uVar5 = 0;
        }
        else {
          lVar8 = *(long *)(param_1 + 0x18);
          uVar7 = 0;
          iVar4 = *(int *)(lVar8 + 8);
          do {
            cVar2 = *(char *)(lVar8 + (ulong)(iVar10 + (int)uVar7 & iVar4 - 1U) + 0xc);
            *(char *)(param_1 + 0x70 + uVar7) = cVar2;
            uVar7 = uVar7 + 1;
            if (cVar2 == '\n') break;
          } while (uVar7 < uVar6);
          uVar5 = (uint)uVar7;
          if (0xff < uVar5) {
            __ubsan_handle_cfi_check_fail_abort(0x12);
                    /* WARNING: Does not return */
            pcVar3 = (code *)UndefinedInstructionException(0,0x101ed8);
            (*pcVar3)();
          }
        }
        *(undefined1 *)(param_1 + uVar7 + 0x70) = 0;
        DataSynchronizationBarrier(3,1,0);
        iVar4 = *piVar9;
        if ((uint)piVar9[2] < (uint)(iVar4 - iVar10)) {
          printk(&DAT_00100323);
          iVar10 = iVar4 - piVar9[2];
        }
        else {
          printk(puVar1,param_1 + 0x70);
          iVar10 = uVar5 + iVar10;
        }
        iVar4 = piVar9[1];
      } while (iVar4 != iVar10);
    }
    *(int *)(param_1 + 0x20) = iVar10;
  }
  return;
}


