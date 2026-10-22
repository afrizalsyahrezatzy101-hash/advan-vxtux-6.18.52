// sprd_mbox_phy_shutdown @ 0010371c
// signature: undefined sprd_mbox_phy_shutdown(void)
// calling convention: unknown


void sprd_mbox_phy_shutdown(long *param_1)

{
  char cVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = *param_1;
  lVar2 = param_1[0x1e];
  cVar1 = *(char *)(lVar3 + 0xc4) + -1;
  *(char *)(lVar3 + 0xc4) = cVar1;
  if (cVar1 == '\0') {
    DataMemoryBarrier(0,2);
    *(undefined4 *)(*(long *)(lVar3 + 0x90) + 0x1c) = 7;
    DataMemoryBarrier(0,2);
    *(undefined4 *)(*(long *)(lVar3 + 0x98) + 0x1c) = 0x1f;
    g_started_chan_msk = g_started_chan_msk & 0xfffffffe;
  }
  if ((*(long *)(lVar3 + 0xd8) == param_1[0x1e]) && (*(char *)(lVar3 + 0xd4) != '\0')) {
    DataMemoryBarrier(0,2);
    *(undefined4 *)(*(long *)(lVar3 + 200) + 0x1c) = 0x1f;
    *(undefined1 *)(lVar3 + 0xd4) = 0;
  }
  g_started_chan_msk = g_started_chan_msk & (1 << (ulong)((uint)lVar2 & 0x1f) ^ 0xffffffffU);
  return;
}


