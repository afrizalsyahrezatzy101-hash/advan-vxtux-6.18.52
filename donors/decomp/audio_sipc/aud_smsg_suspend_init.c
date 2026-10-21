// signature: undefined aud_smsg_suspend_init(void)

undefined8 aud_smsg_suspend_init(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  register_syscore_ops(&aud_smsg_syscore_ops);
  return 0;
}



