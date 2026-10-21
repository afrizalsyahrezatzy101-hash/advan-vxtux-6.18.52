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



