====================mmsys_sw_cgb_enable@0011607c====================
// signature: undefined mmsys_sw_cgb_enable(void)

void mmsys_sw_cgb_enable(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *DAT_00100c28 = *DAT_00100c28 & 0xffffff7f;
  }
  else {
    *DAT_00100c28 = *DAT_00100c28 | 0x80;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


