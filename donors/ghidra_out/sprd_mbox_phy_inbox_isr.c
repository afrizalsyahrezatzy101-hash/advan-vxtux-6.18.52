// sprd_mbox_phy_inbox_isr @ 0010399c
// signature: undefined sprd_mbox_phy_inbox_isr(void)
// calling convention: unknown


undefined8 sprd_mbox_phy_inbox_isr(undefined8 param_1,long param_2)

{
  uint uVar1;
  char cVar2;
  undefined4 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(*(long *)(param_2 + 0x90) + 0x14);
  DataMemoryBarrier(0,1);
  DataMemoryBarrier(0,2);
  *(uint *)(*(long *)(param_2 + 0x90) + 0x10) = uVar1 & 0xffff00;
  DataMemoryBarrier(0,1);
  cVar2 = get_tx_fifo_mask(~uVar1 & uVar1 >> 0x10);
  if (cVar2 != '\0') {
    mbox_tx_mask = cVar2;
    __wake_up(&deliver_thread_wait,3,1,0);
  }
  DataMemoryBarrier(0,2);
  uVar3 = 3;
  if ((uVar1 & 0xff) == 0) {
    uVar3 = 4;
  }
  *(undefined4 *)(*(long *)(param_2 + 0x90) + 0x1c) = uVar3;
  DataMemoryBarrier(0,2);
  *(undefined4 *)(*(long *)(param_2 + 0x90) + 0x18) = 1;
  return 1;
}


