====================__cfi_check_fail@00101000====================
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



====================sc27xx_efuse_probe@00101030====================
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



====================sc27xx_efuse_remove@001012cc====================
// signature: undefined sc27xx_efuse_remove(void)

undefined8 sc27xx_efuse_remove(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  hwspin_lock_free(*(undefined8 *)(*(long *)(param_1 + 0xa8) + 0x10));
  return 0;
}



====================ump962x_efuse_read@001012f4====================
// signature: undefined ump962x_efuse_read(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int ump962x_efuse_read(long *param_1,ulong param_2,void *param_3,ulong param_4)

{
  long *plVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_5c;
  long local_58;
  
  *in_x18 = unaff_x30;
  iVar3 = -0x16;
  local_58 = ___stack_chk_guard;
  local_5c = 0;
  if (param_4 < 3) {
    uVar2 = (uint)(param_2 >> 1);
    if ((uVar2 & 0x7fffffff) < *(uint *)(param_1[8] + 4)) {
      plVar1 = param_1 + 3;
      mutex_lock(plVar1);
      iVar3 = __hwspin_lock_timeout(param_1[2],5000,3,0);
      if (iVar3 == 0) {
        iVar3 = regmap_update_bits_base(param_1[1],*(undefined4 *)param_1[8],0x40,0x40,0,0,0);
        if (iVar3 == 0) {
          iVar3 = of_device_is_compatible(*(undefined8 *)(*param_1 + 0x300),"sprd,ump518-efuse");
          if (iVar3 == 0) {
            of_device_is_compatible(*(undefined8 *)(*param_1 + 0x300),"sprd,ump9620-efuse");
            uVar4 = 0x2010;
          }
          else {
            uVar4 = 0x1810;
          }
          iVar3 = regmap_update_bits_base(param_1[1],uVar4,0x800,0x800,0,0,0);
          if (iVar3 == 0) {
            iVar3 = regmap_update_bits_base(param_1[1],(int)param_1[7],8,0,0,0,0);
            if (iVar3 == 0) {
              regmap_update_bits_base(param_1[1],(int)param_1[7] + 0x10,4,4,0,0,0);
              iVar3 = regmap_read(param_1[1],(int)param_1[7] + uVar2 * 4 + 0x40,&local_5c);
              regmap_update_bits_base(param_1[1],*(undefined4 *)param_1[8],0x40,0,0,0,0);
            }
          }
        }
        __hwspin_unlock(param_1[2],3,0);
        mutex_unlock(plVar1);
        if (iVar3 == 0) {
          local_5c = local_5c >> (ulong)(((uint)param_2 & 1) << 3);
          memcpy(param_3,&local_5c,param_4);
        }
      }
      else {
        _dev_err(*param_1,"timeout to get the hwspinlock\n");
        mutex_unlock(plVar1);
      }
    }
  }
  if (___stack_chk_guard == local_58) {
    return iVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================sc27xx_efuse_read@00101528====================
// signature: undefined sc27xx_efuse_read(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sc27xx_efuse_read(undefined8 *param_1,ulong param_2,void *param_3,ulong param_4)

{
  undefined8 *puVar1;
  int iVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  iVar2 = -0x16;
  local_48 = ___stack_chk_guard;
  local_4c = 0;
  if (param_4 < 3) {
    if (((uint)(param_2 >> 1) & 0x7fffffff) <= *(uint *)(param_1[8] + 4)) {
      puVar1 = param_1 + 3;
      mutex_lock(puVar1);
      iVar2 = __hwspin_lock_timeout(param_1[2],5000,3,0);
      if (iVar2 == 0) {
        iVar2 = regmap_update_bits_base(param_1[1],*(undefined4 *)param_1[8],0x40,0x40,0,0,0);
        if (iVar2 == 0) {
          iVar2 = sc27xx_efuse_poll_status(param_1,4);
          if (iVar2 == 0) {
            iVar2 = regmap_write(param_1[1],*(int *)(param_1 + 7) + 0xc,(uint)param_2 >> 1 & 0x1f);
            if (iVar2 == 0) {
              iVar2 = regmap_update_bits_base(param_1[1],*(int *)(param_1 + 7) + 0x10,2,2,0,0,0);
              if (iVar2 == 0) {
                iVar2 = sc27xx_efuse_poll_status(param_1,0x10);
                if (iVar2 == 0) {
                  iVar2 = regmap_read(param_1[1],*(int *)(param_1 + 7) + 4,&local_4c);
                  if (iVar2 == 0) {
                    iVar2 = regmap_update_bits_base
                                      (param_1[1],*(int *)(param_1 + 7) + 0x10,4,4,0,0,0);
                  }
                }
              }
            }
          }
          regmap_update_bits_base(param_1[1],*(undefined4 *)param_1[8],0x40,0,0,0,0);
        }
        __hwspin_unlock(param_1[2],3,0);
        mutex_unlock(puVar1);
        if (iVar2 == 0) {
          local_4c = local_4c >> (ulong)(((uint)param_2 & 1) << 3);
          memcpy(param_3,&local_4c,param_4);
        }
      }
      else {
        _dev_err(*param_1,"timeout to get the hwspinlock\n");
        mutex_unlock(puVar1);
      }
    }
  }
  if (___stack_chk_guard == local_48) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================sc27xx_efuse_poll_status@00101740====================
// signature: undefined sc27xx_efuse_poll_status(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sc27xx_efuse_poll_status(undefined8 *param_1,uint param_2)

{
  int iVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  lVar2 = ktime_get();
  iVar1 = regmap_read(param_1[1],*(int *)(param_1 + 7) + 0x14,&local_3c);
  if (iVar1 == 0) {
    do {
      if ((local_3c & param_2) != 0) {
LAB_00101800:
        iVar1 = 0;
        goto LAB_00101804;
      }
      lVar3 = ktime_get();
      if (lVar2 + 3000000000 < lVar3) {
        iVar1 = regmap_read(param_1[1],*(int *)(param_1 + 7) + 0x14,&local_3c);
        if (iVar1 == 0) {
          if ((local_3c & param_2) != 0) goto LAB_00101800;
          iVar1 = -0x6e;
        }
        break;
      }
      usleep_range(0x9c5,10000);
      iVar1 = regmap_read(param_1[1],*(int *)(param_1 + 7) + 0x14,&local_3c);
    } while (iVar1 == 0);
  }
  _dev_err(*param_1,"timeout to update the efuse status\n");
LAB_00101804:
  if (___stack_chk_guard != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar1;
}



====================__cfi_check@00102000====================
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



====================init_module@001020e8====================
// signature: undefined init_module(void)

void init_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  __platform_driver_register(&sc27xx_efuse_driver,&__this_module);
  return;
}



====================cleanup_module@00102114====================
// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&sc27xx_efuse_driver);
  return;
}



====================__platform_driver_register@00103000====================
// signature: undefined __platform_driver_register(void)

/* WARNING: Control flow encountered bad instruction data */

void __platform_driver_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_device_get_match_data@00103008====================
// signature: undefined of_device_get_match_data(void)

/* WARNING: Control flow encountered bad instruction data */

void of_device_get_match_data(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_dev_err@00103010====================
// signature: undefined _dev_err(void)

/* WARNING: Control flow encountered bad instruction data */

void _dev_err(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_get_regmap@00103018====================
// signature: undefined dev_get_regmap(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_get_regmap(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_hwspin_lock_get_id@00103020====================
// signature: undefined of_hwspin_lock_get_id(void)

/* WARNING: Control flow encountered bad instruction data */

void of_hwspin_lock_get_id(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================hwspin_lock_request_specific@00103028====================
// signature: undefined hwspin_lock_request_specific(void)

/* WARNING: Control flow encountered bad instruction data */

void hwspin_lock_request_specific(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__mutex_init@00103030====================
// signature: undefined __mutex_init(void)

/* WARNING: Control flow encountered bad instruction data */

void __mutex_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_device_is_compatible@00103038====================
// signature: undefined of_device_is_compatible(void)

/* WARNING: Control flow encountered bad instruction data */

void of_device_is_compatible(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_alias_get_id@00103040====================
// signature: undefined of_alias_get_id(void)

/* WARNING: Control flow encountered bad instruction data */

void of_alias_get_id(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_nvmem_register@00103048====================
// signature: undefined devm_nvmem_register(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_nvmem_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================hwspin_lock_free@00103050====================
// signature: undefined hwspin_lock_free(void)

/* WARNING: Control flow encountered bad instruction data */

void hwspin_lock_free(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_kmalloc@00103058====================
// signature: undefined devm_kmalloc(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_kmalloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_property_read_variable_u32_array@00103060====================
// signature: undefined of_property_read_variable_u32_array(void)

/* WARNING: Control flow encountered bad instruction data */

void of_property_read_variable_u32_array(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_update_bits_base@00103068====================
// signature: undefined regmap_update_bits_base(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_update_bits_base(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_read@00103070====================
// signature: undefined regmap_read(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_lock@00103078====================
// signature: undefined mutex_lock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_lock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_unlock@00103080====================
// signature: undefined mutex_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__hwspin_lock_timeout@00103088====================
// signature: undefined __hwspin_lock_timeout(void)

/* WARNING: Control flow encountered bad instruction data */

void __hwspin_lock_timeout(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__hwspin_unlock@00103090====================
// signature: undefined __hwspin_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void __hwspin_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_write@00103098====================
// signature: undefined regmap_write(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_write(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================ktime_get@001030a0====================
// signature: undefined ktime_get(void)

/* WARNING: Control flow encountered bad instruction data */

void ktime_get(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================usleep_range@001030a8====================
// signature: undefined usleep_range(void)

/* WARNING: Control flow encountered bad instruction data */

void usleep_range(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================platform_driver_unregister@001030b0====================
// signature: undefined platform_driver_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void platform_driver_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__stack_chk_fail@001030b8====================
// signature: noreturn undefined __stack_chk_fail(void)

/* WARNING: Control flow encountered bad instruction data */

void __stack_chk_fail(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__ubsan_handle_cfi_check_fail_abort@001030c8====================
// signature: undefined __ubsan_handle_cfi_check_fail_abort(void)

/* WARNING: Control flow encountered bad instruction data */

void __ubsan_handle_cfi_check_fail_abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================memcpy@001030d0====================
// signature: void * memcpy(void * __dest, void * __src, size_t __n)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



