// qogirn6pro_gsp_val_to_volt @ 0010ecf8
// signature: undefined qogirn6pro_gsp_val_to_volt(void)
// calling convention: unknown


undefined * qogirn6pro_gsp_val_to_volt(uint param_1)

{
  undefined *puVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 5) {
    puVar1 = (&PTR_s_0_55v_00105d30)[(int)param_1];
  }
  else {
    printk(&DAT_0010238e);
    puVar1 = &DAT_001023b6;
  }
  return puVar1;
}


