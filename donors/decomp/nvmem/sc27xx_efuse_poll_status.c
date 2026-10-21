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



