// set_idle_freq_store.183 @ 0010aa24
// signature: undefined set_idle_freq_store.183(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_idle_freq_store_183(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  long lVar3;
  undefined8 unaff_x30;
  undefined4 local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  lVar2 = param_1 + -0x20;
  local_48 = ___stack_chk_guard;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_4c = 0;
  mutex_lock(lVar2);
  iVar1 = sscanf(param_3,"%d\n",&local_4c);
  if (iVar1 == 0) {
    mutex_unlock(lVar2);
    lVar2 = -0x16;
  }
  else {
    *(undefined4 *)(lVar3 + 1000) = 1;
    *(undefined4 *)(lVar3 + 0x3e4) = local_4c;
    iVar1 = update_devfreq(param_1 + -0x30);
    if (iVar1 != 0) {
      param_4 = iVar1;
    }
    mutex_unlock(lVar2);
    lVar2 = (long)param_4;
  }
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


