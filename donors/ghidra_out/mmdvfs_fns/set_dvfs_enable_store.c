====================set_dvfs_enable_store@0010ce38====================
// signature: undefined set_dvfs_enable_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_enable_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  code *pcVar4;
  undefined8 *in_x18;
  ulong *puVar5;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  puVar5 = *(ulong **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*puVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      printk(&DAT_00105d0f,"set_dvfs_enable_store",1,local_50);
      *(int *)((long)puVar5 + 0x6c) = (int)local_50;
      *puVar5 = local_50;
      if ((puVar5[0xc] == 0) || (pcVar4 = *(code **)(puVar5[0xc] + 0x30), pcVar4 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_dvfs_enable_store");
      }
      else {
        uVar3 = local_50;
        if (0xb < ((ulong)(pcVar4 + -0x1177fc) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xe1569ae966436cf9,pcVar4);
          uVar3 = (ulong)*(uint *)((long)puVar5 + 0x6c);
        }
        (*pcVar4)(param_1 + -0x30,uVar3);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010ceb4;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010ceb4:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


