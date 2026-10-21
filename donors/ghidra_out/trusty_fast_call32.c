// trusty_fast_call32 @ 00101b14
// signature: undefined trusty_fast_call32(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void trusty_fast_call32(undefined8 param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  code *pcVar1;
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
  if (-1 < (int)param_2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101b88);
    (*pcVar1)();
  }
  if ((param_2 >> 0x1e & 1) == 0) {
    uStack_38 = (ulong)param_2;
    local_30 = (ulong)param_3;
    uStack_28 = (ulong)param_4;
    local_20 = (ulong)param_5;
    local_40 = 0;
    __smc(&local_40);
    if (___stack_chk_guard == local_18) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101b8c);
  (*pcVar1)();
}


