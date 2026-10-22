====================set_work_freq_store@0010d224====================
// signature: undefined set_work_freq_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_work_freq_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  long *plVar3;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar3 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      printk(&DAT_00105434,"set_work_freq_store",local_50);
      *(int *)((long)plVar3 + 0x74) = (int)local_50;
      iVar1 = update_devfreq(param_1 + -0x30);
      if (iVar1 != 0) {
        param_4 = iVar1;
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010d2a0;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010d2a0:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


