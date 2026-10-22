// sprd_mbox_phy_r1_init @ 00103424
// signature: undefined sprd_mbox_phy_r1_init(void)
// calling convention: unknown


ulong sprd_mbox_phy_r1_init(long param_1)

{
  int iVar1;
  ulong uVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(param_1 + 0x88) + -0x10;
  uVar2 = devm_platform_ioremap_resource(lVar3,0);
  *(ulong *)(param_1 + 0x90) = uVar2;
  if (uVar2 < 0xfffffffffffff001) {
    uVar2 = devm_platform_ioremap_resource(lVar3,1);
    *(ulong *)(param_1 + 0x98) = uVar2;
    if (uVar2 < 0xfffffffffffff001) {
      iVar1 = *(int *)(uVar2 + 0x24);
      DataMemoryBarrier(0,1);
      sprd_outbox_base = uVar2;
      *(undefined4 *)(param_1 + 0x18) = 8;
      *(int *)(param_1 + 0xb0) = iVar1 + 1;
      *(undefined4 *)(param_1 + 0xb4) = 0x1000;
      DataMemoryBarrier(0,1);
      *(uint *)(param_1 + 0xb8) = *(uint *)(*(long *)(param_1 + 0x90) + 0x28) & 0xffff;
      *(undefined ***)(param_1 + 0xe0) = &sprd_mbox_phy_ops_r1;
      uVar2 = register_pm_notifier(&mbox_pm_notifier_block_19);
      if ((int)uVar2 != 0) {
        _dev_err(*(undefined8 *)(param_1 + 0x88),"failed to register pm notifier\n");
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}


