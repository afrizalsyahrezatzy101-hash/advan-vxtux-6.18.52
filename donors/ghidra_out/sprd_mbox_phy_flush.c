// sprd_mbox_phy_flush @ 001037b8
// signature: undefined sprd_mbox_phy_flush(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 sprd_mbox_phy_flush(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  undefined8 *in_x18;
  long lVar3;
  ulong uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = _jiffies;
  lVar3 = *param_1;
  uVar4 = param_1[0x1e];
  lVar2 = __msecs_to_jiffies(param_2);
  if (_jiffies - (lVar2 + lVar1) < 0) {
    do {
      DataMemoryBarrier(0,1);
      if ((1L << (uVar4 & 0x3f) & (ulong)(*(uint *)(*(long *)(lVar3 + 0x90) + 0x14) & 0xff)) == 0) {
        mbox_chan_txdone(param_1,0);
        return 0;
      }
      __const_udelay(0x10c7);
    } while (_jiffies - (lVar2 + lVar1) < 0);
  }
  return 0xffffffc2;
}


