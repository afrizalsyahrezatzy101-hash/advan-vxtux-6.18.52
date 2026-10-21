====================power_on_nb@0010f5ec====================
// signature: undefined power_on_nb(void)

void power_on_nb(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init();
  return;
}


