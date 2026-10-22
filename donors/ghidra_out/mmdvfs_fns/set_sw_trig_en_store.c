====================set_sw_trig_en_store@0010eb10====================
// signature: undefined set_sw_trig_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_sw_trig_en_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  code *pcVar3;
  long *plVar4;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar1 = sscanf(param_3,"%lu\n",&local_40);
    if (iVar1 == 1) {
      *(undefined4 *)((long)plVar4 + 0x114) = (undefined4)local_40;
      if ((plVar4[0xc] == 0) || (pcVar3 = *(code **)(plVar4[0xc] + 0xb8), pcVar3 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_sw_trig_en_store");
      }
      else {
        if (0x21 < ((ulong)(pcVar3 + -0x1175f8) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar3);
        }
        (*pcVar3)((undefined4)local_40);
      }
      mutex_unlock(param_1);
      lVar2 = (long)param_4;
      goto LAB_0010ebcc;
    }
    mutex_unlock(param_1);
  }
  lVar2 = -0x16;
LAB_0010ebcc:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


