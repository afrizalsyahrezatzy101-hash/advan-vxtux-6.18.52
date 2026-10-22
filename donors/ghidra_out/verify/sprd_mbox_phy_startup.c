// sprd_mbox_phy_startup @ 001035d4
// signature: undefined sprd_mbox_phy_startup(void)
// calling convention: unknown


undefined8 sprd_mbox_phy_startup(long *param_1)

{
  char cVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *param_1;
  lVar4 = param_1[0x1e];
  cVar1 = *(char *)(lVar2 + 0xc4);
  *(char *)(lVar2 + 0xc4) = cVar1 + '\x01';
  if (cVar1 == '\0') {
    DataMemoryBarrier(0,2);
    *(undefined4 *)(*(long *)(lVar2 + 0x98) + 0x10) = 0;
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,2);
    *(uint *)(*(long *)(lVar2 + 0x90) + 0x1c) =
         *(uint *)(*(long *)(lVar2 + 0x90) + 0x1c) & 0xfffffff8 | 4;
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,2);
    *(uint *)(*(long *)(lVar2 + 0x98) + 0x1c) =
         *(uint *)(*(long *)(lVar2 + 0x98) + 0x1c) & 0xfffffffe;
    g_started_chan_msk = g_started_chan_msk | 1;
    lVar3 = *(long *)(lVar2 + 200);
  }
  else {
    lVar3 = *(long *)(lVar2 + 200);
  }
  if ((lVar3 != 0) && (*(char *)(lVar2 + 0xd4) == '\0')) {
    DataMemoryBarrier(0,2);
    *(undefined4 *)(*(long *)(lVar2 + 200) + 0x10) = 0;
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,2);
    *(uint *)(*(long *)(lVar2 + 200) + 0x1c) = *(uint *)(*(long *)(lVar2 + 200) + 0x1c) & 0xfffffffe
    ;
    g_mbox_nowakeup_base = *(undefined8 *)(lVar2 + 200);
    *(undefined1 *)(lVar2 + 0xd4) = 1;
  }
  sprd_mbox_process_rx_fifo();
  g_started_chan_msk = g_started_chan_msk | 1 << (ulong)((uint)lVar4 & 0x1f);
  return 0;
}


