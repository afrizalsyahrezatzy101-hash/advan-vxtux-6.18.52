// set_work_index_store.108 @ 001095ac
// signature: undefined set_work_index_store.108(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_work_index_store_108(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  code *pcVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  undefined4 local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_3c = 0;
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_00102e1a);
  }
  else {
    iVar1 = sscanf(param_3,"%u\n",&local_3c);
    if (iVar1 == 1) {
      lVar3 = *(long *)(lVar5 + 1000);
      if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x70), pcVar4 == (code *)0x0)) {
        printk(&DAT_0010230e,"set_work_index_store");
      }
      else {
        if (0x3b < ((ulong)(pcVar4 + -0x117e0c) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0x32d5576efc9df576,pcVar4);
        }
        (*pcVar4)(lVar5,local_3c);
      }
      goto LAB_00109650;
    }
  }
  param_4 = 0xffffffffffffffea;
LAB_00109650:
  if (___stack_chk_guard == local_38) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


