// signature: undefined aud_send_block_param(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void aud_send_block_param
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined1 *param_6,ulong param_7)

{
  undefined4 uVar1;
  ulong uVar2;
  undefined1 *puVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  undefined1 *__dest;
  size_t __n;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_68;
  undefined8 local_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  local_68 = 0;
  local_60 = 0;
  mutex_lock(&DAT_00100530);
  lVar5 = aud_ipc_block_param_get(param_5);
  if (lVar5 == 0) {
LAB_00103cf0:
    mutex_unlock(&DAT_00100530);
  }
  else {
    if (*(uint *)(lVar5 + 0x10) < param_7) {
      printk(&DAT_001019cd,"aud_send_block_param",(ulong)*(uint *)(lVar5 + 0x10),param_7);
      goto LAB_00103cf0;
    }
    __dest = *(undefined1 **)(lVar5 + 8);
    uVar2 = param_7;
    __n = param_7;
    puVar3 = param_6;
    if ((((uint)__dest ^ (uint)param_6) & 7) == 0) {
      while (((ulong)puVar3 & 7) != 0) {
        if (__n == 0) {
          __n = 0xffffffffffffffff;
          break;
        }
        *__dest = *param_6;
        __dest = __dest + 1;
        __n = __n - 1;
        param_6 = param_6 + 1;
        puVar3 = puVar3 + 1;
      }
      memcpy(__dest,param_6,__n);
    }
    else {
      for (; uVar2 != 0; uVar2 = uVar2 - 1) {
        *__dest = *param_6;
        __dest = __dest + 1;
        param_6 = param_6 + 1;
      }
    }
    local_68 = CONCAT44(param_5,param_2);
    uVar1 = *(undefined4 *)(lVar5 + 4);
    local_60 = CONCAT44((int)param_7,uVar1);
    printk(&DAT_00101483,param_4,param_2,param_5);
    printk(&DAT_00101860,uVar1,param_7 & 0xffffffff);
    iVar4 = aud_send_cmd(param_1,param_2,param_3,param_4,&local_68,0x10);
    mutex_unlock(&DAT_00100530);
    if (-1 < iVar4) {
      uVar6 = 0;
      goto LAB_00103d00;
    }
    printk(&DAT_00100dd6,"aud_send_block_param",param_4,iVar4);
  }
  uVar6 = 0xffffffff;
LAB_00103d00:
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}



