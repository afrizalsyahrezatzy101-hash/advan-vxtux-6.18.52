// signature: undefined aud_smsg_init_debugfs(void)

undefined8 aud_smsg_init_debugfs(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    debugfs_create_file("audio-smsg-sipc",0x124,param_1,0,&aud_smsg_debug_fops);
    return 0;
  }
  return 0xfffffffa;
}



