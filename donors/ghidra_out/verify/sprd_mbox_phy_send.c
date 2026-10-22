// sprd_mbox_phy_send @ 00103870
// signature: undefined sprd_mbox_phy_send(void)
// calling convention: unknown


undefined8 sprd_mbox_phy_send(long *param_1,undefined4 *param_2)

{
  uint uVar1;
  undefined8 uVar2;
  uint uVar3;
  long lVar4;
  uint uVar5;
  int iVar6;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *param_1;
  uVar5 = *(uint *)(*(long *)(lVar4 + 0x90) + 0x14);
  DataMemoryBarrier(0,1);
  uVar3 = (uint)param_1[0x1e];
  uVar1 = 1 << (ulong)(uVar3 & 0x1f);
  if ((uVar1 & uVar5) == 0) {
    if (*(uint *)(lVar4 + 0xb8) < 0x101) {
LAB_00103940:
      DataMemoryBarrier(0,2);
      uVar2 = 0;
      *(undefined4 *)(*(long *)(lVar4 + 0x90) + 4) = *param_2;
      DataMemoryBarrier(0,2);
      *(undefined4 *)(*(long *)(lVar4 + 0x90) + 8) = param_2[1];
      DataMemoryBarrier(0,2);
      **(uint **)(lVar4 + 0x90) = uVar3;
      DataMemoryBarrier(0,2);
      *(undefined4 *)(*(long *)(lVar4 + 0x90) + 0xc) = 1;
    }
    else {
      iVar6 = 1000;
      do {
        if ((uVar5 & 1 << (ulong)(uVar3 + 0x18 & 0x1f)) == 0) goto LAB_00103940;
        uVar5 = *(uint *)(*(long *)(lVar4 + 0x90) + 0x14);
        DataMemoryBarrier(0,1);
        if ((uVar5 & uVar1) != 0) goto LAB_001038b4;
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
      uVar2 = 0xffffff92;
    }
  }
  else {
LAB_001038b4:
    uVar2 = 0xffffff97;
  }
  return uVar2;
}


