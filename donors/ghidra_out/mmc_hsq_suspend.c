// mmc_hsq_suspend @ 00101030
// signature: undefined mmc_hsq_suspend(void)
// calling convention: unknown


void mmc_hsq_suspend(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mmc_hsq_disable();
  return;
}


