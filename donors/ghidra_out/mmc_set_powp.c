// mmc_set_powp @ 00108978
// signature: undefined mmc_set_powp(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void mmc_set_powp(long *param_1)

{
  int iVar1;
  ulong uVar2;
  undefined *puVar3;
  long lVar4;
  long lVar5;
  undefined8 *in_x18;
  long *plVar6;
  uint uVar7;
  undefined8 unaff_x30;
  undefined8 local_1f8;
  undefined8 *local_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined8 local_1d8;
  undefined8 uStack_1d0;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 uStack_1b0;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  undefined8 local_198;
  undefined8 uStack_190;
  undefined8 local_188;
  undefined8 uStack_180;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 local_168;
  undefined8 *local_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  ulong local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 local_80;
  ulong local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  plVar6 = (long *)*param_1;
  if ((int)plVar6[0x76] != 0) {
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    printk(&DAT_00100f23,lVar4);
    uVar2 = 0xffffffff;
    goto LAB_00108bf8;
  }
  local_168 = 0;
  iVar1 = of_property_read_variable_u32_array
                    (*(undefined8 *)(*plVar6 + 0x300),"sprd,part-address-size",&local_168,2,0);
  if (iVar1 < 0) {
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    puVar3 = &DAT_00101018;
LAB_00108a7c:
    printk(puVar3,lVar4);
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    printk(&DAT_0010113e,lVar4);
  }
  else {
    _DAT_00100660 = local_168 & 0xffffffff;
    DAT_00100668 = (ulong)local_168._4_4_;
    DAT_00100658 = *(int *)((long)param_1 + 0x4c4) * (uint)*(byte *)((long)param_1 + 0x526);
    uVar7 = 0;
    if (DAT_00100658 != 0) {
      uVar7 = local_168._4_4_ / DAT_00100658;
    }
    if (local_168._4_4_ != uVar7 * DAT_00100658) {
      lVar4 = plVar6[0xf];
      if (lVar4 == 0) {
        lVar4 = plVar6[1];
      }
      puVar3 = &DAT_00101d25;
      goto LAB_00108a7c;
    }
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    DAT_00100654 = uVar7;
    printk(&DAT_00100f3c,lVar4);
  }
  if (DAT_00100650 == 0) {
    mmc_get_wp_current_status_and_type(param_1);
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    puVar3 = &DAT_0010144b;
  }
  else {
    lVar4 = *param_1;
    local_160 = &local_98;
    local_168 = 0;
    local_98 = 0x23b000100000006;
    uStack_90 = 0;
    local_a8 = 0;
    uStack_a0 = 0;
    local_b8 = 0;
    uStack_b0 = 0;
    local_c8 = 0;
    local_c0 = 0;
    local_d8 = 0;
    uStack_d0 = 0;
    local_178 = 0;
    uStack_170 = 0;
    local_188 = 0;
    uStack_180 = 0;
    local_198 = 0;
    uStack_190 = 0;
    local_1a8 = 0;
    uStack_1a0 = 0;
    local_1b8 = 0;
    uStack_1b0 = 0;
    local_1c8 = 0;
    uStack_1c0 = 0;
    local_1d8 = 0;
    uStack_1d0 = 0;
    local_1e8 = 0;
    uStack_1e0 = 0;
    local_1f8 = 0;
    local_1f0 = (undefined8 *)0x0;
    local_68 = 0;
    uStack_60 = 0;
    local_78 = 0;
    uStack_70 = 0;
    local_88 = 0;
    local_e8 = 0;
    uStack_e0 = 0;
    local_f8 = 0;
    uStack_f0 = 0;
    local_108 = 0;
    uStack_100 = 0;
    local_118 = 0;
    uStack_110 = 0;
    local_128 = 0;
    uStack_120 = 0;
    local_138 = 0;
    uStack_130 = 0;
    local_148 = 0;
    uStack_140 = 0;
    local_158 = 0;
    uStack_150 = 0;
    local_80 = 0x49d;
    mmc_wait_for_req(lVar4,&local_168);
    if ((int)local_78 == 0) {
      local_98 = CONCAT44(0x2af0101,(undefined4)local_98);
      mmc_wait_for_req(*param_1,&local_168);
      if ((int)local_78 != 0) {
        lVar5 = *(long *)(lVar4 + 0x78);
        if (lVar5 == 0) {
          lVar5 = *(long *)(lVar4 + 8);
        }
        puVar3 = &DAT_00101081;
        goto LAB_00108bec;
      }
      local_98 = CONCAT44(0x1ab0101,(undefined4)local_98);
      mmc_wait_for_req(*param_1,&local_168);
      if ((int)local_78 != 0) {
        lVar5 = *(long *)(lVar4 + 0x78);
        if (lVar5 == 0) {
          lVar5 = *(long *)(lVar4 + 8);
        }
        puVar3 = &DAT_00101555;
        goto LAB_00108bec;
      }
    }
    else {
      lVar5 = *(long *)(lVar4 + 0x78);
      if (lVar5 == 0) {
        lVar5 = *(long *)(lVar4 + 8);
      }
      puVar3 = &DAT_00100a00;
LAB_00108bec:
      printk(puVar3,lVar5);
      uVar2 = local_78 & 0xffffffff;
      if ((int)local_78 != 0) goto LAB_00108bf8;
    }
    local_1f0 = &local_d8;
    local_d8 = CONCAT44(local_d8._4_4_,0x1c);
    local_c0 = CONCAT44(local_c0._4_4_,0x49d);
    if (*(int *)(lVar4 + 0x3b0) == 0) {
      uVar7 = 0;
      do {
        if (DAT_00100654 <= uVar7) goto LAB_00108cc0;
        local_d8 = CONCAT44(DAT_00100660 + DAT_00100658 * uVar7,(undefined4)local_d8);
        mmc_wait_for_req(*param_1,&local_1f8);
        if ((int)local_b8 != 0) goto LAB_00108ca0;
        uVar7 = uVar7 + 1;
      } while (*(int *)(lVar4 + 0x3b0) == 0);
    }
    __ubsan_handle_cfi_check_fail_abort(0x12);
LAB_00108ca0:
    lVar5 = *(long *)(lVar4 + 0x78);
    if (lVar5 == 0) {
      lVar5 = *(long *)(lVar4 + 8);
    }
    printk(&DAT_00100de2,lVar5);
    uVar2 = local_b8 & 0xffffffff;
    if ((int)local_b8 != 0) goto LAB_00108bf8;
LAB_00108cc0:
    mmc_get_wp_current_status_and_type(param_1);
    lVar4 = plVar6[0xf];
    if (lVar4 == 0) {
      lVar4 = plVar6[1];
    }
    puVar3 = &DAT_00100a9a;
  }
  printk(puVar3,lVar4);
  uVar2 = 0;
LAB_00108bf8:
  if (___stack_chk_guard == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


