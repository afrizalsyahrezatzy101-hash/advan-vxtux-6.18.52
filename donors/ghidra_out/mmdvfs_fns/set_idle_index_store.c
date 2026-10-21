====================set_idle_index_store@0010d80c====================
// signature: undefined set_idle_index_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_idle_index_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  code *pcVar4;
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
    lVar3 = param_1 + -0x20;
    mutex_lock(lVar3);
    iVar2 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar2 == 1) {
      *(undefined4 *)(plVar5 + 0x10) = (undefined4)local_50;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0xd0), pcVar4 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_idle_index_store");
      }
      else {
        uVar1 = (undefined4)local_50;
        if (0xe < ((ulong)(pcVar4 + -0x11782c) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar4);
          uVar1 = (undefined4)plVar5[0x10];
        }
        (*pcVar4)(param_1 + -0x30,uVar1);
      }
      mutex_unlock(lVar3);
      lVar3 = (long)param_4;
      goto LAB_0010d8d0;
    }
    mutex_unlock(lVar3);
  }
  lVar3 = -0x16;
LAB_0010d8d0:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}


