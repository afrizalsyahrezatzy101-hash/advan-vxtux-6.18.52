// trusty_std_call32 @ 00101268
// signature: undefined trusty_std_call32(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int trusty_std_call32(long param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  code *pcVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  long local_90;
  ulong uStack_88;
  ulong local_80;
  ulong local_78;
  ulong local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  if ((int)param_2 < 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101464);
    (*pcVar1)();
  }
  if ((param_2 >> 0x1e & 1) != 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x101468);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x98);
  if (param_2 != 0x3c000003) {
    mutex_lock(lVar3);
    *(undefined4 *)(lVar3 + 0x30) = 0;
  }
  uStack_88 = (ulong)param_2;
  local_80 = (ulong)param_3;
  local_78 = (ulong)param_4;
  local_70 = (ulong)param_5;
  local_90 = param_1;
  iVar2 = trusty_std_call_helper_reschedule(&local_90);
  while ((iVar2 == -3 || (iVar2 == -0xd))) {
    if ((iVar2 == -0xd) && (iVar2 = wait_for_completion_timeout(lVar3 + 0x30,0x9c4), iVar2 == 0)) {
      printk(&DAT_0010055c,"trusty_std_call_cpu_idle");
    }
    uStack_88 = 0x3c000000;
    local_78 = 0;
    local_70 = 0;
    local_80 = 0;
    local_90 = param_1;
    iVar2 = trusty_std_call_helper_reschedule(&local_90);
  }
  if ((iVar2 == -0xb) && (trusty_std_call32___warned == '\0')) {
    trusty_std_call32___warned = 1;
    __warn_printk("trusty crashed");
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x800,0x10144c);
    (*pcVar1)();
  }
  if (iVar2 == -0xb) {
                    /* WARNING: Subroutine does not return */
    panic("tospanic");
  }
  if (param_2 == 0x3c000003) {
    complete(lVar3 + 0x30);
  }
  else {
    mutex_unlock(lVar3);
  }
  if (___stack_chk_guard == local_68) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


