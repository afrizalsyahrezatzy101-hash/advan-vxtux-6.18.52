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



