// signature: undefined sc27xx_efuse_probe(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong sc27xx_efuse_probe(long param_1)

{
  long lVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  ulong uVar7;
  char *pcVar8;
  undefined8 *in_x18;
  undefined8 uVar9;
  ulong uVar10;
  undefined8 unaff_x30;
  long local_b8;
  char *local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined1 *local_80;
  undefined8 local_78;
  int local_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  long *local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_48 = ___stack_chk_guard;
  uVar9 = *(undefined8 *)(param_1 + 0x310);
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_64 = 0;
  local_60 = (long *)0x0;
  local_78 = 0;
  local_70 = 0;
  uStack_6c = 0;
  local_88 = 0;
  local_80 = (undefined1 *)0x0;
  local_98 = 0;
  uStack_90 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_b8 = 0;
  local_b0 = (char *)0x0;
  lVar4 = of_device_get_match_data(lVar1);
  if (lVar4 == 0) {
    _dev_err(lVar1,"No matching driver data found\n");
    uVar10 = 0xffffffea;
    goto LAB_001011c4;
  }
  plVar5 = (long *)devm_kmalloc(lVar1,0x48,0xdc0);
  if (plVar5 == (long *)0x0) {
    uVar10 = 0xfffffff4;
    goto LAB_001011c4;
  }
  lVar6 = dev_get_regmap(*(undefined8 *)(param_1 + 0x70),0);
  plVar5[1] = lVar6;
  if (lVar6 == 0) {
    _dev_err(lVar1,"failed to get efuse regmap\n");
    uVar10 = 0xffffffed;
    goto LAB_001011c4;
  }
  uVar2 = of_property_read_variable_u32_array(uVar9,&DAT_00100252,plVar5 + 7,1,0);
  if ((int)uVar2 < 0) {
    uVar10 = (ulong)uVar2;
    pcVar8 = "failed to get efuse base address\n";
  }
  else {
    uVar7 = of_hwspin_lock_get_id(uVar9,0);
    uVar10 = uVar7 & 0xffffffff;
    if (-1 < (int)uVar7) {
      lVar6 = hwspin_lock_request_specific(uVar10);
      plVar5[2] = lVar6;
      if (lVar6 == 0) {
        _dev_err(lVar1,"failed to request hwspinlock\n");
        uVar10 = 0xfffffffa;
        goto LAB_001011c4;
      }
      __mutex_init(plVar5 + 3,"&efuse->mutex",0x100000);
      *plVar5 = lVar1;
      plVar5[8] = lVar4;
      *(long **)(param_1 + 0xa8) = plVar5;
      uStack_6c = 1;
      local_68 = 1;
      local_88 = CONCAT71(local_88._1_7_,1);
      local_b0 = "sc27xx-efuse";
      local_70 = *(int *)(lVar4 + 4) << 1;
      iVar3 = of_device_is_compatible(*(undefined8 *)(param_1 + 0x310),"sprd,ump9620-efuse");
      if ((iVar3 == 0) &&
         (iVar3 = of_device_is_compatible(*(undefined8 *)(*plVar5 + 0x300),"sprd,ump9621-efuse"),
         iVar3 == 0)) {
        iVar3 = of_device_is_compatible(*(undefined8 *)(*plVar5 + 0x300),"sprd,ump518-efuse");
        if (iVar3 != 0) goto LAB_00101190;
        local_80 = &LAB_001020e0;
      }
      else {
        iVar3 = of_alias_get_id(uVar9,"pmic_efuse");
        local_a8 = CONCAT44(local_a8._4_4_,iVar3);
        if (iVar3 < 0) {
          _dev_err(lVar1,"failed to get pmic_efuse device id, econfig.id:%d\n",iVar3);
          uVar10 = 0xffffffea;
          goto LAB_001011c4;
        }
LAB_00101190:
        local_80 = &LAB_001020dc;
      }
      local_b8 = lVar1;
      local_60 = plVar5;
      uVar10 = devm_nvmem_register(lVar1,&local_b8);
      if (uVar10 < 0xfffffffffffff001) {
        uVar10 = 0;
      }
      else {
        _dev_err(lVar1,"failed to register nvmem config\n");
        hwspin_lock_free(plVar5[2]);
      }
      goto LAB_001011c4;
    }
    pcVar8 = "failed to get hwspinlock id\n";
  }
  _dev_err(lVar1,pcVar8);
LAB_001011c4:
  if (___stack_chk_guard == local_48) {
    return uVar10 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



