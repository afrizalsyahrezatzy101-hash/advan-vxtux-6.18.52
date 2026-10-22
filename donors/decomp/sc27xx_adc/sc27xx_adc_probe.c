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



