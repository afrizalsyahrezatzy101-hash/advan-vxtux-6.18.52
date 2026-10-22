// trusty_fast_call64 @ 00101d64
// signature: undefined trusty_fast_call64(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void trusty_fast_call64(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                       undefined8 param_5)

{
  code *pcVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 local_30;
  undefined8 uStack_28;
  undefined8 local_20;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  if (*(long *)(param_1 + 0x98) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101dd0);
    (*pcVar1)();
  }
  if (-1 < (int)(uint)param_2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101dd4);
    (*pcVar1)();
  }
  if (((uint)param_2 >> 0x1e & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101dd8);
    (*pcVar1)();
  }
  local_40 = 0;
  uStack_38 = param_2;
  local_30 = param_3;
  uStack_28 = param_4;
  local_20 = param_5;
  __smc(&local_40);
  if (___stack_chk_guard == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


