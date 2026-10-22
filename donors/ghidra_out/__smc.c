// __smc @ 0010183c
// signature: undefined __smc(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __smc(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_68;
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
  __arm_smccc_smc(*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10),
                  *(undefined8 *)(param_1 + 0x18),*(undefined8 *)(param_1 + 0x20),0,0,0,0,&local_68,
                  0);
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(local_68);
}


