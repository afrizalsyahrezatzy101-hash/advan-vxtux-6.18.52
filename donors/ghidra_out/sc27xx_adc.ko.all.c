====================__cfi_check_fail@00103000====================
// signature: undefined __cfi_check_fail(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte * __cfi_check_fail(byte *param_1)

{
  long *plVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  int *piVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  long lVar9;
  char *pcVar10;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  ulong uVar11;
  code *pcVar12;
  undefined8 uVar13;
  undefined8 unaff_x30;
  undefined8 uStack_70;
  long lStack_68;
  undefined1 *puStack_60;
  code *pcStack_58;
  
  *in_x18 = unaff_x30;
  if ((param_1 != (byte *)0x0) && (4 < *param_1)) {
    return param_1;
  }
  lVar4 = __ubsan_handle_cfi_check_fail_abort(2);
  *extraout_x18 = sc27xx_adc_probe;
  pcStack_58 = sc27xx_adc_probe;
  plVar1 = (long *)(lVar4 + 0x10);
  lStack_68 = ___stack_chk_guard;
  uVar13 = *(undefined8 *)(lVar4 + 0x310);
  puStack_60 = &stack0xfffffffffffffff0;
  piVar5 = (int *)of_device_get_match_data(plVar1);
  if (piVar5 == (int *)0x0) {
    _dev_err(plVar1,"No matching driver data found\n");
    uVar11 = 0xffffffea;
    goto LAB_001033e8;
  }
  lVar6 = devm_iio_device_alloc(plVar1,0xd0);
  if (lVar6 == 0) {
    uVar11 = 0xfffffff4;
    goto LAB_001033e8;
  }
  lVar7 = dev_get_regmap(*(undefined8 *)(lVar4 + 0x70),0);
  *(long *)(lVar6 + 0x5d8) = lVar7;
  if (lVar7 == 0) {
    _dev_err(plVar1,"failed to get ADC regmap\n");
    uVar11 = 0xffffffed;
    goto LAB_001033e8;
  }
  lVar7 = lVar6 + 0x5c0;
  uVar2 = of_property_read_variable_u32_array(uVar13,&DAT_00100419,lVar6 + 0x668,1,0);
  if ((int)uVar2 < 0) {
    uVar11 = (ulong)uVar2;
    pcVar10 = "failed to get ADC base address\n";
  }
  else {
    iVar3 = platform_get_irq(lVar4,0);
    *(int *)(lVar6 + 0x66c) = iVar3;
    if (iVar3 < 0) {
      _dev_err(plVar1,"failed to get ADC irq number\n");
      uVar11 = (ulong)*(uint *)(lVar6 + 0x66c);
      goto LAB_001033e8;
    }
    uVar8 = of_hwspin_lock_get_id(uVar13,0);
    uVar11 = uVar8 & 0xffffffff;
    if ((int)uVar8 < 0) {
      pcVar10 = "failed to get hwspinlock id\n";
    }
    else {
      lVar9 = hwspin_lock_request_specific(uVar11);
      *(long *)(lVar6 + 0x5e0) = lVar9;
      if (lVar9 == 0) {
        _dev_err(plVar1,"failed to request hwspinlock\n");
        uVar11 = 0xfffffffa;
        goto LAB_001033e8;
      }
      uVar2 = devm_add_action(plVar1,&LAB_0010520c,lVar9);
      if (uVar2 == 0) {
        if (*piVar5 == 1) {
          uVar11 = devm_regulator_get_optional(plVar1,&DAT_001003d9);
          *(ulong *)(lVar6 + 0x5d0) = uVar11;
          if ((uVar11 == 0) || (0xfffffffffffff000 < uVar11)) {
            _dev_err(plVar1,"err! ADC volref, err: %d\n",uVar11 & 0xffffffff);
            goto LAB_001033e8;
          }
        }
        *(long **)(lVar6 + 0x5c8) = plVar1;
        *(int **)(lVar6 + 0x670) = piVar5;
        *(long *)(lVar6 + 0x5c0) = lVar6;
        pcVar12 = *(code **)(piVar5 + 10);
        if (3 < ((ulong)(pcVar12 + -0x1051e8) >> 2 | (long)pcVar12 << 0x3e)) {
          __cfi_slowpath(0xec843035baddf00b,pcVar12);
        }
        (*pcVar12)(lVar7);
        uVar2 = regmap_update_bits_base
                          (*(undefined8 *)(lVar6 + 0x5d8),
                           *(undefined4 *)(*(long *)(lVar6 + 0x670) + 4),0x20,0x20,0,0,0);
        if (uVar2 == 0) {
          uVar2 = regmap_update_bits_base
                            (*(undefined8 *)(lVar6 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar6 + 0x670) + 8),0x60,0x60,0,0,0);
          if (uVar2 == 0) {
            if ((**(uint **)(lVar6 + 0x670) & 0xfffffffe) == 2) {
              uVar11 = ump96xx_adc_scale_cal(lVar7,0);
              if (((int)uVar11 == 0) && (uVar11 = ump96xx_adc_scale_cal(lVar7,1), (int)uVar11 == 0))
              {
                uVar11 = ump96xx_adc_scale_cal(lVar7,2);
                iVar3 = (int)uVar11;
joined_r0x00103238:
                if (iVar3 == 0) {
                  uVar2 = devm_add_action(plVar1,&LAB_00105210,lVar7);
                  if (uVar2 == 0) {
                    uStack_70 = 0;
                    uVar13 = *(undefined8 *)(*(long *)(lVar6 + 0x5c8) + 0x300);
                    *(undefined1 *)(lVar6 + 0x688) = 0;
                    uVar11 = syscon_regmap_lookup_by_phandle_args
                                       (uVar13,"sprd_adc_pm_reg",2,&uStack_70);
                    *(ulong *)(lVar6 + 0x678) = uVar11;
                    if ((uVar11 != 0) && (uVar11 < 0xfffffffffffff001)) {
                      *(undefined1 *)(lVar6 + 0x688) = 1;
                      *(undefined4 *)(lVar6 + 0x680) = (undefined4)uStack_70;
                      *(undefined4 *)(lVar6 + 0x684) = uStack_70._4_4_;
                      _dev_info(*(undefined8 *)(lVar6 + 0x5c8),
                                "sprd_adc_rpm_reg reg 0x%x, mask 0x%x\n");
                      iVar3 = regmap_update_bits_base
                                        (*(undefined8 *)(lVar6 + 0x678),
                                         *(undefined4 *)(lVar6 + 0x680),
                                         *(undefined4 *)(lVar6 + 0x684),
                                         *(undefined4 *)(lVar6 + 0x684),0,0,0);
                      if (iVar3 != 0) {
                        _dev_err(*(undefined8 *)(lVar6 + 0x5c8),
                                 "failed to set the ADC clk26m bit8 on IP\n");
                        _dev_err(plVar1,"adc pm init err.\n");
                        uVar11 = 0xfffffff0;
                        goto LAB_001033e8;
                      }
                      *(undefined1 *)(lVar6 + 0x689) = 0;
                    }
                    *(long **)(lVar6 + 0x78) = plVar1;
                    lVar7 = *(long *)(lVar4 + 0x80);
                    if (lVar7 == 0) {
                      lVar7 = *plVar1;
                    }
                    *(long *)(lVar6 + 0x490) = lVar7;
                    *(undefined4 *)(lVar6 + 0x10) = 1;
                    *(undefined1 **)(lVar6 + 0x498) = &sc27xx_info;
                    *(undefined1 **)(lVar6 + 0x448) = &sc27xx_channels;
                    *(undefined4 *)(lVar6 + 0x450) = 0x20;
                    uVar8 = __devm_iio_device_register(plVar1,lVar6,&__this_module);
                    uVar11 = uVar8 & 0xffffffff;
                    if ((int)uVar8 != 0) {
                      _dev_err(plVar1,"could not register iio (ADC)");
                    }
                    *(long *)(lVar4 + 0xa8) = lVar6;
                    goto LAB_001033e8;
                  }
                  uVar11 = (ulong)uVar2;
                  regmap_update_bits_base
                            (*(undefined8 *)(lVar6 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar6 + 0x670) + 8),0x60,0,0,0,0);
                  regmap_update_bits_base
                            (*(undefined8 *)(lVar6 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar6 + 0x670) + 4),0x20,0,0,0,0);
                  pcVar10 = "failed to add ADC disable action\n";
                  goto LAB_001033e0;
                }
              }
            }
            else {
              uVar11 = sc27xx_adc_scale_calibration
                                 (*(undefined8 *)(lVar6 + 0x5c8),*(uint **)(lVar6 + 0x670),1);
              if ((int)uVar11 == 0) {
                uVar11 = sc27xx_adc_scale_calibration
                                   (*(undefined8 *)(lVar6 + 0x5c8),*(undefined8 *)(lVar6 + 0x670),0)
                ;
                iVar3 = (int)uVar11;
                goto joined_r0x00103238;
              }
            }
            uVar11 = uVar11 & 0xffffffff;
            regmap_update_bits_base
                      (*(undefined8 *)(lVar6 + 0x5d8),*(undefined4 *)(*(long *)(lVar6 + 0x670) + 8),
                       0x60,0,0,0,0);
          }
          else {
            uVar11 = (ulong)uVar2;
          }
          regmap_update_bits_base
                    (*(undefined8 *)(lVar6 + 0x5d8),*(undefined4 *)(*(long *)(lVar6 + 0x670) + 4),
                     0x20,0,0,0,0);
        }
        else {
          uVar11 = (ulong)uVar2;
        }
        pcVar10 = "failed to enable ADC module\n";
      }
      else {
        uVar11 = (ulong)uVar2;
        hwspin_lock_free(*(undefined8 *)(lVar6 + 0x5e0));
        pcVar10 = "failed to add hwspinlock action\n";
      }
    }
  }
LAB_001033e0:
  _dev_err(plVar1,pcVar10);
LAB_001033e8:
  if (___stack_chk_guard == lStack_68) {
    return (byte *)(uVar11 & 0xffffffff);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================sc27xx_adc_probe@00103030====================
// signature: undefined sc27xx_adc_probe(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong sc27xx_adc_probe(long param_1)

{
  long *plVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  char *pcVar9;
  undefined8 *in_x18;
  ulong uVar10;
  code *pcVar11;
  undefined8 uVar12;
  undefined8 unaff_x30;
  undefined8 local_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  plVar1 = (long *)(param_1 + 0x10);
  local_58 = ___stack_chk_guard;
  uVar12 = *(undefined8 *)(param_1 + 0x310);
  piVar4 = (int *)of_device_get_match_data(plVar1);
  if (piVar4 == (int *)0x0) {
    _dev_err(plVar1,"No matching driver data found\n");
    uVar10 = 0xffffffea;
    goto LAB_001033e8;
  }
  lVar5 = devm_iio_device_alloc(plVar1,0xd0);
  if (lVar5 == 0) {
    uVar10 = 0xfffffff4;
    goto LAB_001033e8;
  }
  lVar6 = dev_get_regmap(*(undefined8 *)(param_1 + 0x70),0);
  *(long *)(lVar5 + 0x5d8) = lVar6;
  if (lVar6 == 0) {
    _dev_err(plVar1,"failed to get ADC regmap\n");
    uVar10 = 0xffffffed;
    goto LAB_001033e8;
  }
  lVar6 = lVar5 + 0x5c0;
  uVar2 = of_property_read_variable_u32_array(uVar12,&DAT_00100419,lVar5 + 0x668,1,0);
  if ((int)uVar2 < 0) {
    uVar10 = (ulong)uVar2;
    pcVar9 = "failed to get ADC base address\n";
  }
  else {
    iVar3 = platform_get_irq(param_1,0);
    *(int *)(lVar5 + 0x66c) = iVar3;
    if (iVar3 < 0) {
      _dev_err(plVar1,"failed to get ADC irq number\n");
      uVar10 = (ulong)*(uint *)(lVar5 + 0x66c);
      goto LAB_001033e8;
    }
    uVar7 = of_hwspin_lock_get_id(uVar12,0);
    uVar10 = uVar7 & 0xffffffff;
    if ((int)uVar7 < 0) {
      pcVar9 = "failed to get hwspinlock id\n";
    }
    else {
      lVar8 = hwspin_lock_request_specific(uVar10);
      *(long *)(lVar5 + 0x5e0) = lVar8;
      if (lVar8 == 0) {
        _dev_err(plVar1,"failed to request hwspinlock\n");
        uVar10 = 0xfffffffa;
        goto LAB_001033e8;
      }
      uVar2 = devm_add_action(plVar1,&LAB_0010520c,lVar8);
      if (uVar2 == 0) {
        if (*piVar4 == 1) {
          uVar10 = devm_regulator_get_optional(plVar1,&DAT_001003d9);
          *(ulong *)(lVar5 + 0x5d0) = uVar10;
          if ((uVar10 == 0) || (0xfffffffffffff000 < uVar10)) {
            _dev_err(plVar1,"err! ADC volref, err: %d\n",uVar10 & 0xffffffff);
            goto LAB_001033e8;
          }
        }
        *(long **)(lVar5 + 0x5c8) = plVar1;
        *(int **)(lVar5 + 0x670) = piVar4;
        *(long *)(lVar5 + 0x5c0) = lVar5;
        pcVar11 = *(code **)(piVar4 + 10);
        if (3 < ((ulong)(pcVar11 + -0x1051e8) >> 2 | (long)pcVar11 << 0x3e)) {
          __cfi_slowpath(0xec843035baddf00b,pcVar11);
        }
        (*pcVar11)(lVar6);
        uVar2 = regmap_update_bits_base
                          (*(undefined8 *)(lVar5 + 0x5d8),
                           *(undefined4 *)(*(long *)(lVar5 + 0x670) + 4),0x20,0x20,0,0,0);
        if (uVar2 == 0) {
          uVar2 = regmap_update_bits_base
                            (*(undefined8 *)(lVar5 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar5 + 0x670) + 8),0x60,0x60,0,0,0);
          if (uVar2 == 0) {
            if ((**(uint **)(lVar5 + 0x670) & 0xfffffffe) == 2) {
              uVar10 = ump96xx_adc_scale_cal(lVar6,0);
              if (((int)uVar10 == 0) && (uVar10 = ump96xx_adc_scale_cal(lVar6,1), (int)uVar10 == 0))
              {
                uVar10 = ump96xx_adc_scale_cal(lVar6,2);
                iVar3 = (int)uVar10;
joined_r0x00103238:
                if (iVar3 == 0) {
                  uVar2 = devm_add_action(plVar1,&LAB_00105210,lVar6);
                  if (uVar2 == 0) {
                    local_60 = 0;
                    uVar12 = *(undefined8 *)(*(long *)(lVar5 + 0x5c8) + 0x300);
                    *(undefined1 *)(lVar5 + 0x688) = 0;
                    uVar10 = syscon_regmap_lookup_by_phandle_args
                                       (uVar12,"sprd_adc_pm_reg",2,&local_60);
                    *(ulong *)(lVar5 + 0x678) = uVar10;
                    if ((uVar10 != 0) && (uVar10 < 0xfffffffffffff001)) {
                      *(undefined1 *)(lVar5 + 0x688) = 1;
                      *(undefined4 *)(lVar5 + 0x680) = (undefined4)local_60;
                      *(undefined4 *)(lVar5 + 0x684) = local_60._4_4_;
                      _dev_info(*(undefined8 *)(lVar5 + 0x5c8),
                                "sprd_adc_rpm_reg reg 0x%x, mask 0x%x\n");
                      iVar3 = regmap_update_bits_base
                                        (*(undefined8 *)(lVar5 + 0x678),
                                         *(undefined4 *)(lVar5 + 0x680),
                                         *(undefined4 *)(lVar5 + 0x684),
                                         *(undefined4 *)(lVar5 + 0x684),0,0,0);
                      if (iVar3 != 0) {
                        _dev_err(*(undefined8 *)(lVar5 + 0x5c8),
                                 "failed to set the ADC clk26m bit8 on IP\n");
                        _dev_err(plVar1,"adc pm init err.\n");
                        uVar10 = 0xfffffff0;
                        goto LAB_001033e8;
                      }
                      *(undefined1 *)(lVar5 + 0x689) = 0;
                    }
                    *(long **)(lVar5 + 0x78) = plVar1;
                    lVar6 = *(long *)(param_1 + 0x80);
                    if (lVar6 == 0) {
                      lVar6 = *plVar1;
                    }
                    *(long *)(lVar5 + 0x490) = lVar6;
                    *(undefined4 *)(lVar5 + 0x10) = 1;
                    *(undefined1 **)(lVar5 + 0x498) = &sc27xx_info;
                    *(undefined1 **)(lVar5 + 0x448) = &sc27xx_channels;
                    *(undefined4 *)(lVar5 + 0x450) = 0x20;
                    uVar7 = __devm_iio_device_register(plVar1,lVar5,&__this_module);
                    uVar10 = uVar7 & 0xffffffff;
                    if ((int)uVar7 != 0) {
                      _dev_err(plVar1,"could not register iio (ADC)");
                    }
                    *(long *)(param_1 + 0xa8) = lVar5;
                    goto LAB_001033e8;
                  }
                  uVar10 = (ulong)uVar2;
                  regmap_update_bits_base
                            (*(undefined8 *)(lVar5 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar5 + 0x670) + 8),0x60,0,0,0,0);
                  regmap_update_bits_base
                            (*(undefined8 *)(lVar5 + 0x5d8),
                             *(undefined4 *)(*(long *)(lVar5 + 0x670) + 4),0x20,0,0,0,0);
                  pcVar9 = "failed to add ADC disable action\n";
                  goto LAB_001033e0;
                }
              }
            }
            else {
              uVar10 = sc27xx_adc_scale_calibration
                                 (*(undefined8 *)(lVar5 + 0x5c8),*(uint **)(lVar5 + 0x670),1);
              if ((int)uVar10 == 0) {
                uVar10 = sc27xx_adc_scale_calibration
                                   (*(undefined8 *)(lVar5 + 0x5c8),*(undefined8 *)(lVar5 + 0x670),0)
                ;
                iVar3 = (int)uVar10;
                goto joined_r0x00103238;
              }
            }
            uVar10 = uVar10 & 0xffffffff;
            regmap_update_bits_base
                      (*(undefined8 *)(lVar5 + 0x5d8),*(undefined4 *)(*(long *)(lVar5 + 0x670) + 8),
                       0x60,0,0,0,0);
          }
          else {
            uVar10 = (ulong)uVar2;
          }
          regmap_update_bits_base
                    (*(undefined8 *)(lVar5 + 0x5d8),*(undefined4 *)(*(long *)(lVar5 + 0x670) + 4),
                     0x20,0,0,0,0);
        }
        else {
          uVar10 = (ulong)uVar2;
        }
        pcVar9 = "failed to enable ADC module\n";
      }
      else {
        uVar10 = (ulong)uVar2;
        hwspin_lock_free(*(undefined8 *)(lVar5 + 0x5e0));
        pcVar9 = "failed to add hwspinlock action\n";
      }
    }
  }
LAB_001033e0:
  _dev_err(plVar1,pcVar9);
LAB_001033e8:
  if (___stack_chk_guard == local_58) {
    return uVar10 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================sc27xx_adc_remove@00103534====================
// signature: undefined sc27xx_adc_remove(void)

undefined8 sc27xx_adc_remove(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0xa8);
  if ((*(char *)(lVar2 + 0x688) != '\0') &&
     (iVar1 = regmap_update_bits_base
                        (*(undefined8 *)(lVar2 + 0x678),*(undefined4 *)(lVar2 + 0x680),
                         *(undefined4 *)(lVar2 + 0x684),0,0,0,0), iVar1 != 0)) {
    _dev_err(*(undefined8 *)(lVar2 + 0x5c8),"clean clk26m_sinout_pmic failed\n");
  }
  return 0;
}



====================sc27xx_adc_pm_suspend@0010359c====================
// signature: undefined sc27xx_adc_pm_suspend(void)

undefined8 sc27xx_adc_pm_suspend(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x98);
  if (*(char *)(lVar2 + 0x688) != '\0') {
    mutex_lock(*(long *)(lVar2 + 0x5c0) + 1000);
    iVar1 = regmap_update_bits_base
                      (*(undefined8 *)(lVar2 + 0x678),*(undefined4 *)(lVar2 + 0x680),
                       *(undefined4 *)(lVar2 + 0x684),0,0,0,0);
    if (iVar1 == 0) {
      *(undefined1 *)(lVar2 + 0x689) = 1;
    }
    else {
      _dev_err(*(undefined8 *)(lVar2 + 0x5c8),"clean clk26m_sinout_pmic failed\n");
    }
    mutex_unlock(*(long *)(lVar2 + 0x5c0) + 1000);
  }
  return 0;
}



====================sc27xx_adc_pm_resume@00103624====================
// signature: undefined sc27xx_adc_pm_resume(void)

undefined8 sc27xx_adc_pm_resume(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x98);
  if (*(char *)(lVar2 + 0x688) != '\0') {
    mutex_lock(*(long *)(lVar2 + 0x5c0) + 1000);
    iVar1 = regmap_update_bits_base
                      (*(undefined8 *)(lVar2 + 0x678),*(undefined4 *)(lVar2 + 0x680),
                       *(undefined4 *)(lVar2 + 0x684),*(undefined4 *)(lVar2 + 0x684),0,0,0);
    if (iVar1 == 0) {
      *(undefined1 *)(lVar2 + 0x689) = 0;
    }
    else {
      _dev_err(param_1,"failed to set the UMP9620 ADC clk26m bit8 on IP\n");
    }
    mutex_unlock(*(long *)(lVar2 + 0x5c0) + 1000);
  }
  return 0;
}



====================ump9620_adc_scale_init@001036ac====================
// signature: undefined ump9620_adc_scale_init(void)

void ump9620_adc_scale_init(long param_1)

{
  uint uVar1;
  long lVar2;
  undefined4 uVar3;
  
  lVar2 = 0;
  do {
    if ((uint)lVar2 < 0x20) {
      uVar1 = 1 << (ulong)((uint)lVar2 & 0x1f);
      if ((uVar1 & 0xc0080400) == 0) {
        if ((uVar1 & 0x280) == 0) {
          if ((uVar1 & 0x2001) == 0) goto LAB_00103714;
          uVar3 = 1;
        }
        else {
          uVar3 = 2;
        }
      }
      else {
        uVar3 = 3;
      }
    }
    else {
LAB_00103714:
      uVar3 = 0;
    }
    *(undefined4 *)(param_1 + 0x28 + lVar2 * 4) = uVar3;
    lVar2 = lVar2 + 1;
    if (lVar2 == 0x20) {
      return;
    }
  } while( true );
}



====================ump9620_adc_get_ratio@00103720====================
// signature: undefined ump9620_adc_get_ratio(void)

undefined4 ump9620_adc_get_ratio(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0x10001;
  switch(param_1) {
  case 0xb:
    goto switchD_00103744_caseD_b;
  default:
    if (param_2 - 1U < 3) {
      uVar1 = *(undefined4 *)(&DAT_00102068 + (long)(int)(param_2 - 1U) * 4);
    }
switchD_00103744_caseD_b:
    return uVar1;
  case 0xe:
    uVar1 = 0x440384;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0xf:
    uVar1 = 0x10003;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0x15:
  case 0x16:
  case 0x17:
    uVar1 = 0x30008;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  }
}



====================sc2720_adc_scale_init@0010379c====================
// signature: undefined sc2720_adc_scale_init(void)

void sc2720_adc_scale_init(long param_1)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined4 uVar4;
  
  lVar3 = 0;
  do {
    uVar2 = (uint)lVar3;
    if (uVar2 < 0x20) {
      uVar1 = 1 << (ulong)(uVar2 & 0x1f);
      if ((uVar1 & 0xc0080020) == 0) {
        if ((uVar1 & 0x280) == 0) {
          if (uVar2 != 0xd) goto LAB_00103800;
          uVar4 = 1;
        }
        else {
          uVar4 = 2;
        }
      }
      else {
        uVar4 = 3;
      }
    }
    else {
LAB_00103800:
      uVar4 = 0;
    }
    *(undefined4 *)(param_1 + 0x28 + lVar3 * 4) = uVar4;
    lVar3 = lVar3 + 1;
    if (lVar3 == 0x20) {
      return;
    }
  } while( true );
}



====================sc2720_adc_get_ratio@0010380c====================
// signature: undefined sc2720_adc_get_ratio(void)

undefined4 sc2720_adc_get_ratio(int param_1,uint param_2)

{
  if (param_1 - 0x15U < 3) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c80 + (long)(int)param_2 * 4);
    }
  }
  else if (param_1 == 0x10) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c90 + (long)(int)param_2 * 4);
    }
  }
  else if (param_1 == 0xe) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100ca0 + (long)(int)param_2 * 4);
    }
  }
  else if (param_2 - 1 < 3) {
    return *(undefined4 *)(&DAT_00102074 + (long)(int)(param_2 - 1) * 4);
  }
  return 0x10001;
}



====================sc2731_adc_scale_init@00103894====================
// signature: undefined sc2731_adc_scale_init(void)

void sc2731_adc_scale_init(long param_1)

{
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x60) = 0;
  *(undefined8 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x70) = 0;
  *(undefined8 *)(param_1 + 0x78) = 0;
  *(undefined8 *)(param_1 + 0x80) = 0;
  *(undefined8 *)(param_1 + 0x88) = 0;
  *(undefined8 *)(param_1 + 0x90) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0x100000000;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x98) = 0;
  *(undefined8 *)(param_1 + 0xa0) = 0;
  return;
}



====================sc2721_adc_get_ratio@001038bc====================
// signature: undefined sc2721_adc_get_ratio(void)

undefined4 sc2721_adc_get_ratio(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  switch(param_1) {
  case 1:
  case 2:
  case 3:
  case 4:
    uVar2 = 0x1900401;
    break;
  case 5:
    return 0x7001d;
  default:
    return 0x10001;
  case 7:
  case 9:
    uVar2 = 0x64007d;
    break;
  case 0xe:
    return 0x440384;
  case 0x10:
    return 0x300064;
  case 0x13:
    return 0x10003;
  }
  uVar1 = 0x10001;
  if (param_2 != 0) {
    uVar1 = uVar2;
  }
  return uVar1;
}



====================sc2730_adc_scale_init@0010393c====================
// signature: undefined sc2730_adc_scale_init(void)

void sc2730_adc_scale_init(long param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  
  lVar4 = 0;
  lVar1 = param_1 + 0x28;
  do {
    uVar3 = (uint)lVar4;
    if (uVar3 < 0x20) {
      uVar2 = 1 << (ulong)(uVar3 & 0x1f);
      if ((uVar2 & 0xc0080420) == 0) {
        if ((uVar2 & 0x280) == 0) {
          if (uVar3 != 0xd) goto LAB_001039a4;
          *(undefined4 *)(param_1 + 0x5c) = 1;
        }
        else {
          *(undefined4 *)(lVar1 + lVar4 * 4) = 2;
        }
      }
      else {
        *(undefined4 *)(lVar1 + lVar4 * 4) = 3;
      }
    }
    else {
LAB_001039a4:
      *(undefined4 *)(lVar1 + lVar4 * 4) = 0;
    }
    lVar4 = lVar4 + 1;
    if (lVar4 == 0x20) {
      return;
    }
  } while( true );
}



====================sc2730_adc_get_ratio@001039b0====================
// signature: undefined sc2730_adc_get_ratio(void)

undefined4 sc2730_adc_get_ratio(undefined4 param_1,uint param_2)

{
  switch(param_1) {
  case 0xe:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100ca0 + (long)(int)param_2 * 4);
    }
    break;
  case 0xf:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100cb0 + (long)(int)param_2 * 4);
    }
    break;
  case 0x10:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c90 + (long)(int)param_2 * 4);
    }
    break;
  default:
    if (param_2 - 1 < 3) {
      return *(undefined4 *)(&DAT_00102080 + (long)(int)(param_2 - 1) * 4);
    }
    break;
  case 0x15:
  case 0x16:
  case 0x17:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c80 + (long)(int)param_2 * 4);
    }
  }
  return 0x10001;
}



====================sc2731_adc_get_ratio@00103a58====================
// signature: undefined sc2731_adc_get_ratio(void)

undefined4 sc2731_adc_get_ratio(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  switch(param_1) {
  case 1:
  case 2:
  case 3:
  case 4:
    uVar2 = 0x1900401;
    break;
  case 5:
    return 0x7001d;
  case 6:
    return 0x1772328;
  case 7:
  case 8:
    uVar2 = 0x64007d;
    break;
  default:
    return 0x10001;
  case 0x13:
    return 0x10003;
  }
  uVar1 = 0x10001;
  if (param_2 != 0) {
    uVar1 = uVar2;
  }
  return uVar1;
}



====================sc27xx_adc_free_hwlock@00103acc====================
// signature: undefined sc27xx_adc_free_hwlock(void)

void sc27xx_adc_free_hwlock(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  hwspin_lock_free();
  return;
}



====================ump96xx_adc_scale_cal@00103ae8====================
// signature: undefined ump96xx_adc_scale_cal(void)

uint ump96xx_adc_scale_cal(long param_1,ulong param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == 0) {
    uVar2 = 0xffffffea;
  }
  else {
    uVar6 = -(param_2 >> 0x1f & 1) & 0xfffffff800000000 | (param_2 & 0xffffffff) << 3;
    uVar5 = *(undefined8 *)((long)&PTR_s_big_scale_calib1_001020a8 + uVar6);
    uVar2 = adc_nvmem_cell_calib_data(param_1,uVar5);
    if ((int)uVar2 < 0) {
      _dev_err(*(undefined8 *)(param_1 + 8),"err! %s:%d\n",uVar5,uVar2);
    }
    else {
      uVar5 = *(undefined8 *)((long)&PTR_s_big_scale_calib2_001020c0 + uVar6);
      uVar3 = adc_nvmem_cell_calib_data(param_1,uVar5);
      if ((int)uVar3 < 0) {
        _dev_err(*(undefined8 *)(param_1 + 8),"err! %s:%d\n",uVar5,uVar3);
        uVar2 = uVar3;
      }
      else {
        uVar1 = uVar2 >> 4 & 0xfff;
        uVar2 = 0;
        if ((uVar1 != 0) && (uVar3 = uVar3 >> 4 & 0xfff, uVar3 != 0)) {
          lVar4 = *(long *)((long)&PTR_big_scale_graph_00102090 + uVar6);
          *(uint *)(lVar4 + 4) = uVar1;
          *(uint *)(lVar4 + 0xc) = uVar3;
          uVar2 = 0;
        }
      }
    }
  }
  return uVar2;
}



====================sc27xx_adc_scale_calibration@00103bd4====================
// signature: undefined sc27xx_adc_scale_calibration(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong sc27xx_adc_scale_calibration(undefined8 param_1,long param_2,ulong param_3)

{
  char *pcVar1;
  size_t __n;
  undefined4 *puVar2;
  bool bVar3;
  ulong uVar4;
  void *__src;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  ulong local_48;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  bVar3 = (param_3 & 1) == 0;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  local_48 = 0;
  lVar5 = 0x18;
  if (bVar3) {
    lVar5 = 0x20;
  }
  lVar5 = *(long *)(param_2 + lVar5);
  pcVar1 = "big_scale_calib";
  if (bVar3) {
    pcVar1 = "small_scale_calib";
  }
  uVar4 = nvmem_cell_get(param_1,pcVar1);
  if (uVar4 < 0xfffffffffffff001) {
    __src = (void *)nvmem_cell_read(uVar4,&local_48);
    nvmem_cell_put(uVar4);
    if (__src < (void *)0xfffffffffffff001) {
      __n = local_48;
      if (3 < local_48) {
        __n = 4;
      }
      memcpy(&local_3c,__src,__n);
      puVar2 = &big_scale_graph;
      if ((param_3 & 1) == 0) {
        puVar2 = &small_scale_graph;
      }
      puVar2[1] = (*(int *)(lVar5 + 4) + (local_3c & 0xff)) * 4 + -0x200;
      puVar2[3] = (*(int *)(lVar5 + 0xc) + (local_3c >> 8 & 0xff)) * 4 + -0x200;
      kfree(__src);
      uVar4 = 0;
    }
    else {
      uVar4 = (ulong)__src & 0xffffffff;
    }
  }
  if (___stack_chk_guard == local_38) {
    return uVar4 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================sc27xx_adc_disable@00103d08====================
// signature: undefined sc27xx_adc_disable(void)

void sc27xx_adc_disable(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base
            (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(*(long *)(param_1 + 0xb0) + 8),0x60,0,0
             ,0,0);
  regmap_update_bits_base
            (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(*(long *)(param_1 + 0xb0) + 4),0x20,0,0
             ,0,0);
  return;
}



====================sc27xx_adc_read_raw@00103d74====================
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



====================sc27xx_adc_write_raw@0010406c====================
// signature: undefined sc27xx_adc_write_raw(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sc27xx_adc_write_raw(long param_1,long param_2,int param_3,undefined4 *param_4,long param_5)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  undefined8 unaff_x30;
  undefined1 auVar9 [12];
  undefined8 uStack_60;
  long lStack_58;
  undefined1 *puStack_50;
  code *pcStack_48;
  
  *in_x18 = unaff_x30;
  if (param_5 == 2) {
    if (0x1f < *(uint *)(param_2 + 4)) {
      auVar9 = __ubsan_handle_cfi_check_fail_abort(0x12);
      lVar5 = auVar9._0_8_;
      *extraout_x18 = sc27xx_adc_read;
      pcStack_48 = sc27xx_adc_read;
      lStack_58 = ___stack_chk_guard;
      uStack_60 = 0;
      if ((*(char *)(lVar5 + 200) == '\0') || (*(char *)(lVar5 + 0xc9) == '\0')) {
        puStack_50 = &stack0xfffffffffffffff0;
        iVar3 = __hwspin_lock_timeout(*(undefined8 *)(lVar5 + 0x20),5000,3,0);
        if (iVar3 == 0) {
          uVar1 = auVar9._8_4_ & 0xfffffffe;
          if (((uVar1 == 0x1e) && (**(int **)(lVar5 + 0xb0) == 1)) &&
             (iVar3 = regulator_set_voltage(*(undefined8 *)(lVar5 + 0x10),3500000,3500000),
             iVar3 != 0)) {
            _dev_err(*(undefined8 *)(lVar5 + 8),"failed to set the volref 3.5V\n");
            __hwspin_unlock(*(undefined8 *)(lVar5 + 0x20),3,0);
          }
          else {
            iVar3 = regmap_update_bits_base
                              (*(undefined8 *)(lVar5 + 0x18),*(undefined4 *)(lVar5 + 0xa8),1,1,0,0,0
                              );
            if (iVar3 == 0) {
              iVar3 = regmap_update_bits_base
                                (*(undefined8 *)(lVar5 + 0x18),*(int *)(lVar5 + 0xa8) + 0x54,1,1,0,0
                                 ,0);
              if (((iVar3 == 0) &&
                  (uVar2 = *(uint *)(*(long *)(lVar5 + 0xb0) + 0x10),
                  iVar3 = regmap_update_bits_base
                                    (*(undefined8 *)(lVar5 + 0x18),*(int *)(lVar5 + 0xa8) + 4,
                                     uVar2 | 0x1f,
                                     param_3 <<
                                     (ulong)(*(uint *)(*(long *)(lVar5 + 0xb0) + 0xc) & 0x1f) &
                                     uVar2 | auVar9._8_4_ & 0x1f,0,0,0), iVar3 == 0)) &&
                 ((iVar3 = regmap_update_bits_base
                                     (*(undefined8 *)(lVar5 + 0x18),*(undefined4 *)(lVar5 + 0xa8),
                                      0xf4,4,0,0,0), iVar3 == 0 &&
                  (iVar3 = regmap_update_bits_base
                                     (*(undefined8 *)(lVar5 + 0x18),*(undefined4 *)(lVar5 + 0xa8),2,
                                      2,0,0,0), iVar3 == 0)))) {
                lVar6 = ktime_get();
                iVar3 = regmap_read(*(undefined8 *)(lVar5 + 0x18),*(int *)(lVar5 + 0xa8) + 0x5c,
                                    &uStack_60);
                if (iVar3 == 0) {
                  while ((uStack_60 & 1) == 0) {
                    lVar7 = ktime_get();
                    if (lVar6 + 1000000000 < lVar7) {
                      iVar3 = regmap_read(*(undefined8 *)(lVar5 + 0x18),
                                          *(int *)(lVar5 + 0xa8) + 0x5c,&uStack_60);
                      uVar8 = uStack_60 & 0xffffffff;
                      if (iVar3 == 0) {
                        if ((uStack_60 & 1) != 0) break;
                        iVar3 = -0x6e;
                      }
                      goto LAB_0010442c;
                    }
                    usleep_range(0x7e,500);
                    iVar3 = regmap_read(*(undefined8 *)(lVar5 + 0x18),*(int *)(lVar5 + 0xa8) + 0x5c,
                                        &uStack_60);
                    if (iVar3 != 0) goto LAB_001042a8;
                  }
                  iVar3 = regmap_read(*(undefined8 *)(lVar5 + 0x18),*(int *)(lVar5 + 0xa8) + 0x4c,
                                      (long)&uStack_60 + 4);
                  uStack_60 = uStack_60 & 0xfffffffffff;
                }
                else {
LAB_001042a8:
                  uVar8 = uStack_60 & 0xffffffff;
LAB_0010442c:
                  _dev_err(*(undefined8 *)(lVar5 + 8),"read adc timeout 0x%x\n",uVar8);
                }
              }
              regmap_update_bits_base
                        (*(undefined8 *)(lVar5 + 0x18),*(undefined4 *)(lVar5 + 0xa8),1,0,0,0,0);
            }
            if (((uVar1 == 0x1e) && (**(int **)(lVar5 + 0xb0) == 1)) &&
               (iVar4 = regulator_set_voltage(*(undefined8 *)(lVar5 + 0x10),2800000,2800000),
               iVar4 != 0)) {
              _dev_err(*(undefined8 *)(lVar5 + 8),
                       "failed to set the volref 2.8V, ret_volref = 0x%x\n",iVar4);
              __hwspin_unlock(*(undefined8 *)(lVar5 + 0x20),3,0);
              iVar3 = 1;
            }
            else {
              __hwspin_unlock(*(undefined8 *)(lVar5 + 0x20),3,0);
              if (iVar3 == 0) {
                *param_4 = uStack_60._4_4_;
              }
            }
          }
        }
        else {
          _dev_err(*(undefined8 *)(lVar5 + 8),"timeout to get the hwspinlock\n");
        }
      }
      else {
        puStack_50 = &stack0xfffffffffffffff0;
        _dev_info(*(undefined8 *)(lVar5 + 8),"adc_exp: adc clk26 bas been closed, ignore.\n");
        iVar3 = -0x10;
      }
      if (___stack_chk_guard != lStack_58) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return iVar3;
    }
    iVar3 = 1;
    *(int *)(param_1 + (ulong)*(uint *)(param_2 + 4) * 4 + 0x5e8) = param_3;
  }
  else {
    iVar3 = -0x16;
  }
  return iVar3;
}



====================sc27xx_adc_read@001040b8====================
// signature: undefined sc27xx_adc_read(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sc27xx_adc_read(long param_1,uint param_2,int param_3,undefined4 *param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  if ((*(char *)(param_1 + 200) == '\0') || (*(char *)(param_1 + 0xc9) == '\0')) {
    iVar2 = __hwspin_lock_timeout(*(undefined8 *)(param_1 + 0x20),5000,3,0);
    if (iVar2 == 0) {
      if ((((param_2 & 0xfffffffe) == 0x1e) && (**(int **)(param_1 + 0xb0) == 1)) &&
         (iVar2 = regulator_set_voltage(*(undefined8 *)(param_1 + 0x10),3500000,3500000), iVar2 != 0
         )) {
        _dev_err(*(undefined8 *)(param_1 + 8),"failed to set the volref 3.5V\n");
        __hwspin_unlock(*(undefined8 *)(param_1 + 0x20),3,0);
      }
      else {
        iVar2 = regmap_update_bits_base
                          (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 0xa8),1,1,0,0,0
                          );
        if (iVar2 == 0) {
          iVar2 = regmap_update_bits_base
                            (*(undefined8 *)(param_1 + 0x18),*(int *)(param_1 + 0xa8) + 0x54,1,1,0,0
                             ,0);
          if (((iVar2 == 0) &&
              (uVar1 = *(uint *)(*(long *)(param_1 + 0xb0) + 0x10),
              iVar2 = regmap_update_bits_base
                                (*(undefined8 *)(param_1 + 0x18),*(int *)(param_1 + 0xa8) + 4,
                                 uVar1 | 0x1f,
                                 param_3 <<
                                 (ulong)(*(uint *)(*(long *)(param_1 + 0xb0) + 0xc) & 0x1f) & uVar1
                                 | param_2 & 0x1f,0,0,0), iVar2 == 0)) &&
             ((iVar2 = regmap_update_bits_base
                                 (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 0xa8),
                                  0xf4,4,0,0,0), iVar2 == 0 &&
              (iVar2 = regmap_update_bits_base
                                 (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 0xa8),2,
                                  2,0,0,0), iVar2 == 0)))) {
            lVar4 = ktime_get();
            iVar2 = regmap_read(*(undefined8 *)(param_1 + 0x18),*(int *)(param_1 + 0xa8) + 0x5c,
                                &local_50);
            if (iVar2 == 0) {
              while ((local_50 & 1) == 0) {
                lVar5 = ktime_get();
                if (lVar4 + 1000000000 < lVar5) {
                  iVar2 = regmap_read(*(undefined8 *)(param_1 + 0x18),
                                      *(int *)(param_1 + 0xa8) + 0x5c,&local_50);
                  uVar6 = local_50 & 0xffffffff;
                  if (iVar2 == 0) {
                    if ((local_50 & 1) != 0) break;
                    iVar2 = -0x6e;
                  }
                  goto LAB_0010442c;
                }
                usleep_range(0x7e,500);
                iVar2 = regmap_read(*(undefined8 *)(param_1 + 0x18),*(int *)(param_1 + 0xa8) + 0x5c,
                                    &local_50);
                if (iVar2 != 0) goto LAB_001042a8;
              }
              iVar2 = regmap_read(*(undefined8 *)(param_1 + 0x18),*(int *)(param_1 + 0xa8) + 0x4c,
                                  (long)&local_50 + 4);
              local_50 = local_50 & 0xfffffffffff;
            }
            else {
LAB_001042a8:
              uVar6 = local_50 & 0xffffffff;
LAB_0010442c:
              _dev_err(*(undefined8 *)(param_1 + 8),"read adc timeout 0x%x\n",uVar6);
            }
          }
          regmap_update_bits_base
                    (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 0xa8),1,0,0,0,0);
        }
        if ((((param_2 & 0xfffffffe) == 0x1e) && (**(int **)(param_1 + 0xb0) == 1)) &&
           (iVar3 = regulator_set_voltage(*(undefined8 *)(param_1 + 0x10),2800000,2800000),
           iVar3 != 0)) {
          _dev_err(*(undefined8 *)(param_1 + 8),"failed to set the volref 2.8V, ret_volref = 0x%x\n"
                   ,iVar3);
          __hwspin_unlock(*(undefined8 *)(param_1 + 0x20),3,0);
          iVar2 = 1;
        }
        else {
          __hwspin_unlock(*(undefined8 *)(param_1 + 0x20),3,0);
          if (iVar2 == 0) {
            *param_4 = local_50._4_4_;
          }
        }
      }
    }
    else {
      _dev_err(*(undefined8 *)(param_1 + 8),"timeout to get the hwspinlock\n");
    }
  }
  else {
    _dev_info(*(undefined8 *)(param_1 + 8),"adc_exp: adc clk26 bas been closed, ignore.\n");
    iVar2 = -0x10;
  }
  if (___stack_chk_guard != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar2;
}



====================adc_nvmem_cell_calib_data@00104444====================
// signature: undefined adc_nvmem_cell_calib_data(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong adc_nvmem_cell_calib_data(long param_1)

{
  size_t __n;
  ulong uVar1;
  void *__src;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  ulong local_38;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  local_38 = 0;
  if (param_1 == 0) {
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = nvmem_cell_get(*(undefined8 *)(param_1 + 8));
    if ((uVar1 != 0) && (uVar1 < 0xfffffffffffff001)) {
      __src = (void *)nvmem_cell_read(uVar1,&local_38);
      if ((__src == (void *)0x0) || ((void *)0xfffffffffffff000 < __src)) {
        nvmem_cell_put(uVar1);
        uVar1 = (ulong)__src & 0xffffffff;
      }
      else {
        __n = local_38;
        if (3 < local_38) {
          __n = 4;
        }
        memcpy(&local_2c,__src,__n);
        kfree(__src);
        nvmem_cell_put(uVar1);
        uVar1 = (ulong)local_2c;
      }
    }
  }
  if (___stack_chk_guard == local_28) {
    return uVar1 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================__cfi_check@00105000====================
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



====================__typeid__ZTSFvP15sc27xx_adc_dataE_global_addr@001051e8====================
// signature: undefined __typeid__ZTSFvP15sc27xx_adc_dataE_global_addr(void)

void __typeid__ZTSFvP15sc27xx_adc_dataE_global_addr(long param_1)

{
  uint uVar1;
  long lVar2;
  undefined4 uVar3;
  
  lVar2 = 0;
  do {
    if ((uint)lVar2 < 0x20) {
      uVar1 = 1 << (ulong)((uint)lVar2 & 0x1f);
      if ((uVar1 & 0xc0080400) == 0) {
        if ((uVar1 & 0x280) == 0) {
          if ((uVar1 & 0x2001) == 0) goto LAB_00103714;
          uVar3 = 1;
        }
        else {
          uVar3 = 2;
        }
      }
      else {
        uVar3 = 3;
      }
    }
    else {
LAB_00103714:
      uVar3 = 0;
    }
    *(undefined4 *)(param_1 + 0x28 + lVar2 * 4) = uVar3;
    lVar2 = lVar2 + 1;
    if (lVar2 == 0x20) {
      return;
    }
  } while( true );
}



====================__typeid__ZTSFiiiE_global_addr@001051f8====================
// signature: undefined __typeid__ZTSFiiiE_global_addr(void)

undefined4 __typeid__ZTSFiiiE_global_addr(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0x10001;
  switch(param_1) {
  case 0xb:
    goto switchD_00103744_caseD_b;
  default:
    if (param_2 - 1U < 3) {
      uVar1 = *(undefined4 *)(&DAT_00102068 + (long)(int)(param_2 - 1U) * 4);
    }
switchD_00103744_caseD_b:
    return uVar1;
  case 0xe:
    uVar1 = 0x440384;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0xf:
    uVar1 = 0x10003;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0x15:
  case 0x16:
  case 0x17:
    uVar1 = 0x30008;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  }
}



====================init_module@00105220====================
// signature: undefined init_module(void)

void init_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  __platform_driver_register(&sc27xx_adc_driver,&__this_module);
  return;
}



====================cleanup_module@0010524c====================
// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&sc27xx_adc_driver);
  return;
}



====================__platform_driver_register@00106000====================
// signature: undefined __platform_driver_register(void)

/* WARNING: Control flow encountered bad instruction data */

void __platform_driver_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_device_get_match_data@00106008====================
// signature: undefined of_device_get_match_data(void)

/* WARNING: Control flow encountered bad instruction data */

void of_device_get_match_data(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_dev_err@00106010====================
// signature: undefined _dev_err(void)

/* WARNING: Control flow encountered bad instruction data */

void _dev_err(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_iio_device_alloc@00106018====================
// signature: undefined devm_iio_device_alloc(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_iio_device_alloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_get_regmap@00106020====================
// signature: undefined dev_get_regmap(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_get_regmap(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================platform_get_irq@00106028====================
// signature: undefined platform_get_irq(void)

/* WARNING: Control flow encountered bad instruction data */

void platform_get_irq(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_hwspin_lock_get_id@00106030====================
// signature: undefined of_hwspin_lock_get_id(void)

/* WARNING: Control flow encountered bad instruction data */

void of_hwspin_lock_get_id(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================hwspin_lock_request_specific@00106038====================
// signature: undefined hwspin_lock_request_specific(void)

/* WARNING: Control flow encountered bad instruction data */

void hwspin_lock_request_specific(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_add_action@00106040====================
// signature: undefined devm_add_action(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_add_action(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_regulator_get_optional@00106048====================
// signature: undefined devm_regulator_get_optional(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_regulator_get_optional(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__cfi_slowpath@00106050====================
// signature: undefined __cfi_slowpath(void)

/* WARNING: Control flow encountered bad instruction data */

void __cfi_slowpath(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__devm_iio_device_register@00106058====================
// signature: undefined __devm_iio_device_register(void)

/* WARNING: Control flow encountered bad instruction data */

void __devm_iio_device_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_property_read_variable_u32_array@00106060====================
// signature: undefined of_property_read_variable_u32_array(void)

/* WARNING: Control flow encountered bad instruction data */

void of_property_read_variable_u32_array(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================hwspin_lock_free@00106068====================
// signature: undefined hwspin_lock_free(void)

/* WARNING: Control flow encountered bad instruction data */

void hwspin_lock_free(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_update_bits_base@00106070====================
// signature: undefined regmap_update_bits_base(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_update_bits_base(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================nvmem_cell_get@00106078====================
// signature: undefined nvmem_cell_get(void)

/* WARNING: Control flow encountered bad instruction data */

void nvmem_cell_get(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================nvmem_cell_read@00106080====================
// signature: undefined nvmem_cell_read(void)

/* WARNING: Control flow encountered bad instruction data */

void nvmem_cell_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================nvmem_cell_put@00106088====================
// signature: undefined nvmem_cell_put(void)

/* WARNING: Control flow encountered bad instruction data */

void nvmem_cell_put(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================kfree@00106090====================
// signature: undefined kfree(void)

/* WARNING: Control flow encountered bad instruction data */

void kfree(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================syscon_regmap_lookup_by_phandle_args@00106098====================
// signature: undefined syscon_regmap_lookup_by_phandle_args(void)

/* WARNING: Control flow encountered bad instruction data */

void syscon_regmap_lookup_by_phandle_args(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_dev_info@001060a0====================
// signature: undefined _dev_info(void)

/* WARNING: Control flow encountered bad instruction data */

void _dev_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_lock@001060a8====================
// signature: undefined mutex_lock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_lock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_unlock@001060b0====================
// signature: undefined mutex_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regulator_set_voltage@001060b8====================
// signature: undefined regulator_set_voltage(void)

/* WARNING: Control flow encountered bad instruction data */

void regulator_set_voltage(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================ktime_get@001060c0====================
// signature: undefined ktime_get(void)

/* WARNING: Control flow encountered bad instruction data */

void ktime_get(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_read@001060c8====================
// signature: undefined regmap_read(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================usleep_range@001060d0====================
// signature: undefined usleep_range(void)

/* WARNING: Control flow encountered bad instruction data */

void usleep_range(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__hwspin_lock_timeout@001060d8====================
// signature: undefined __hwspin_lock_timeout(void)

/* WARNING: Control flow encountered bad instruction data */

void __hwspin_lock_timeout(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__hwspin_unlock@001060e0====================
// signature: undefined __hwspin_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void __hwspin_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================platform_driver_unregister@001060e8====================
// signature: undefined platform_driver_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void platform_driver_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__stack_chk_fail@001060f0====================
// signature: noreturn undefined __stack_chk_fail(void)

/* WARNING: Control flow encountered bad instruction data */

void __stack_chk_fail(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__ubsan_handle_cfi_check_fail_abort@00106100====================
// signature: undefined __ubsan_handle_cfi_check_fail_abort(void)

/* WARNING: Control flow encountered bad instruction data */

void __ubsan_handle_cfi_check_fail_abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================memcpy@00106108====================
// signature: void * memcpy(void * __dest, void * __src, size_t __n)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



