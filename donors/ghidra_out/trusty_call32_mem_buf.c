// trusty_call32_mem_buf @ 00103344
// signature: undefined trusty_call32_mem_buf(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 trusty_call32_mem_buf(long param_1,uint param_2,long param_3,ulong param_4,ulong param_5)

{
  ulong uVar1;
  code *pcVar2;
  undefined4 uVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_40;
  ulong uStack_38;
  ulong local_30;
  ulong uStack_28;
  ulong local_20;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  uVar3 = 0xffffffea;
  if ((param_1 != 0) && (param_3 != 0)) {
    uVar1 = mair_el1;
    uVar4 = (param_3 + (_memstart_addr >> 0xc) * 0x40) * 0x40 + 0x4008000000 | param_5;
    if ((int)param_2 < 0) {
      if ((param_2 >> 0x1e & 1) != 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0x800,0x103418);
        (*pcVar2)();
      }
      uStack_38 = (ulong)param_2;
      local_30 = uVar4 & 0xffffffff;
      local_40 = 0;
      uStack_28 = (uVar4 & 0xffff00000000 | (uVar1 >> (((uint)param_5 & 0x1c) << 1) & 0xff) << 0x30)
                  >> 0x20;
      local_20 = param_4 & 0xffffffff;
      uVar3 = __smc(&local_40);
    }
    else {
      uVar3 = trusty_std_call32();
    }
  }
  if (___stack_chk_guard == local_18) {
    return uVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


