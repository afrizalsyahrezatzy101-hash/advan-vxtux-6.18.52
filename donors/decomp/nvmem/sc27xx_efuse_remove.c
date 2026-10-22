// signature: undefined sc27xx_efuse_remove(void)

undefined8 sc27xx_efuse_remove(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  hwspin_lock_free(*(undefined8 *)(*(long *)(param_1 + 0xa8) + 0x10));
  return 0;
}



