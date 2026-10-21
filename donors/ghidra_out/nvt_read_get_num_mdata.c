// nvt_read_get_num_mdata @ 0012f70c
// signature: undefined nvt_read_get_num_mdata(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nvt_read_get_num_mdata(uint param_1,uint *param_2,uint param_3)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  code *pcVar6;
  long lVar7;
  ulong uVar8;
  ulong uVar9;
  byte *pbVar10;
  ulong uVar11;
  undefined8 *in_x18;
  undefined1 *puVar12;
  long lVar13;
  ulong uVar14;
  undefined8 unaff_x30;
  undefined4 local_178;
  undefined1 local_174 [260];
  long local_70;
  
  *in_x18 = unaff_x30;
  local_70 = ___stack_chk_guard;
  memset(local_174,0,0x102);
  uVar4 = param_1 & 0xff;
  iVar5 = param_3 * 2;
  uVar2 = iVar5 + uVar4;
  if (0xff < (int)uVar2) {
    lVar13 = 0;
    uVar14 = 0;
    puVar12 = &xdata_tmp;
    do {
      uVar3 = (param_1 & 0xffffff00) + (int)uVar14 * 0x100;
      local_178._0_2_ = CONCAT11((char)(uVar3 >> 0xf),0xff);
      local_178._0_3_ = CONCAT12((char)(uVar3 >> 7),(undefined2)local_178);
      local_178 = (uint)(uint3)local_178;
      CTP_SPI_WRITE(*ts,&local_178,3);
      local_174[0] = 0;
      CTP_SPI_READ(*ts,local_174,0x101);
      lVar7 = 0;
      do {
        if (0x270 < (ulong)(lVar13 + lVar7) >> 3) goto LAB_0012f9b0;
        lVar1 = lVar7 + 1;
        puVar12[lVar7] = *(undefined1 *)(((ulong)local_174 | 1) + lVar7);
        lVar7 = lVar1;
      } while (lVar1 != 0x100);
      uVar14 = uVar14 + 1;
      puVar12 = puVar12 + 0x100;
      lVar13 = lVar13 + 0x100;
    } while (uVar14 != uVar2 >> 8);
  }
  param_1 = iVar5 + param_1;
  if ((param_1 & 0xff) != 0) {
    local_178._0_2_ = CONCAT11((char)(param_1 >> 0xf),0xff);
    local_178 = CONCAT12((char)(param_1 >> 7),(undefined2)local_178) & 0xfffeffff;
    CTP_SPI_WRITE(*ts,&local_178,3);
    local_174[0] = 0;
    CTP_SPI_READ(*ts,local_174,0x101);
    uVar2 = uVar2 - (param_1 & 0xff);
    lVar13 = 0;
    if (uVar2 < 0x1389) {
      lVar13 = 5000 - (ulong)uVar2;
    }
    lVar7 = 0;
    do {
      if (lVar13 == lVar7) goto LAB_0012f9b0;
      lVar1 = lVar7 + 1;
      (&xdata_tmp)[lVar7 + (ulong)uVar2] = *(undefined1 *)(((ulong)local_174 | 1) + lVar7);
      lVar7 = lVar1;
    } while (lVar1 != 0x100);
  }
  if (1 < iVar5) {
    uVar11 = (ulong)uVar4;
    uVar14 = (ulong)(param_3 & 0x7fffffff);
    uVar8 = (ulong)(5000 - uVar4 >> 1);
    uVar9 = 0x1389 - uVar11 >> 1;
    lVar13 = uVar11 - 4999;
    pbVar10 = &DAT_00102a5a + uVar11;
    do {
      if (uVar8 == 0) {
LAB_0012f9b0:
                    /* WARNING: Subroutine does not return */
        __ubsan_handle_cfi_check_fail_abort(0x12);
      }
      if ((uVar9 == 0) || (lVar13 == 0)) {
                    /* WARNING: Does not return */
        pcVar6 = (code *)SoftwareBreakpoint(1,0x12f9bc);
        (*pcVar6)();
      }
      uVar9 = uVar9 - 1;
      uVar14 = uVar14 - 1;
      uVar8 = uVar8 - 1;
      *param_2 = (int)(short)((ushort)*pbVar10 << 8) | (uint)pbVar10[-1];
      lVar13 = lVar13 + 2;
      pbVar10 = pbVar10 + 2;
      param_2 = param_2 + 1;
    } while (uVar14 != 0);
  }
  local_178._0_2_ = CONCAT11((char)(*(uint *)ts[0x1f] >> 0xf),0xff);
  local_178._0_3_ = CONCAT12((char)(*(uint *)ts[0x1f] >> 7),(undefined2)local_178);
  local_178 = (uint)(uint3)local_178;
  CTP_SPI_WRITE(*ts,&local_178,3);
  if (___stack_chk_guard == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


