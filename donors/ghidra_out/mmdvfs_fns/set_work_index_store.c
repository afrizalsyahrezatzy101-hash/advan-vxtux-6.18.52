====================set_work_index_store@0010d5e4====================
// signature: undefined set_work_index_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_work_index_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  code *pcVar5;
  undefined8 *in_x18;
  long *plVar6;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar3 = param_1 + -0x20;
    mutex_lock(lVar3);
    iVar2 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar2 == 1) {
      printk(&DAT_0010623c,"set_work_index_store",param_4);
      printk(&DAT_00105ef9,"set_work_index_store",local_50);
      lVar4 = plVar6[0xc];
      if ((lVar4 == 0) || (*(long *)(lVar4 + 0xc0) == 0)) {
        printk(&DAT_001047b2,"set_work_index_store");
      }
      else {
        *(undefined4 *)((long)plVar6 + 0x7c) = (undefined4)local_50;
        pcVar5 = *(code **)(lVar4 + 0xc0);
        uVar1 = (undefined4)local_50;
        if (0xe < ((ulong)(pcVar5 + -0x11782c) >> 2 | (long)pcVar5 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar5);
          uVar1 = *(undefined4 *)((long)plVar6 + 0x7c);
        }
        (*pcVar5)(param_1 + -0x30,uVar1);
      }
      mutex_unlock(lVar3);
      lVar3 = (long)param_4;
      goto LAB_0010d660;
    }
    mutex_unlock(lVar3);
  }
  lVar3 = -0x16;
LAB_0010d660:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}


