// signature: undefined aud_ipc_ch_open(void)

int aud_ipc_ch_open(undefined4 param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = aud_smsg_ch_open(1,param_1);
  if (iVar1 != 0) {
    printk(&DAT_00100d12,"aud_ipc_ch_open",iVar1);
  }
  return iVar1;
}



