// sprd_pcm_platform_proc_read @ 00104e10
// signature: undefined sprd_pcm_platform_proc_read(void)
// calling convention: unknown


void sprd_pcm_platform_proc_read(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  seq_printf(*param_2,"test_ddr_buffer_size %#x\n",test_ddr_buffer_size);
  seq_printf(*param_2,"test_normal_cap_use_iram %#x\n",test_normal_cap_use_iram);
  seq_printf(*param_2,"debug_pointer_log %#x\n",init_pm_dma___key_3);
  return;
}


