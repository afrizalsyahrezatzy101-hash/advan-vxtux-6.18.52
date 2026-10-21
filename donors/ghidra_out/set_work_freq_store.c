// set_work_freq_store @ 0010841c
// signature: undefined set_work_freq_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_work_freq_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined4 local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_4c = 0;
  uVar3 = get_display_power_status();
  if ((uVar3 & 1) == 0) {
    printk(&DAT_0010244c);
  }
  else {
    lVar1 = param_1 + -0x20;
    mutex_lock(lVar1);
    iVar2 = sscanf(param_3,"%d\n",&local_4c);
    if (iVar2 != 0) {
      *(undefined4 *)(lVar4 + 1000) = 0;
      *(undefined4 *)(lVar4 + 0x3e0) = local_4c;
      iVar2 = update_devfreq(param_1 + -0x30);
      if (iVar2 != 0) {
        param_4 = iVar2;
      }
      mutex_unlock(lVar1);
      lVar4 = (long)param_4;
      goto LAB_001084c4;
    }
    mutex_unlock(lVar1);
  }
  lVar4 = -0x16;
LAB_001084c4:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar4);
}


