// signature: undefined aud_smsg_all_ch_notify(void)

void aud_smsg_all_ch_notify(long param_1,long param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = _raw_spin_lock_irqsave(param_1 + 0x70);
  *(undefined2 *)(param_2 + 2) = 0;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 1;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 2;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 3;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 4;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 5;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 6;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 7;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 8;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 9;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 10;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0xb;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0xc;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0xd;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0xe;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0xf;
  aud_smsg_ch_send(param_1,param_2);
  *(undefined2 *)(param_2 + 2) = 0x10;
  aud_smsg_ch_send(param_1,param_2);
  _raw_spin_unlock_irqrestore(param_1 + 0x70,uVar1);
  return;
}



