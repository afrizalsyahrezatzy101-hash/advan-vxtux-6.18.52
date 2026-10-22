// set_dvfs_enable_store @ 00109acc
// signature: undefined set_dvfs_enable_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_enable_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  ulong uVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined4 local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  local_3c = 0;
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_00102e1a);
  }
  else {
    iVar1 = sscanf(param_3,"%u\n",&local_3c);
    if (iVar1 == 1) {
      if ((*(long *)(lVar4 + 1000) == 0) ||
         (pcVar3 = *(code **)(*(long *)(lVar4 + 1000) + 0x10), pcVar3 == (code *)0x0)) {
        printk(&DAT_0010230e,"set_dvfs_enable_store");
      }
      else {
        if (0x3b < ((ulong)(pcVar3 + -0x117e0c) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0x32d5576efc9df576,pcVar3);
        }
        (*pcVar3)(lVar4,local_3c);
        *(undefined4 *)(lVar4 + 0x474) = local_3c;
      }
      lVar4 = (long)param_4;
      goto LAB_00109b7c;
    }
  }
  lVar4 = -0x16;
LAB_00109b7c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar4);
}


