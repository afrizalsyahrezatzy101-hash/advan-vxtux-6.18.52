// signature: undefined aud_smsg_ipc_create(void)

undefined8 aud_smsg_ipc_create(byte param_1,long param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msg_index = 0;
  if (*(long *)(param_2 + 0x58) == 0) {
    *(undefined1 **)(param_2 + 0x58) = &LAB_00107688;
  }
  *(undefined8 *)(param_2 + 0x68) = 0;
  *(undefined4 *)(param_2 + 0x70) = 0;
  *(undefined4 *)(param_2 + 0x364) = 0;
  __mutex_init(param_2 + 0x78,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x368) = 0;
  __mutex_init(param_2 + 0x98,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x36c) = 0;
  __mutex_init(param_2 + 0xb8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x370) = 0;
  __mutex_init(param_2 + 0xd8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x374) = 0;
  __mutex_init(param_2 + 0xf8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x378) = 0;
  __mutex_init(param_2 + 0x118,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x37c) = 0;
  __mutex_init(param_2 + 0x138,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x380) = 0;
  __mutex_init(param_2 + 0x158,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 900) = 0;
  __mutex_init(param_2 + 0x178,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x388) = 0;
  __mutex_init(param_2 + 0x198,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x38c) = 0;
  __mutex_init(param_2 + 0x1b8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x390) = 0;
  __mutex_init(param_2 + 0x1d8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x394) = 0;
  __mutex_init(param_2 + 0x1f8,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x398) = 0;
  __mutex_init(param_2 + 0x218,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x39c) = 0;
  __mutex_init(param_2 + 0x238,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x3a0) = 0;
  __mutex_init(param_2 + 600,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  *(undefined4 *)(param_2 + 0x3a4) = 0;
  __mutex_init(param_2 + 0x278,"&(ipc->channel_mutexlock[i])",&audio_sipc_driver);
  if (param_1 < 2) {
    (&aud_smsg_ipcs)[param_1] = param_2;
    *(undefined4 *)(param_2 + 0x3cc) = 1;
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



