====================set_sys_dvfs_clk_gate_ctrl_store@0010b248====================
// signature: undefined set_sys_dvfs_clk_gate_ctrl_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_dvfs_clk_gate_ctrl_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x30) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x110), pcVar3 == (code *)0x0)) {
      printk(&DAT_001047b2,"set_sys_dvfs_clk_gate_ctrl_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x30);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


