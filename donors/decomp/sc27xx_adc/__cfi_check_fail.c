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



