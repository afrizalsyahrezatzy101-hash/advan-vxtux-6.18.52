// mbox_phy_outbox_has_irq @ 00103de4
// signature: undefined mbox_phy_outbox_has_irq(void)
// calling convention: unknown


uint mbox_phy_outbox_has_irq(void)

{
  DataMemoryBarrier(0,1);
  return *(uint *)(sprd_outbox_base + 0x18) & 1;
}


