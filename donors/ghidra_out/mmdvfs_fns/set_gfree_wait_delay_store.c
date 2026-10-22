====================set_gfree_wait_delay_store@0010e478====================
// signature: undefined set_gfree_wait_delay_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_gfree_wait_delay_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar2 = sscanf(param_3," %lu\n",&local_40);
    printk(&DAT_00105c69,"set_gfree_wait_delay_store",iVar2,local_40,param_4);
    if (iVar2 == 1) {
      *(undefined4 *)(plVar5 + 0x21) = (undefined4)local_40;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0x98), pcVar4 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_gfree_wait_delay_store");
      }
      else {
        uVar1 = (undefined4)local_40;
        if (0x21 < ((ulong)(pcVar4 + -0x1175f8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar4);
          uVar1 = (undefined4)plVar5[0x21];
        }
        (*pcVar4)(uVar1);
      }
      mutex_unlock(param_1);
      lVar3 = (long)param_4;
      goto LAB_0010e568;
    }
    mutex_unlock(param_1);
  }
  lVar3 = -0x16;
LAB_0010e568:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}


