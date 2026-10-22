====================updata_target_freq.850@00112b20====================
// signature: undefined updata_target_freq.850(void)

undefined8
updata_target_freq_850(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_834(param_1,param_3);
  }
  return 0;
}


