// signature: undefined aud_get_aud_ipc_smsg_addr(void)

undefined8 aud_get_aud_ipc_smsg_addr(long *param_1,long *param_2,int *param_3)

{
  int iVar1;
  
  *param_1 = DAT_00100558;
  *param_2 = DAT_00100550;
  iVar1 = DAT_00100560;
  *param_3 = DAT_00100560;
  if (((*param_2 != 0) && (iVar1 != 0)) && (*param_1 != 0)) {
    return 0;
  }
  return 0xfffffff4;
}



