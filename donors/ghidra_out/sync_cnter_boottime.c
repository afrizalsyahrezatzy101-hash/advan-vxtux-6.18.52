// sync_cnter_boottime @ 00101178
// signature: undefined sync_cnter_boottime(void)
// calling convention: unknown


undefined8 sync_cnter_boottime(void)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  code *pcVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  systimer_seq = systimer_seq + 1;
  DataMemoryBarrier(2,2);
  cnter_to_boottime = ktime_get_boot_fast_ns();
  DAT_00100090 = 0;
  if (sprd_systimer_addr_base != 0) {
    DAT_00100090 = (ulong)*(uint *)(sprd_systimer_addr_base + 0xc);
  }
  if (sprd_sysfrt_addr_base == 0) {
    DAT_00100098 = 0;
  }
  else {
    DAT_00100098 = CONCAT44(*(undefined4 *)(sprd_sysfrt_addr_base + 0xc),
                            *(undefined4 *)(sprd_sysfrt_addr_base + 4));
  }
  DataMemoryBarrier(2,2);
  systimer_seq = systimer_seq + 1;
  pcVar2 = *(code **)(DAT_001000e8 + 0x30);
  __cfi_slowpath(0x3d814571b26b7f8a,pcVar2);
  uVar1 = (*pcVar2)();
  hrtimer_forward(&cnt_to_boot_timer,uVar1,600000000000);
  return 1;
}


