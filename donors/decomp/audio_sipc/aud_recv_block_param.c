// signature: undefined aud_recv_block_param(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void aud_recv_block_param
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined1 *param_6,uint param_7)

{
  undefined1 *puVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  undefined1 *__src;
  ulong __n;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_68;
  undefined8 local_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  local_68 = 0;
  local_60 = 0;
  lVar3 = aud_ipc_block_param_get(param_5);
  if ((lVar3 == 0) || (param_7 < *(uint *)(lVar3 + 0x10))) {
    uVar4 = 0xffffffff;
  }
  else {
    local_68 = CONCAT44(param_5,param_2);
    local_60 = CONCAT44(*(uint *)(lVar3 + 0x10),*(undefined4 *)(lVar3 + 4));
    iVar2 = aud_send_cmd(param_1,param_2,param_3,param_4,&local_68,0x10);
    if (iVar2 < 0) {
      printk(&DAT_00100dd6,"aud_recv_block_param",param_4,iVar2);
      uVar4 = 0xffffffff;
    }
    else {
      __src = *(undefined1 **)(lVar3 + 8);
      __n = (ulong)*(uint *)(lVar3 + 0x10);
      puVar1 = __src;
      if ((((uint)__src ^ (uint)param_6) & 7) == 0) {
        while (((ulong)puVar1 & 7) != 0) {
          if (__n == 0) {
            __n = 0xffffffffffffffff;
            break;
          }
          __n = __n - 1;
          *param_6 = *__src;
          __src = __src + 1;
          puVar1 = puVar1 + 1;
          param_6 = param_6 + 1;
        }
        memcpy(param_6,__src,__n);
      }
      else if (*(uint *)(lVar3 + 0x10) != 0) {
        do {
          __n = __n - 1;
          *param_6 = *__src;
          __src = __src + 1;
          param_6 = param_6 + 1;
        } while (__n != 0);
      }
      uVar4 = 0;
    }
  }
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}



