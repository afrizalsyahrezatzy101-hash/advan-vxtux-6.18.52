====================power_on_nb.1136@0011712c====================
// signature: undefined power_on_nb.1136(void)

void power_on_nb_1136(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mmsys_init();
  mm_power_flag = 1;
  return;
}


