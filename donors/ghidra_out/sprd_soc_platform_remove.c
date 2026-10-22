// sprd_soc_platform_remove @ 0010210c
// signature: undefined sprd_soc_platform_remove(void)
// calling convention: unknown


undefined8 sprd_soc_platform_remove(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  snd_soc_unregister_component(param_1 + 0x10);
  unregister_pm_notifier(pm_dma + 0x10);
  return 0;
}


