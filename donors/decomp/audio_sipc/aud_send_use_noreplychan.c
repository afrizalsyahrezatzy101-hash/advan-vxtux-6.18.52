// signature: undefined aud_send_use_noreplychan(void)

void aud_send_use_noreplychan
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_send_cmd_no_wait(7,param_1,param_2,param_3,param_4,param_5);
  return;
}



