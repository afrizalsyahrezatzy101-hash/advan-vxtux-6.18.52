// signature: undefined aud_ipc_ch_close(void)

undefined8 aud_ipc_ch_close(undefined4 param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = aud_smsg_ch_close(1,param_1);
  if ((int)uVar1 != 0) {
    printk(&DAT_00101167,"aud_ipc_ch_close");
    uVar1 = 0xffffffff;
  }
  return uVar1;
}



