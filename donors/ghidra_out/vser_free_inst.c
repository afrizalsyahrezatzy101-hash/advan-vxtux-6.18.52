// vser_free_inst @ 00102894
// signature: undefined vser_free_inst(void)
// calling convention: unknown


void vser_free_inst(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  kfree(*(undefined8 *)(param_1 + 0xb8));
  kfree(param_1);
  misc_deregister(&vser_device);
  kfree(_vser_dev);
  _vser_dev = 0;
  return;
}


