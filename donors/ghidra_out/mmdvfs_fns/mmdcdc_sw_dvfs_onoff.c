====================mmdcdc_sw_dvfs_onoff@00115fec====================
// signature: undefined mmdcdc_sw_dvfs_onoff(void)

void mmdcdc_sw_dvfs_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x18,0x100000,(ulong)(param_1 != 0) << 0x14,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


