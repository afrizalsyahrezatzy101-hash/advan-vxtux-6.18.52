// sharkl5pro_vsp_val_to_freq @ 0010f724
// signature: undefined sharkl5pro_vsp_val_to_freq(void)
// calling convention: unknown


undefined * sharkl5pro_vsp_val_to_freq(uint param_1)

{
  undefined *puVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 3) {
    puVar1 = (&PTR_DAT_00105da8)[(int)param_1];
  }
  else {
    printk(&DAT_001035ed);
    puVar1 = &DAT_001023b6;
  }
  return puVar1;
}


