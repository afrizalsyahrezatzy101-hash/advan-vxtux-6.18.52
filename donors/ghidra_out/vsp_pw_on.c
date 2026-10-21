// vsp_pw_on @ 001014bc
// signature: undefined vsp_pw_on(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int vsp_pw_on(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  uint uVar2;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  if (*(long *)(param_1 + 0x5f0) == 0) {
    printk(&DAT_00100331);
    iVar1 = -0x16;
  }
  else {
    iVar1 = regmap_update_bits_base
                      (*(long *)(param_1 + 0x5f0),*(undefined4 *)(param_1 + 0x610),
                       *(uint *)(param_1 + 0x620),~*(uint *)(param_1 + 0x620),0,0,0);
    if (iVar1 == 0) {
      iVar1 = regmap_update_bits_base
                        (*(undefined8 *)(param_1 + 0x5f8),*(undefined4 *)(param_1 + 0x614),
                         *(uint *)(param_1 + 0x624),~*(uint *)(param_1 + 0x624),0,0,0);
      if (iVar1 == 0) {
        uVar2 = 0;
        do {
          __const_udelay(0x13a934);
          iVar1 = regmap_read(*(undefined8 *)(param_1 + 0x600),*(undefined4 *)(param_1 + 0x618),
                              &local_3c);
          if (iVar1 != 0) {
            printk(&DAT_0010045d,"vsp_pw_on",0x91);
            goto LAB_00101584;
          }
          local_3c = local_3c & *(uint *)(param_1 + 0x628);
        } while ((uVar2 < 99) && (uVar2 = uVar2 + 1, local_3c != 0));
        if (local_3c == 0) {
          iVar1 = 0;
        }
        else {
          printk(&DAT_001002e7,"vsp_pw_on");
          iVar1 = -0x6e;
        }
      }
      else {
        printk(&DAT_00100357,"vsp_pw_on",0x86);
      }
    }
    else {
      printk(&DAT_00100357,"vsp_pw_on",0x7c);
    }
  }
LAB_00101584:
  if (___stack_chk_guard == local_38) {
    return iVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


