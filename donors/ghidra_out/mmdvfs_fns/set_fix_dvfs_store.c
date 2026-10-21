====================set_fix_dvfs_store@0010d9e0====================
// signature: undefined set_fix_dvfs_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_fix_dvfs_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined8 *in_x18;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      *(undefined4 *)((long)plVar5 + 0x84) = (undefined4)local_50;
      if ((plVar5[0xc] == 0) || (pcVar3 = *(code **)(plVar5[0xc] + 0xf8), pcVar3 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_fix_dvfs_store");
      }
      else {
        uVar4 = (undefined4)local_50;
        if (0x1b < ((ulong)(pcVar3 + -0x1176d4) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0x362db8165d0ff4ec,pcVar3);
          uVar4 = *(undefined4 *)((long)plVar5 + 0x84);
        }
        (*pcVar3)(param_1 + -0x30,uVar4);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010daa8;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010daa8:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


