// sprd_mbox_disable @ 001027fc
// signature: undefined sprd_mbox_disable(void)
// calling convention: unknown


void sprd_mbox_disable(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (reg_ctrl_0 != 0) {
    regmap_update_bits_base(reg_ctrl_0,reg_ctrl_1,reg_ctrl_2,0,0,0,0);
    _dev_info(*(undefined8 *)(param_1 + 0x88),"mailbox disabled.\n");
  }
  return;
}


