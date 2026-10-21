// signature: undefined aud_get_aud_ipc_smsg_para_addr(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 aud_get_aud_ipc_smsg_para_addr(long *param_1,long *param_2,int *param_3)

{
  int iVar1;
  
  *param_1 = _DAT_00100440;
  *param_2 = g_aud_ipc;
  iVar1 = DAT_00100448;
  *param_3 = DAT_00100448;
  if (((*param_2 != 0) && (iVar1 != 0)) && (*param_1 != 0)) {
    return 0;
  }
  return 0xfffffff4;
}



