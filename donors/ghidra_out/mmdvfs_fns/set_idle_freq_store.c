====================set_idle_freq_store@0010d3d0====================
// signature: undefined set_idle_freq_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_idle_freq_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  undefined8 *in_x18;
  code *pcVar5;
  long *plVar6;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 != 0) {
      printk(&DAT_00105775,"set_idle_freq_store");
      lVar4 = plVar6[0xc];
      if ((lVar4 == 0) || (*(long *)(lVar4 + 0x50) == 0)) {
        printk(&DAT_001047b2,"set_idle_freq_store");
      }
      else {
        uVar3 = local_50 & 0xffffffff;
        *(undefined4 *)(plVar6 + 0xf) = (undefined4)local_50;
        pcVar5 = *(code **)(lVar4 + 0x50);
        if (0x1b < ((ulong)(pcVar5 + -0x1176d4) >> 2 | (long)pcVar5 << 0x3e)) {
          __cfi_slowpath(0x362db8165d0ff4ec,pcVar5);
          uVar3 = (ulong)*(uint *)(plVar6 + 0xf);
        }
        iVar1 = (*pcVar5)(param_1 + -0x30,uVar3);
      }
      if (iVar1 != 0) {
        param_4 = iVar1;
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010d448;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010d448:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


