// updata_vsp_target_freq.730 @ 00113640
// signature: undefined updata_vsp_target_freq.730(void)
// calling convention: unknown


void updata_vsp_target_freq_730(undefined8 param_1,undefined8 param_2,int param_3)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_3 == 0) {
    set_vsp_work_freq_715();
  }
  else {
    set_vsp_idle_freq_717();
  }
  return;
}


