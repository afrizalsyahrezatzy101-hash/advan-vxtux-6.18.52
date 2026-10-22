====================mm_sw_dvfs_onoff@00115f5c====================
// signature: undefined mm_sw_dvfs_onoff(void)

void mm_sw_dvfs_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,(ulong)(param_1 != 0) << 2,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


