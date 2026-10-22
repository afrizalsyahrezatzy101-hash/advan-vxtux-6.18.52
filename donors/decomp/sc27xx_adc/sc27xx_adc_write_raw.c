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



