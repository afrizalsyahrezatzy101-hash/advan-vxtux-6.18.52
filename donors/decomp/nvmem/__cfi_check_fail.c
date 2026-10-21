// signature: undefined __cfi_check_fail(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte * __cfi_check_fail(byte *param_1)

{
  long lVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  long lVar7;
  ulong uVar8;
  char *pcVar9;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  undefined8 uVar10;
  ulong uVar11;
  undefined8 unaff_x30;
  long lStack_c8;
  char *pcStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined1 *puStack_90;
  undefined8 uStack_88;
  int iStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  long *plStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  long lStack_58;
  undefined1 *puStack_50;
  code *pcStack_48;
  
  *in_x18 = unaff_x30;
  if ((param_1 != (byte *)0x0) && (4 < *param_1)) {
    return param_1;
  }
  lVar4 = __ubsan_handle_cfi_check_fail_abort(2);
  *extraout_x18 = sc27xx_efuse_probe;
  pcStack_48 = sc27xx_efuse_probe;
  lVar1 = lVar4 + 0x10;
  lStack_58 = ___stack_chk_guard;
  uVar10 = *(undefined8 *)(lVar4 + 0x310);
  uStack_68 = 0;
  uStack_60 = 0;
  uStack_78 = 0;
  uStack_74 = 0;
  plStack_70 = (long *)0x0;
  uStack_88 = 0;
  iStack_80 = 0;
  uStack_7c = 0;
  uStack_98 = 0;
  puStack_90 = (undefined1 *)0x0;
  uStack_a8 = 0;
  uStack_a0 = 0;
  uStack_b8 = 0;
  uStack_b0 = 0;
  lStack_c8 = 0;
  pcStack_c0 = (char *)0x0;
  puStack_50 = &stack0xfffffffffffffff0;
  lVar5 = of_device_get_match_data(lVar1);
  if (lVar5 == 0) {
    _dev_err(lVar1,"No matching driver data found\n");
    uVar11 = 0xffffffea;
    goto LAB_001011c4;
  }
  plVar6 = (long *)devm_kmalloc(lVar1,0x48,0xdc0);
  if (plVar6 == (long *)0x0) {
    uVar11 = 0xfffffff4;
    goto LAB_001011c4;
  }
  lVar7 = dev_get_regmap(*(undefined8 *)(lVar4 + 0x70),0);
  plVar6[1] = lVar7;
  if (lVar7 == 0) {
    _dev_err(lVar1,"failed to get efuse regmap\n");
    uVar11 = 0xffffffed;
    goto LAB_001011c4;
  }
  uVar2 = of_property_read_variable_u32_array(uVar10,&DAT_00100252,plVar6 + 7,1,0);
  if ((int)uVar2 < 0) {
    uVar11 = (ulong)uVar2;
    pcVar9 = "failed to get efuse base address\n";
  }
  else {
    uVar8 = of_hwspin_lock_get_id(uVar10,0);
    uVar11 = uVar8 & 0xffffffff;
    if (-1 < (int)uVar8) {
      lVar7 = hwspin_lock_request_specific(uVar11);
      plVar6[2] = lVar7;
      if (lVar7 == 0) {
        _dev_err(lVar1,"failed to request hwspinlock\n");
        uVar11 = 0xfffffffa;
        goto LAB_001011c4;
      }
      __mutex_init(plVar6 + 3,"&efuse->mutex",0x100000);
      *plVar6 = lVar1;
      plVar6[8] = lVar5;
      *(long **)(lVar4 + 0xa8) = plVar6;
      uStack_7c = 1;
      uStack_78 = 1;
      uStack_98 = CONCAT71(uStack_98._1_7_,1);
      pcStack_c0 = "sc27xx-efuse";
      iStack_80 = *(int *)(lVar5 + 4) << 1;
      iVar3 = of_device_is_compatible(*(undefined8 *)(lVar4 + 0x310),"sprd,ump9620-efuse");
      if ((iVar3 == 0) &&
         (iVar3 = of_device_is_compatible(*(undefined8 *)(*plVar6 + 0x300),"sprd,ump9621-efuse"),
         iVar3 == 0)) {
        iVar3 = of_device_is_compatible(*(undefined8 *)(*plVar6 + 0x300),"sprd,ump518-efuse");
        if (iVar3 != 0) goto LAB_00101190;
        puStack_90 = &LAB_001020e0;
      }
      else {
        iVar3 = of_alias_get_id(uVar10,"pmic_efuse");
        uStack_b8 = CONCAT44(uStack_b8._4_4_,iVar3);
        if (iVar3 < 0) {
          _dev_err(lVar1,"failed to get pmic_efuse device id, econfig.id:%d\n",iVar3);
          uVar11 = 0xffffffea;
          goto LAB_001011c4;
        }
LAB_00101190:
        puStack_90 = &LAB_001020dc;
      }
      lStack_c8 = lVar1;
      plStack_70 = plVar6;
      uVar11 = devm_nvmem_register(lVar1,&lStack_c8);
      if (uVar11 < 0xfffffffffffff001) {
        uVar11 = 0;
      }
      else {
        _dev_err(lVar1,"failed to register nvmem config\n");
        hwspin_lock_free(plVar6[2]);
      }
      goto LAB_001011c4;
    }
    pcVar9 = "failed to get hwspinlock id\n";
  }
  _dev_err(lVar1,pcVar9);
LAB_001011c4:
  if (___stack_chk_guard == lStack_58) {
    return (byte *)(uVar11 & 0xffffffff);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



