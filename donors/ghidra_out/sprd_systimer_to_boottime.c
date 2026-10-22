// sprd_systimer_to_boottime @ 00101030
// signature: undefined sprd_systimer_to_boottime(void)
// calling convention: unknown


long sprd_systimer_to_boottime(long param_1,int param_2)

{
  if (param_2 == 1) {
    if (sprd_sysfrt_addr_base != 0) {
      DataMemoryBarrier(2,1);
      DataMemoryBarrier(2,1);
      return ((param_1 - DAT_00100098) * (ulong)DAT_001000a8 >> ((ulong)DAT_001000ac & 0x3f)) +
             cnter_to_boottime;
    }
  }
  else if ((param_2 == 0) && (sprd_systimer_addr_base != 0)) {
    DataMemoryBarrier(2,1);
    DataMemoryBarrier(2,1);
    return ((ulong)(uint)((int)param_1 - (int)DAT_00100090) * (ulong)DAT_001000a0 >>
           ((ulong)DAT_001000a4 & 0x3f)) + cnter_to_boottime;
  }
  return 0;
}


