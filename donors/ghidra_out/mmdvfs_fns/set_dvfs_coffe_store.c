====================set_dvfs_coffe_store@0010dc58====================
// signature: undefined set_dvfs_coffe_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_coffe_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  code *pcVar3;
  long *plVar4;
  undefined8 unaff_x30;
  long local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%d,%d,%d,%d,%d,%d,%d,%d\n",&local_68,(ulong)&local_68 | 4,&uStack_60,
                   (long)&uStack_60 + 4,&local_58,(long)&local_58 + 4,&uStack_50,
                   (long)&uStack_50 + 4);
    plVar4[0x23] = local_58;
    plVar4[0x24] = uStack_50;
    plVar4[0x21] = local_68;
    plVar4[0x22] = uStack_60;
    if (iVar1 == 8) {
      if ((plVar4[0xc] == 0) || (pcVar3 = *(code **)(plVar4[0xc] + 0x78), pcVar3 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_dvfs_coffe_store");
      }
      else {
        __cfi_slowpath(0xa337d2e020122362,pcVar3);
        (*pcVar3)(param_1 + -0x30,&local_68);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010dd50;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010dd50:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


