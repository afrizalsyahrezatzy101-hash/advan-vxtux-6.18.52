// trusty_std_call_inner @ 001016f0
// signature: undefined trusty_std_call_inner(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void trusty_std_call_inner
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5)

{
  int iVar1;
  undefined8 *in_x18;
  int iVar2;
  undefined8 unaff_x30;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  iVar2 = 5;
  uStack_88 = param_2;
  local_80 = param_3;
  local_78 = param_4;
  local_70 = param_5;
  while( true ) {
    while( true ) {
      local_90 = 0;
      iVar1 = __smc(&local_90);
      if (iVar1 != -0xc) break;
      uStack_88 = 0x3c000002;
      local_78 = 0;
      local_70 = 0;
      local_80 = 0;
    }
    if ((iVar2 == 0) || (iVar1 != -5)) break;
    iVar2 = iVar2 + -1;
    uStack_88 = param_2;
    local_80 = param_3;
    local_78 = param_4;
    local_70 = param_5;
  }
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


