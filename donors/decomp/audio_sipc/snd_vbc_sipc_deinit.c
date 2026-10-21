// signature: undefined snd_vbc_sipc_deinit(void)

void snd_vbc_sipc_deinit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = aud_smsg_ch_close(1,0);
  if (iVar1 != 0) {
    printk(&DAT_00101167,"aud_ipc_ch_close");
  }
  DAT_001009a0 = 0;
  return;
}



