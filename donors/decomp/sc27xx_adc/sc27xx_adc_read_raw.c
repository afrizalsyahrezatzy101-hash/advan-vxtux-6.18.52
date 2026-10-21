// signature: undefined sc27xx_adc_read_raw(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sc27xx_adc_read_raw(long param_1,long param_2,uint *param_3,undefined8 param_4,long param_5)

{
  long lVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint *puVar7;
  int *piVar8;
  ulong uVar9;
  uint uVar10;
  undefined8 *in_x18;
  uint uVar11;
  code *pcVar12;
  undefined8 unaff_x30;
  uint local_50;
  int local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  if (0x1f < *(uint *)(param_2 + 4)) {
    __ubsan_handle_cfi_check_fail_abort(0x12);
    goto LAB_00104068;
  }
  lVar1 = param_1 + 0x5c0;
  uVar11 = *(uint *)(lVar1 + (ulong)*(uint *)(param_2 + 4) * 4 + 0x28);
  local_50 = 0;
  if (param_5 == 2) {
LAB_00104024:
    *param_3 = uVar11;
    iVar5 = 1;
  }
  else if (param_5 == 1) {
    lVar2 = param_1 + 1000;
    mutex_lock(lVar2);
    iVar3 = *(int *)(param_2 + 4);
    local_4c = 0;
    iVar5 = sc27xx_adc_read(lVar1,iVar3,uVar11,&local_4c);
    if (iVar5 == 0) {
      puVar7 = *(uint **)(param_1 + 0x670);
      if ((*puVar7 & 0xfffffffe) == 2) {
        if (iVar3 == 0xb) {
          piVar8 = &big_scale_graph;
LAB_00103ec4:
          iVar4 = piVar8[1] - piVar8[3];
          iVar5 = 0;
          if (iVar4 != 0) {
            iVar5 = ((local_4c - piVar8[3]) * (*piVar8 - piVar8[2])) / iVar4;
          }
          uVar10 = iVar5 + piVar8[2];
        }
        else {
          if (iVar3 == 0) {
            if (uVar11 == 1) {
              piVar8 = &ump9620_bat_det_graph;
              goto LAB_00103ef0;
            }
          }
          else if (uVar11 == 1) {
            piVar8 = &ump9620_bat_det_graph;
            goto LAB_00103ec4;
          }
          iVar5 = 0;
          if (DAT_0010010c - DAT_00100114 != 0) {
            iVar5 = ((local_4c - DAT_00100114) * (small_scale_graph - DAT_00100110)) /
                    (DAT_0010010c - DAT_00100114);
          }
          uVar10 = iVar5 + DAT_00100110;
          if (uVar11 == 3) {
            iVar5 = 0xfdc;
          }
          else {
            if (uVar11 != 2) goto LAB_00103fdc;
            iVar5 = 0xa28;
          }
          uVar10 = (int)(uVar10 * iVar5) / 1000;
        }
LAB_00103fdc:
        pcVar12 = *(code **)(puVar7 + 0xc);
        uVar10 = uVar10 & ((int)uVar10 >> 0x1f ^ 0xffffffffU);
        uVar9 = (ulong)(pcVar12 + -0x1051f8) >> 2 | (long)pcVar12 << 0x3e;
joined_r0x00103f5c:
        if (4 < uVar9) {
          __cfi_slowpath(0x6cf58e448911dfd5,pcVar12);
        }
        uVar6 = (*pcVar12)(iVar3,uVar11);
        uVar11 = 0;
        if (uVar6 >> 0x10 != 0) {
          uVar11 = ((uVar6 & 0xffff) * uVar10 + (uVar6 >> 0x11)) / (uVar6 >> 0x10);
        }
      }
      else {
        if (iVar3 == 1) {
          piVar8 = &small_scale_graph;
        }
        else {
          if (iVar3 != 5) {
            pcVar12 = *(code **)(puVar7 + 0xc);
            iVar5 = 0;
            if (DAT_0010010c - DAT_00100114 != 0) {
              iVar5 = ((local_4c - DAT_00100114) * (small_scale_graph - DAT_00100110)) /
                      (DAT_0010010c - DAT_00100114);
            }
            uVar9 = (ulong)(pcVar12 + -0x1051f8) >> 2 | (long)pcVar12 << 0x3e;
            uVar10 = iVar5 + DAT_00100110 & (iVar5 + DAT_00100110 >> 0x1f ^ 0xffffffffU);
            goto joined_r0x00103f5c;
          }
          piVar8 = &big_scale_graph;
        }
LAB_00103ef0:
        iVar5 = piVar8[1] - piVar8[3];
        iVar3 = 0;
        if (iVar5 != 0) {
          iVar3 = ((local_4c - piVar8[3]) * (*piVar8 - piVar8[2])) / iVar5;
        }
        uVar11 = iVar3 + piVar8[2];
        uVar11 = uVar11 & ((int)uVar11 >> 0x1f ^ 0xffffffffU);
      }
      mutex_unlock(lVar2);
      goto LAB_00104024;
    }
    mutex_unlock(lVar2);
  }
  else if (param_5 == 0) {
    mutex_lock(param_1 + 1000);
    iVar5 = sc27xx_adc_read(lVar1,*(undefined4 *)(param_2 + 4),uVar11,&local_50);
    mutex_unlock(param_1 + 1000);
    if (iVar5 == 0) {
      iVar5 = 1;
      *param_3 = local_50;
    }
  }
  else {
    iVar5 = -0x16;
  }
  if (___stack_chk_guard == local_48) {
    return iVar5;
  }
LAB_00104068:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



