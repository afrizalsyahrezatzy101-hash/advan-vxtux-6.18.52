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



