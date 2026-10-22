====================__cfi_check_fail@00103000====================
// signature: undefined __cfi_check_fail(void)

void __cfi_check_fail(byte *param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((param_1 != (byte *)0x0) && (4 < *param_1)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(2);
}



====================aud_send_use_noreplychan@00103030====================
// signature: undefined aud_send_use_noreplychan(void)

void aud_send_use_noreplychan
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_send_cmd_no_wait(7,param_1,param_2,param_3,param_4,param_5);
  return;
}



====================aud_send_cmd_no_wait@00103064====================
// signature: undefined aud_send_cmd_no_wait(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
aud_send_cmd_no_wait
          (undefined2 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6)

{
  int iVar1;
  undefined8 *in_x18;
  undefined4 uVar2;
  undefined8 unaff_x30;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  printk(&DAT_00101ffc,"aud_send_cmd_no_wait");
  local_50 = 0;
  local_60 = 0;
  uStack_58 = 0;
  printk(&DAT_00100d71,"aud_send_msg",param_2,param_3,param_4);
  printk(&DAT_00100daa,param_5,param_6);
  local_60 = CONCAT44(param_3,CONCAT22(param_1,(short)param_2));
  uStack_58 = CONCAT44(param_5,param_4);
  local_50 = param_6;
  iVar1 = aud_smsg_send(1,&local_60);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    printk(&DAT_001013c1,"aud_send_msg",param_2,iVar1);
    uVar2 = 0xfffffffb;
    printk(&DAT_00102205,"aud_send_cmd_no_wait",param_1,param_2,0xfffffffb);
  }
  if (___stack_chk_guard == local_48) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================aud_send_cmd_result@00103194====================
// signature: undefined aud_send_cmd_result(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int aud_send_cmd_result(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                       undefined8 param_5,ulong param_6,undefined1 *param_7)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined4 uVar4;
  int iVar5;
  undefined *puVar6;
  undefined8 *in_x18;
  uint uVar7;
  undefined8 unaff_x30;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined4 local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined4 local_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  local_78 = 0;
  local_88 = 0;
  uStack_80 = 0;
  mutex_lock(&DAT_00100510);
  iVar5 = audio_cmd_copy(param_5,param_6);
  if (iVar5 < 0) {
    puVar6 = &DAT_001020da;
    iVar5 = -1;
  }
  else {
    uVar7 = 0;
    do {
      iVar5 = aud_recv_cmd(param_1,param_4,&local_88,0);
      uVar4 = DAT_00100440;
      if (0xfe < uVar7) break;
      uVar7 = uVar7 + 1;
    } while (iVar5 == 0);
    local_60 = 0;
    local_70 = 0;
    uStack_68 = 0;
    printk(&DAT_00100d71,"aud_send_msg",param_4,param_2,param_3);
    printk(&DAT_00100daa,uVar4,0);
    local_70 = CONCAT44(param_2,CONCAT22((short)param_1,(short)param_4));
    uStack_68 = CONCAT44(uVar4,param_3);
    local_60 = 0;
    iVar5 = aud_smsg_send(1,&local_70);
    if (iVar5 == 0) {
      uVar7 = 0;
      do {
        iVar5 = aud_recv_cmd(param_1,param_4,&local_88,0x2ee);
        uVar1 = uVar7 + 1;
        if (2 < uVar7) break;
        uVar7 = uVar1;
      } while (iVar5 < 0);
      if (iVar5 < 0) {
        puVar6 = &DAT_00100c20;
      }
      else {
        if (param_7 == (undefined1 *)0x0) {
LAB_00103378:
          printk(&DAT_001015d1,"aud_send_cmd_result",param_4,param_2,local_78,uVar1);
          iVar5 = 0;
          goto LAB_00103330;
        }
        if (g_aud_ipc == (undefined1 *)0x0) {
          printk(&DAT_00102017,"audio_cmd_copy_back");
        }
        else {
          if (param_6 <= DAT_00100448) {
            puVar2 = g_aud_ipc;
            puVar3 = g_aud_ipc;
            if ((((uint)g_aud_ipc ^ (uint)param_7) & 7) == 0) {
              while (((ulong)puVar3 & 7) != 0) {
                if (param_6 == 0) {
                  param_6 = 0xffffffffffffffff;
                  break;
                }
                param_6 = param_6 - 1;
                *param_7 = *puVar2;
                param_7 = param_7 + 1;
                puVar2 = puVar2 + 1;
                puVar3 = puVar3 + 1;
              }
              memcpy(param_7,puVar2,param_6);
            }
            else {
              for (; param_6 != 0; param_6 = param_6 - 1) {
                *param_7 = *puVar2;
                param_7 = param_7 + 1;
                puVar2 = puVar2 + 1;
              }
            }
            goto LAB_00103378;
          }
          printk(&DAT_00101d03,"audio_cmd_copy_back",param_6);
        }
        puVar6 = &DAT_00101826;
        iVar5 = -0x16;
      }
    }
    else {
      printk(&DAT_001013c1,"aud_send_msg",param_4,iVar5);
      puVar6 = &DAT_00100dd6;
      iVar5 = -5;
    }
  }
  printk(puVar6,"aud_send_cmd_result",param_4,iVar5);
LAB_00103330:
  mutex_unlock(&DAT_00100510);
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar5;
}



====================audio_cmd_copy@00103474====================
// signature: undefined audio_cmd_copy(void)

undefined8 audio_cmd_copy(undefined1 *param_1,ulong param_2)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (g_aud_ipc == (undefined1 *)0x0) {
    printk(&DAT_00102017,"audio_cmd_copy");
    uVar3 = 0xffffffff;
  }
  else if (DAT_00100448 < param_2) {
    printk(&DAT_00101d03,"audio_cmd_copy",param_2);
    uVar3 = 0xffffffff;
  }
  else {
    uVar4 = param_2;
    puVar1 = g_aud_ipc;
    puVar2 = param_1;
    if ((((uint)g_aud_ipc ^ (uint)param_1) & 7) == 0) {
      while (((ulong)puVar2 & 7) != 0) {
        if (uVar4 == 0) {
          uVar4 = 0xffffffffffffffff;
          break;
        }
        *puVar1 = *param_1;
        uVar4 = uVar4 - 1;
        param_1 = param_1 + 1;
        puVar2 = puVar2 + 1;
        puVar1 = puVar1 + 1;
      }
      memcpy(puVar1,param_1,uVar4);
    }
    else {
      for (; uVar4 != 0; uVar4 = uVar4 - 1) {
        *puVar1 = *param_1;
        param_1 = param_1 + 1;
        puVar1 = puVar1 + 1;
      }
    }
    printk(&DAT_00102247,"audio_cmd_copy",g_aud_ipc,param_2);
    uVar3 = 0;
  }
  return uVar3;
}



====================aud_recv_cmd@00103580====================
// signature: undefined aud_recv_cmd(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void aud_recv_cmd(short param_1,uint param_2,undefined8 *param_3,undefined4 param_4)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  ushort local_50;
  short sStack_4e;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_50 = (ushort)param_2;
  uStack_4c = 0;
  uStack_48 = 0;
  local_44 = 0;
  uStack_40 = 0;
  sStack_4e = param_1;
  iVar1 = aud_smsg_recv(1,&local_50,param_4);
  if (iVar1 < 0) {
    if (iVar1 == -0x3d) {
      printk(&DAT_0010195d,"aud_recv_cmd",param_1,param_2,0xffffffc3);
      uVar2 = 0xffffffc3;
      goto LAB_0010368c;
    }
    printk(&DAT_00102040,"aud_recv_cmd",param_1,param_2,iVar1);
  }
  else {
    printk(&DAT_00101ab3,"aud_recv_cmd",sStack_4e,local_50,uStack_4c,uStack_48);
    printk(&DAT_00101f76,local_44,uStack_40,param_4);
    if (((local_50 == param_2) && (param_3 != (undefined8 *)0x0)) && (sStack_4e == param_1)) {
      uVar2 = 0;
      *(undefined4 *)(param_3 + 2) = uStack_40;
      *param_3 = CONCAT44(uStack_4c,CONCAT22(sStack_4e,local_50));
      param_3[1] = CONCAT44(local_44,uStack_48);
      goto LAB_0010368c;
    }
    printk(&DAT_00102081,"aud_recv_cmd",param_2);
    printk(&DAT_00101e64,sStack_4e);
  }
  uVar2 = 0xfffffffb;
LAB_0010368c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}



====================aud_send_cmd_no_wait_param@001036e8====================
// signature: undefined aud_send_cmd_no_wait_param(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
aud_send_cmd_no_wait_param
          (undefined2 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined8 param_5,undefined8 param_6)

{
  int iVar1;
  undefined *puVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined4 uVar4;
  undefined8 unaff_x30;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  mutex_lock(&DAT_00100510);
  iVar1 = audio_cmd_copy(param_5,param_6);
  uVar4 = DAT_00100440;
  if (iVar1 < 0) {
    puVar2 = &DAT_001020da;
    uVar3 = 0xffffffff;
    uVar4 = 0xffffffff;
  }
  else {
    local_50 = 0;
    local_60 = 0;
    uStack_58 = 0;
    printk(&DAT_00100d71,"aud_send_msg",param_4,param_2,param_3);
    printk(&DAT_00100daa,uVar4,0);
    local_60 = CONCAT44(param_2,CONCAT22(param_1,(short)param_4));
    uStack_58 = CONCAT44(uVar4,param_3);
    local_50 = 0;
    iVar1 = aud_smsg_send(1,&local_60);
    if (iVar1 == 0) {
      printk(&DAT_00100bb3,"aud_send_cmd_no_wait_param",0x2e6);
      uVar4 = 0;
      goto LAB_00103820;
    }
    printk(&DAT_001013c1,"aud_send_msg",param_4,iVar1);
    puVar2 = &DAT_00100dd6;
    uVar3 = 0xfffffffb;
    uVar4 = 0xfffffffb;
  }
  printk(puVar2,"aud_send_cmd_no_wait_param",param_4,uVar3);
LAB_00103820:
  mutex_unlock(&DAT_00100510);
  if (___stack_chk_guard == local_48) {
    return uVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================aud_send_cmd_no_param@00103864====================
// signature: undefined aud_send_cmd_no_param(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void aud_send_cmd_no_param
               (uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  uint uVar1;
  int iVar2;
  undefined *puVar3;
  undefined8 uVar4;
  undefined8 *in_x18;
  uint uVar5;
  undefined8 unaff_x30;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined4 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined4 local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  local_88 = 0;
  local_98 = 0;
  uStack_90 = 0;
  do {
    iVar2 = aud_recv_cmd(param_1,param_2,&local_98,0);
  } while (iVar2 == 0);
  local_70 = 0;
  local_80 = 0;
  uStack_78 = 0;
  printk(&DAT_00100d71,"aud_send_msg",param_2,param_3,param_4);
  printk(&DAT_00100daa,param_5,param_6);
  local_80 = CONCAT44(param_3,CONCAT22((short)param_1,(short)param_2));
  uStack_78 = CONCAT44(param_5,param_4);
  local_70 = param_6;
  iVar2 = aud_smsg_send(1,&local_80);
  if (iVar2 == 0) {
    uVar5 = 0;
    do {
      iVar2 = aud_recv_cmd(param_1,param_2,&local_98,param_7);
      uVar1 = uVar5 + 1;
      printk(&DAT_00101ca6,"aud_send_cmd_no_param",param_1 & 0xffff,param_2,uVar1);
      if (2 < uVar5) break;
      uVar5 = uVar1;
    } while (iVar2 < 0);
    if (-1 < iVar2) {
      printk(&DAT_00101b3b,"aud_send_cmd_no_param",param_1 & 0xffff,param_2,local_88,uVar1);
      uVar4 = 0;
      goto LAB_00103a18;
    }
    puVar3 = &DAT_00101af9;
  }
  else {
    printk(&DAT_001013c1,"aud_send_msg",param_2,iVar2);
    puVar3 = &DAT_0010198b;
    iVar2 = -5;
  }
  printk(puVar3,"aud_send_cmd_no_param",param_1 & 0xffff,param_2,iVar2);
  uVar4 = 0xffffffff;
LAB_00103a18:
  if (___stack_chk_guard == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}



====================aud_send_cmd@00103a54====================
// signature: undefined aud_send_cmd(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int aud_send_cmd(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined8 param_5,undefined8 param_6)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined *puVar4;
  undefined8 *in_x18;
  uint uVar5;
  undefined8 unaff_x30;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined4 local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_68 = 0;
  local_78 = 0;
  uStack_70 = 0;
  mutex_lock(&DAT_00100510);
  iVar3 = audio_cmd_copy(param_5,param_6);
  if (iVar3 < 0) {
    puVar4 = &DAT_001020da;
    iVar3 = -1;
  }
  else {
    do {
      iVar3 = aud_recv_cmd(param_1,param_4,&local_78,0);
    } while (iVar3 == 0);
    printk(&DAT_0010109b,"aud_send_cmd",param_4,param_2,local_68);
    uVar2 = DAT_00100440;
    local_50 = 0;
    local_60 = 0;
    uStack_58 = 0;
    printk(&DAT_00100d71,"aud_send_msg",param_4,param_2,param_3);
    printk(&DAT_00100daa,uVar2,0);
    local_60 = CONCAT44(param_2,CONCAT22((short)param_1,(short)param_4));
    uStack_58 = CONCAT44(uVar2,param_3);
    local_50 = 0;
    iVar3 = aud_smsg_send(1,&local_60);
    if (iVar3 == 0) {
      printk(&DAT_00100bb3,"aud_send_cmd",0x2b6);
      uVar5 = 0;
      do {
        iVar3 = aud_recv_cmd(param_1,param_4,&local_78,0x2ee);
        uVar1 = uVar5 + 1;
        if (2 < uVar5) break;
        uVar5 = uVar1;
      } while (iVar3 < 0);
      if (-1 < iVar3) {
        mutex_unlock(&DAT_00100510);
        printk(&DAT_001015d1,"aud_send_cmd",param_4,param_2,local_68,uVar1);
        iVar3 = 0;
        goto LAB_00103c1c;
      }
      puVar4 = &DAT_00100c20;
    }
    else {
      printk(&DAT_001013c1,"aud_send_msg",param_4,iVar3);
      puVar4 = &DAT_00100dd6;
      iVar3 = -5;
    }
  }
  printk(puVar4,"aud_send_cmd",param_4,iVar3);
  mutex_unlock(&DAT_00100510);
LAB_00103c1c:
  if (___stack_chk_guard != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar3;
}



====================aud_send_block_param@00103c8c====================
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



====================aud_ipc_block_param_get@00103e64====================
// signature: undefined aud_ipc_block_param_get(void)

undefined4 * aud_ipc_block_param_get(uint param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 5) {
    iVar1 = *(int *)(&DAT_001026e0 + (long)(int)param_1 * 4);
    if ((((((DAT_00100450 != iVar1) || (puVar2 = &DAT_00100450, DAT_00100460 == 0)) &&
          ((DAT_00100468 != iVar1 || (puVar2 = &DAT_00100468, DAT_00100478 == 0)))) &&
         ((DAT_00100480 != iVar1 || (puVar2 = &DAT_00100480, DAT_00100490 == 0)))) &&
        ((DAT_00100498 != iVar1 || (puVar2 = &DAT_00100498, DAT_001004a8 == 0)))) &&
       ((((DAT_001004b0 != iVar1 || (puVar2 = &DAT_001004b0, DAT_001004c0 == 0)) &&
         ((DAT_001004c8 != iVar1 || (puVar2 = &DAT_001004c8, DAT_001004d8 == 0)))) &&
        ((DAT_001004e0 != iVar1 || (puVar2 = &DAT_001004e0, DAT_001004f0 == 0)))))) {
      puVar2 = (undefined4 *)0x0;
      if (DAT_00100508 != 0 && DAT_001004f8 == iVar1) {
        puVar2 = &DAT_001004f8;
      }
    }
  }
  else {
    printk(&DAT_00101a3c,"aud_ipc_block_param_get");
    puVar2 = (undefined4 *)0x0;
  }
  return puVar2;
}



====================aud_recv_block_param@00103f90====================
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



====================aud_ipc_dump@001040f4====================
// signature: undefined aud_ipc_dump(void)

uint aud_ipc_dump(undefined1 *param_1,uint param_2)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  ulong uVar5;
  undefined8 *in_x18;
  uint uVar6;
  long lVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar5 = (ulong)DAT_00100560;
  puVar3 = param_1;
  puVar4 = DAT_00100550;
  puVar2 = DAT_00100550;
  if ((((uint)DAT_00100550 ^ (uint)param_1) & 7) == 0) {
    while (((ulong)puVar2 & 7) != 0) {
      if (uVar5 == 0) {
        uVar5 = 0xffffffffffffffff;
        break;
      }
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
      puVar2 = puVar2 + 1;
    }
    memcpy(puVar3,puVar4,uVar5);
  }
  else {
    puVar3 = DAT_00100550;
    puVar4 = param_1;
    if (DAT_00100560 != 0) {
      do {
        uVar5 = uVar5 - 1;
        *puVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
      } while (uVar5 != 0);
    }
  }
  uVar6 = DAT_00100560;
  uVar5 = (ulong)DAT_00100560;
  *(undefined8 *)(param_1 + uVar5) = 0x5a5a5a5a5a5a5a5a;
  *(undefined8 *)((long)(param_1 + uVar5) + 8) = 0x5a5a5a5a5a5a5a5a;
  uVar5 = (ulong)DAT_00100448;
  puVar3 = param_1 + (uVar6 + 0x10);
  puVar4 = g_aud_ipc;
  puVar2 = g_aud_ipc;
  if ((((uint)g_aud_ipc ^ (uint)puVar3) & 7) == 0) {
    while (((ulong)puVar2 & 7) != 0) {
      if (uVar5 == 0) {
        uVar5 = 0xffffffffffffffff;
        break;
      }
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
      puVar2 = puVar2 + 1;
    }
    memcpy(puVar3,puVar4,uVar5);
  }
  else if (DAT_00100448 != 0) {
    do {
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    } while (uVar5 != 0);
  }
  uVar1 = DAT_00100448 + uVar6 + 0x10;
  lVar7 = 0;
  uVar6 = uVar1 + 0x10;
  *(undefined8 *)(param_1 + uVar1) = 0x5b5b5b5b5b5b5b5b;
  *(undefined8 *)((long)(param_1 + uVar1) + 8) = 0x5b5b5b5b5b5b5b5b;
  do {
    uVar1 = (&DAT_00100460)[lVar7 * 6];
    uVar5 = (ulong)uVar1;
    if ((uVar1 != 0) && (uVar1 + uVar6 < param_2)) {
      puVar4 = (undefined1 *)(&DAT_00100458)[lVar7 * 3];
      puVar3 = param_1 + uVar6;
      puVar2 = puVar4;
      if ((((uint)puVar4 ^ (uint)puVar3) & 7) == 0) {
        while (((ulong)puVar2 & 7) != 0) {
          if (uVar5 == 0) {
            uVar5 = 0xffffffffffffffff;
            break;
          }
          uVar5 = uVar5 - 1;
          *puVar3 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar2 = puVar2 + 1;
          puVar3 = puVar3 + 1;
        }
        memcpy(puVar3,puVar4,uVar5);
      }
      else {
        do {
          uVar5 = uVar5 - 1;
          *puVar3 = *puVar4;
          puVar3 = puVar3 + 1;
          puVar4 = puVar4 + 1;
        } while (uVar5 != 0);
      }
      uVar6 = (&DAT_00100460)[lVar7 * 6] + uVar6;
    }
    lVar7 = lVar7 + 1;
    if (lVar7 == 8) {
      if (uVar6 + 0x10 < param_2) {
        *(undefined8 *)(param_1 + uVar6) = 0x5c5c5c5c5c5c5c5c;
        *(undefined8 *)((long)(param_1 + uVar6) + 8) = 0x5c5c5c5c5c5c5c5c;
        uVar6 = uVar6 + 0x10;
      }
      return uVar6;
    }
  } while( true );
}



====================aud_ipc_ch_open@00104328====================
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



====================aud_ipc_ch_close@0010437c====================
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



====================aud_get_aud_ipc_smsg_para_addr@001043c0====================
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



====================aud_get_aud_ipc_smsg_addr@00104404====================
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



====================snd_vbc_sipc_deinit@00104448====================
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



====================audio_sipc_probe@00104490====================
// signature: undefined audio_sipc_probe(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void audio_sipc_probe(long param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  long lVar5;
  ulong uVar6;
  undefined8 uVar7;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  int local_5c;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  printk(&DAT_00101e91,"audio_sipc_probe");
  printk(&DAT_00100e22,"aud_ipc_init");
  __mutex_init(&DAT_00100530,"&aud_ipc->lock_block_param",&audio_sipc_create_s_sipc_inst);
  __mutex_init(&DAT_00100510,"&aud_ipc->lock",&audio_sipc_create_s_sipc_inst);
  local_5c = 0;
  uVar3 = audio_mem_alloc(7,&local_5c);
  if (uVar3 != 0) {
    lVar5 = audio_mem_vmap((ulong)uVar3,local_5c,0);
    if (lVar5 != 0) {
      DAT_00100560 = local_5c;
      DAT_00100550 = lVar5;
      DAT_00100558 = (ulong)uVar3;
      __memset_io(lVar5,0);
      uVar2 = local_5c - 0x10;
      uVar1 = uVar2 >> 1;
      uVar6 = (ulong)uVar1;
      printk(&DAT_00101d50,"audio_sipc_create",lVar5,uVar3,uVar6);
      DAT_001005b0 = 1;
      audio_sipc_create_s_sipc_inst = "ap-agdsp-ipc";
      DAT_001005c8 = uVar6 + lVar5 + uVar6;
      DAT_00100984 = uVar1 + uVar3;
      DAT_001005d0 = DAT_001005c8 + 4;
      DAT_0010097c = DAT_00100984 + uVar1;
      DAT_001005e8 = DAT_001005c8 + 8;
      DAT_001005f0 = DAT_001005c8 + 0xc;
      DAT_00100980 = DAT_0010097c + 4;
      DAT_00100988 = DAT_0010097c + 8;
      DAT_0010098c = DAT_0010097c + 0xc;
      DAT_001005c0 = uVar2 / 0x28;
      DAT_001005f8 = 0;
      DAT_001005b8 = lVar5;
      DAT_001005d8 = uVar6 + lVar5;
      DAT_001005e0 = DAT_001005c0;
      DAT_00100978 = uVar3;
      aud_smsg_ipc_create(1);
      printk(&DAT_00100c71,"audio_sipc_create");
      uVar3 = audio_mem_alloc(6,&DAT_00100448);
      _DAT_00100440 = (ulong)uVar3;
      if (uVar3 == 0) {
        printk(&DAT_0010112f,"aud_ipc_init",0x1d7);
        DAT_001009a0 = 0;
      }
      else {
        g_aud_ipc = audio_mem_vmap(_DAT_00100440,DAT_00100448,0);
        if (g_aud_ipc == 0) {
          printk(&DAT_00101a06,"aud_ipc_command_param_init");
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(0,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100450 = 0;
          DAT_00100460 = local_5c;
          DAT_00100454 = iVar4;
          DAT_00100458 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(1,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100468 = 1;
          DAT_00100478 = local_5c;
          DAT_0010046c = iVar4;
          DAT_00100470 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(2,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100480 = 2;
          DAT_00100490 = local_5c;
          DAT_00100484 = iVar4;
          DAT_00100488 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(3,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100498 = 3;
          DAT_001004a8 = local_5c;
          DAT_0010049c = iVar4;
          DAT_001004a0 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(4,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_001004c0 = local_5c;
          DAT_001004b0 = 4;
          DAT_001004b4 = iVar4;
          DAT_001004b8 = uVar7;
        }
      }
      goto LAB_0010457c;
    }
    printk(&DAT_00101a5f,"audio_sipc_create");
  }
  printk(&DAT_0010163e,"aud_ipc_init",0xffffffff);
LAB_0010457c:
  DAT_00100588 = &LAB_00107680;
  DAT_00100598 = 0;
  DAT_00100570 = 0;
  DAT_00100580 = 0;
  DAT_00100578 = 500;
  DAT_00100568 = param_1 + 0x10;
  DAT_001005a0 = mbox_request_channel(&DAT_00100568,0);
  if (DAT_001005a0 < 0xfffffffffffff001) {
    printk(&DAT_001011dc);
    aud_smsg_mboxchan = DAT_001005a0;
    agdsp_set_mboxchan();
    uVar6 = 0;
  }
  else {
    _dev_err(param_1 + 0x10,"mailbox request chan fail !!!");
    uVar6 = DAT_001005a0 & 0xffffffff;
  }
  if (___stack_chk_guard == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}



====================audio_sipc_remove@00104940====================
// signature: undefined audio_sipc_remove(void)

undefined8 audio_sipc_remove(void)

{
  return 0;
}



====================audio_sipc_rx_callback@00104948====================
// signature: undefined audio_sipc_rx_callback(void)

void audio_sipc_rx_callback(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_smsg_irq_handler(param_1,DAT_001009a0);
  return;
}



====================__cfi_check@00105000====================
// signature: undefined __cfi_check(void)

void __cfi_check(long param_1,undefined1 *param_2,undefined8 param_3)

{
  undefined1 *puVar1;
  
  if (param_1 < 0x7ff5a213d37b1a6) {
    if (param_1 < -0x25f41a7ab8fea45e) {
      if (param_1 < -0x631f31ee7603c3b1) {
        if (param_1 == -0x734b09deb920018a) {
          puVar1 = &LAB_00107694;
        }
        else if (param_1 == -0x6ef86220e8ff92f4) {
          puVar1 = &LAB_0010764c;
        }
        else {
          if (param_1 != -0x6b7d9f646c25cf62) goto LAB_0010550c;
          puVar1 = &LAB_00107690;
        }
      }
      else if (param_1 < -0x38162e00706dfb96) {
        if (param_1 == -0x631f31ee7603c3b1) {
          puVar1 = &LAB_00107680;
LAB_001054fc:
          if (((ulong)((long)param_2 - (long)puVar1) >> 2 | (long)param_2 << 0x3e) < 2) {
            return;
          }
          goto LAB_0010550c;
        }
        if (param_1 != -0x45ce73be2dfb584e) goto LAB_0010550c;
        puVar1 = &LAB_00107640;
      }
      else {
        if (param_1 != -0x38162e00706dfb96) {
          if (param_1 != -0x3557318fb8d07b71) goto LAB_0010550c;
          puVar1 = &LAB_00107668;
          goto LAB_001054fc;
        }
        puVar1 = &LAB_0010767c;
      }
    }
    else if (param_1 < -0x9fff7a1f5d276c3) {
      if (param_1 == -0x25f41a7ab8fea45e) {
        puVar1 = &LAB_00107650;
      }
      else if (param_1 == -0x158f0b0c250d73d0) {
        puVar1 = &LAB_00107648;
      }
      else {
        if (param_1 != -0xfaf8b6d0939286c) goto LAB_0010550c;
        puVar1 = &LAB_00107674;
      }
    }
    else if (param_1 < -0x8962c8020a87446) {
      if (param_1 != -0x9fff7a1f5d276c3) {
        if (param_1 != -0x8dc801903aca538) goto LAB_0010550c;
        puVar1 = (undefined1 *)0x1076ac;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107644;
    }
    else {
      if (param_1 != -0x8962c8020a87446) {
        if (param_1 != 0x2b3a43e29242445) goto LAB_0010550c;
        puVar1 = (undefined1 *)0x1076b8;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107678;
    }
  }
  else if (param_1 < 0x3c3507c0ff3a7aff) {
    if (param_1 < 0x28cd34a3b6815913) {
      if (param_1 == 0x7ff5a213d37b1a6) {
        puVar1 = &LAB_001076a8;
      }
      else {
        if (param_1 != 0xc185a8305be510f) {
          if (param_1 != 0x26b69e04af459c2a) goto LAB_0010550c;
          puVar1 = &LAB_00107658;
          goto LAB_001054fc;
        }
        puVar1 = &LAB_00107638;
      }
    }
    else if (param_1 < 0x34d1c2fe15049aef) {
      if (param_1 == 0x28cd34a3b6815913) {
        puVar1 = &LAB_00107634;
      }
      else {
        if (param_1 != 0x321969b888d4949e) goto LAB_0010550c;
        puVar1 = &LAB_001076a4;
      }
    }
    else {
      if (param_1 != 0x34d1c2fe15049aef) {
        if (param_1 != 0x3bf49544a6c9deff) goto LAB_0010550c;
        puVar1 = &LAB_0010769c;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107688;
    }
  }
  else if (param_1 < 0x57641507abf06d7d) {
    if (param_1 == 0x3c3507c0ff3a7aff) {
      puVar1 = &LAB_00107698;
    }
    else if (param_1 == 0x5233d5bc7887ae44) {
      puVar1 = &LAB_001076b4;
    }
    else {
      if (param_1 != 0x55e179eb2d73f26e) goto LAB_0010550c;
      puVar1 = &LAB_0010768c;
    }
  }
  else if (param_1 < 0x75c7cc3c6e221ec8) {
    if (param_1 == 0x57641507abf06d7d) {
      puVar1 = &LAB_0010763c;
    }
    else {
      if (param_1 != 0x5e77f5d343869bc9) goto LAB_0010550c;
      puVar1 = &LAB_00107654;
    }
  }
  else {
    if (param_1 == 0x75c7cc3c6e221ec8) {
      puVar1 = &LAB_00107660;
      goto LAB_001054fc;
    }
    if (param_1 != 0x7e04a0fb7ad8bcd5) goto LAB_0010550c;
    puVar1 = (undefined1 *)0x107670;
  }
  if (param_2 == puVar1) {
    return;
  }
LAB_0010550c:
  __cfi_check_fail(param_3);
  return;
}



====================aud_smsg_wakeup_ch@00105520====================
// signature: undefined aud_smsg_wakeup_ch(void)

undefined8 aud_smsg_wakeup_ch(byte param_1,ulong param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  ulong uVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 2) {
    lVar2 = (&aud_smsg_ipcs)[param_1];
    if (lVar2 == 0) {
      uVar1 = 0xffffffed;
    }
    else {
      uVar3 = 0;
      do {
        if (((param_2 & 0xff) == uVar3) && (*(char *)(lVar2 + 0x3a8 + uVar3) == '\x02')) {
          *(undefined1 *)(lVar2 + (param_2 & 0xff) + 0x3b9) = 1;
          __wake_up(*(undefined8 *)(lVar2 + (param_2 & 0xff) * 8 + 0x298),1,0,0);
        }
        uVar3 = uVar3 + 1;
      } while (uVar3 != 0x11);
      uVar1 = 0;
    }
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



====================aud_smsg_set_mboxchan@001055e4====================
// signature: undefined aud_smsg_set_mboxchan(void)

void aud_smsg_set_mboxchan(undefined8 param_1)

{
  aud_smsg_mboxchan = param_1;
  return;
}



====================aud_smsg_send@001055f0====================
// signature: undefined aud_smsg_send(void)

undefined4 aud_smsg_send(byte param_1,ushort *param_2)

{
  undefined8 uVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  undefined1 *puVar5;
  ushort *__src;
  size_t __n;
  ulong uVar7;
  ushort *puVar8;
  undefined8 *in_x18;
  undefined4 uVar9;
  undefined1 *__dest;
  long lVar10;
  undefined8 unaff_x30;
  ushort *puVar6;
  
  *in_x18 = unaff_x30;
  if (1 < param_1) {
LAB_00105910:
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  lVar10 = (&aud_smsg_ipcs)[param_1];
  printk(&DAT_00101cdc,"aud_smsg_send",param_1,param_2[1]);
  if (lVar10 == 0) {
    uVar9 = 0xffffffed;
  }
  else {
    uVar7 = (ulong)param_2[1];
    if (0x10 < uVar7) goto LAB_00105910;
    if (*(long *)(lVar10 + uVar7 * 8 + 0x298) == 0) {
      printk(&DAT_00102298,"aud_smsg_send",uVar7);
      uVar9 = 0xffffffed;
    }
    else if ((*(char *)(lVar10 + uVar7 + 0x3a8) == '\x02') && (*(int *)(lVar10 + 0x3cc) != 0)) {
      uVar4 = _raw_spin_lock_irqsave(lVar10 + 0x68);
      if ((uint)(**(int **)(lVar10 + 0x28) - **(int **)(lVar10 + 0x20)) < *(uint *)(lVar10 + 0x18))
      {
        if (*(int *)(lVar10 + 0x3cc) == 0) {
          printk(&DAT_00101faf,"aud_smsg_send");
          uVar9 = 0;
          uVar2 = msg_index;
        }
        else {
          __dest = (undefined1 *)
                   (*(long *)(lVar10 + 0x10) +
                   (ulong)(*(int *)(lVar10 + 0x18) - 1U & **(uint **)(lVar10 + 0x28)) * 0x14);
          if ((((uint)__dest ^ (uint)param_2) & 7) == 0) {
            if (((ulong)param_2 & 7) == 0) {
              __n = 0x14;
              __src = param_2;
            }
            else {
              puVar5 = __dest;
              puVar6 = param_2;
              puVar8 = param_2;
              do {
                puVar8 = (ushort *)((long)puVar8 + 1);
                __src = (ushort *)((long)puVar6 + 1);
                __dest = puVar5 + 1;
                *puVar5 = (char)*puVar6;
                puVar5 = __dest;
                puVar6 = __src;
              } while (((ulong)puVar8 & 7) != 0);
              __n = ((ulong)param_2 | 0xfffffffffffffff8) + 0x14;
            }
            memcpy(__dest,__src,__n);
          }
          else {
            *__dest = (char)*param_2;
            __dest[1] = *(undefined1 *)((long)param_2 + 1);
            __dest[2] = (char)param_2[1];
            __dest[3] = *(undefined1 *)((long)param_2 + 3);
            __dest[4] = (char)param_2[2];
            __dest[5] = *(undefined1 *)((long)param_2 + 5);
            __dest[6] = (char)param_2[3];
            __dest[7] = *(undefined1 *)((long)param_2 + 7);
            __dest[8] = (char)param_2[4];
            __dest[9] = *(undefined1 *)((long)param_2 + 9);
            __dest[10] = (char)param_2[5];
            __dest[0xb] = *(undefined1 *)((long)param_2 + 0xb);
            __dest[0xc] = (char)param_2[6];
            __dest[0xd] = *(undefined1 *)((long)param_2 + 0xd);
            __dest[0xe] = (char)param_2[7];
            __dest[0xf] = *(undefined1 *)((long)param_2 + 0xf);
            __dest[0x10] = (char)param_2[8];
            __dest[0x11] = *(undefined1 *)((long)param_2 + 0x11);
            __dest[0x12] = (char)param_2[9];
            __dest[0x13] = *(undefined1 *)((long)param_2 + 0x13);
          }
          **(int **)(lVar10 + 0x28) = **(int **)(lVar10 + 0x28) + 1;
          uVar1 = aud_smsg_mboxchan;
          if (0x13 < msg_index) goto LAB_00105910;
          *(ulong *)(&aud_smsg_send_msg_val + (ulong)msg_index * 8) =
               (ulong)*param_2 << 0x30 | (ulong)param_2[1] << 0x20 | (ulong)*(uint *)(param_2 + 2);
          iVar3 = mbox_send_message(uVar1);
          if (iVar3 < 0) {
            printk(&DAT_00101b95,"aud_smsg_send",iVar3);
          }
          mbox_chan_txdone(aud_smsg_mboxchan,0);
          uVar9 = 0;
          uVar2 = 0;
          if ((int)(msg_index + 1) < 0x14) {
            uVar2 = msg_index + 1;
          }
        }
      }
      else {
        printk(&DAT_00102117,"aud_smsg_send");
        uVar9 = 0xfffffff0;
        uVar2 = msg_index;
      }
      msg_index = uVar2;
      _raw_spin_unlock_irqrestore(lVar10 + 0x68,uVar4);
    }
    else {
      printk(&DAT_0010230a,"aud_smsg_send",uVar7);
      uVar9 = 0xffffffea;
    }
  }
  return uVar9;
}



====================aud_smsg_rx_handler@00105980====================
// signature: undefined aud_smsg_rx_handler(void)

void aud_smsg_rx_handler(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_smsg_irq_handler(param_1,DAT_001009a0);
  return;
}



====================aud_smsg_irq_handler@001059a4====================
// signature: undefined aud_smsg_irq_handler(void)

undefined8 aud_smsg_irq_handler(undefined8 param_1,long param_2)

{
  short sVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  short *psVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = _raw_spin_lock_irqsave(param_2 + 0x6c);
  if (**(int **)(param_2 + 0x48) != **(int **)(param_2 + 0x40)) {
    do {
      psVar3 = (short *)(*(long *)(param_2 + 0x30) +
                        (ulong)(*(int *)(param_2 + 0x38) - 1U & **(uint **)(param_2 + 0x40)) * 0x14)
      ;
      if (((*(int *)(param_2 + 0x3cc) == 0) && (*psVar3 == -0x4112)) &&
         (*(int *)(psVar3 + 2) == 0xbeee)) {
        sVar1 = psVar3[1];
        if (sVar1 != -0x4112) goto joined_r0x00105b54;
        printk(&DAT_00100e6e);
        *(undefined4 *)(param_2 + 0x3cc) = 1;
        aud_smsg_all_ch_notify(param_2,psVar3);
        **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
      }
      else {
        sVar1 = psVar3[1];
joined_r0x00105b54:
        if ((sVar1 == 2) && (*psVar3 == 0x25)) {
          *(undefined4 *)(param_2 + 0x3cc) = 0;
          printk(&DAT_001014d8,*(undefined4 *)(psVar3 + 2),*(undefined4 *)(psVar3 + 4),
                 *(undefined4 *)(psVar3 + 6),*(undefined4 *)(psVar3 + 8));
          aud_smsg_all_ch_notify(param_2,psVar3);
          **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
          agdsp_access_dumpreg();
          aud_smsg_dump_func(0);
          **(undefined4 **)(param_2 + 0x28) = 0;
          **(undefined4 **)(param_2 + 0x20) = 0;
          **(undefined4 **)(param_2 + 0x48) = 0;
          **(undefined4 **)(param_2 + 0x40) = 0;
        }
        else {
          if (*(int *)(param_2 + 0x3cc) != 0) {
            aud_smsg_ch_send(param_2,psVar3);
          }
          **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
        }
      }
    } while (**(int **)(param_2 + 0x48) != **(int **)(param_2 + 0x40));
  }
  _raw_spin_unlock_irqrestore(param_2 + 0x6c,uVar2);
  return 1;
}



====================aud_smsg_all_ch_notify@00105c30====================
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



====================aud_smsg_dump_func@00105dcc====================
// signature: undefined aud_smsg_dump_func(void)

void aud_smsg_dump_func(undefined4 param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  code *pcVar5;
  bool bVar6;
  undefined2 *puVar7;
  undefined8 *in_x18;
  int iVar8;
  uint uVar9;
  ulong uVar10;
  uint uVar11;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = DAT_001009a0;
  uVar2 = **(uint **)(DAT_001009a0 + 0x40);
  iVar8 = *(int *)(DAT_001009a0 + 0x3dc);
  uVar11 = *(int *)(DAT_001009a0 + 0x38) - 1;
  if ((uVar11 & uVar2) != 0) {
    uVar11 = (uVar11 & uVar2) - 1;
  }
  printk(&DAT_001023c5,"aud_smsg_rx_dump",iVar8,*(undefined4 *)(DAT_001009a0 + 0x3e0),
         *(undefined4 *)(DAT_001009a0 + 0x3e4));
  printk(&DAT_0010151a,"aud_smsg_rx_dump",**(undefined4 **)(lVar4 + 0x48),
         **(undefined4 **)(lVar4 + 0x40),*(undefined4 *)(lVar4 + 0x38),iVar8 + uVar11 * 0x14,uVar2,
         uVar11);
  uVar9 = *(uint *)(lVar4 + 0x38);
  if (0x45 < uVar9) {
    uVar9 = 0x46;
  }
  printk(&DAT_0010157b,uVar9);
  printk(&DAT_00100b80,uVar11,*(undefined4 *)(lVar4 + 0x38));
  uVar1 = uVar9;
  if (uVar11 + 1 <= uVar9) {
    uVar1 = uVar11 + 1;
  }
  if (uVar9 != uVar1) {
    iVar8 = uVar1 - uVar9;
    do {
      uVar9 = uVar9 - 1;
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x30) + (ulong)(uint)(iVar8 + *(int *)(lVar4 + 0x38)) * 0x14);
      printk(&DAT_00101670,"aud_smsg_rx_dump",uVar9,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      bVar6 = iVar8 != -1;
      iVar8 = iVar8 + 1;
    } while (bVar6);
  }
  if (uVar1 != 0) {
    uVar11 = uVar11 - uVar1;
    uVar10 = (ulong)uVar1;
    do {
      uVar11 = uVar11 + 1;
      uVar10 = uVar10 - 1;
      puVar7 = (undefined2 *)(*(long *)(lVar4 + 0x30) + (ulong)uVar11 * 0x14);
      printk(&DAT_00101d95,"aud_smsg_rx_dump",uVar10 & 0xffffffff,*puVar7,puVar7[1],
             *(undefined4 *)(puVar7 + 2),*(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),
             *(undefined4 *)(puVar7 + 8));
    } while (uVar10 != 0);
  }
  printk(&DAT_00101f1b,**(int **)(lVar4 + 0x48) - **(int **)(lVar4 + 0x40));
  if (**(uint **)(lVar4 + 0x48) != uVar2) {
    iVar8 = 0;
    do {
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x30) +
               (ulong)(*(int *)(lVar4 + 0x38) - 1U & uVar2 + iVar8) * 0x14);
      printk(&DAT_00100ef9,"aud_smsg_rx_dump",iVar8,**(undefined4 **)(lVar4 + 0x48),
             **(undefined4 **)(lVar4 + 0x40),uVar2 + iVar8,puVar7);
      printk(&DAT_00100c91,"aud_smsg_rx_dump",iVar8,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      iVar8 = iVar8 + 1;
    } while (uVar2 + iVar8 != **(int **)(lVar4 + 0x48));
  }
  lVar4 = DAT_001009a0;
  uVar2 = **(uint **)(DAT_001009a0 + 0x20);
  uVar11 = *(int *)(DAT_001009a0 + 0x18) - 1U & uVar2;
  if (uVar11 == 0) {
    uVar11 = *(uint *)(DAT_001009a0 + 0x38);
  }
  iVar8 = *(int *)(DAT_001009a0 + 0x3d0);
  iVar3 = uVar11 - 1;
  printk(&DAT_00101204,"aud_smsg_tx_dump",iVar8,*(undefined4 *)(DAT_001009a0 + 0x3d4),
         *(undefined4 *)(DAT_001009a0 + 0x3d8));
  printk(&DAT_00100e93,"aud_smsg_tx_dump",**(undefined4 **)(lVar4 + 0x28),
         **(undefined4 **)(lVar4 + 0x20),*(undefined4 *)(lVar4 + 0x18),iVar8 + iVar3 * 0x14,uVar2,
         iVar3);
  uVar9 = *(uint *)(lVar4 + 0x18);
  if (0x45 < uVar9) {
    uVar9 = 0x46;
  }
  printk(&DAT_00100bdf,uVar9);
  printk(&DAT_00100b80,iVar3,*(undefined4 *)(lVar4 + 0x18));
  uVar1 = uVar9;
  if (uVar11 <= uVar9) {
    uVar1 = uVar11;
  }
  if (uVar9 != uVar1) {
    iVar8 = uVar1 - uVar9;
    do {
      uVar9 = uVar9 - 1;
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x10) + (ulong)(uint)(iVar8 + *(int *)(lVar4 + 0x18)) * 0x14);
      printk(&DAT_00101778,"aud_smsg_tx_dump",uVar9,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      bVar6 = iVar8 != -1;
      iVar8 = iVar8 + 1;
    } while (bVar6);
  }
  if (uVar1 != 0) {
    uVar10 = (ulong)uVar1;
    uVar11 = uVar11 - uVar1;
    do {
      uVar10 = uVar10 - 1;
      puVar7 = (undefined2 *)(*(long *)(lVar4 + 0x10) + (ulong)uVar11 * 0x14);
      printk(&DAT_00100f52,"aud_smsg_tx_dump",uVar10 & 0xffffffff,*puVar7,puVar7[1],
             *(undefined4 *)(puVar7 + 2),*(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),
             *(undefined4 *)(puVar7 + 8));
      uVar11 = uVar11 + 1;
    } while (uVar10 != 0);
  }
  printk(&DAT_00101f1b,**(int **)(lVar4 + 0x28) - **(int **)(lVar4 + 0x20));
  if (**(uint **)(lVar4 + 0x28) != uVar2) {
    iVar8 = 0;
    do {
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x10) +
               (ulong)(*(int *)(lVar4 + 0x18) - 1U & uVar2 + iVar8) * 0x14);
      printk(&DAT_00100ef9,"aud_smsg_tx_dump",iVar8,**(undefined4 **)(lVar4 + 0x28),
             **(undefined4 **)(lVar4 + 0x20),uVar2 + iVar8,puVar7);
      printk(&DAT_00100c91,"aud_smsg_tx_dump",iVar8,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      iVar8 = iVar8 + 1;
    } while (uVar2 + iVar8 != **(int **)(lVar4 + 0x28));
  }
  pcVar5 = dump_func;
  if (dump_func != (code *)0x0) {
    __cfi_slowpath(0xccf68899c64718a6,dump_func);
    (*pcVar5)(dump_func_priv,param_1);
  }
  return;
}



====================aud_smsg_ch_send@001062c8====================
// signature: undefined aud_smsg_ch_send(void)

void aud_smsg_ch_send(long param_1,undefined1 *param_2)

{
  int *piVar1;
  uint *puVar2;
  char cVar3;
  bool bVar4;
  undefined1 *puVar5;
  undefined1 *__dest;
  undefined1 *__src;
  size_t __n;
  ulong uVar7;
  long lVar8;
  undefined1 *puVar9;
  long lVar10;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined1 *puVar6;
  
  *in_x18 = unaff_x30;
  if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
    piVar1 = (int *)(param_1 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar4) {
        *piVar1 = *piVar1 + 1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    uVar7 = (ulong)*(ushort *)(param_2 + 2);
    if (0x11 < uVar7) goto LAB_001064e0;
    if (*(int *)(param_1 + uVar7 * 4 + 0x364) != 0) {
      if (0x10 < *(ushort *)(param_2 + 2)) goto LAB_001064e0;
      if (*(char *)(param_1 + uVar7 + 0x3a8) == '\x02') {
        lVar8 = *(long *)(param_1 + uVar7 * 8 + 0x298);
        puVar2 = (uint *)(lVar8 + 0x38);
        if ((int)(*puVar2 - *(int *)(lVar8 + 0x3c)) < 0x100) {
          lVar10 = lVar8 + (ulong)(*puVar2 & 0xff) * 0x14;
          __dest = (undefined1 *)(lVar10 + 0x40);
          if ((((uint)__dest ^ (uint)param_2) & 7) == 0) {
            if (((ulong)param_2 & 7) == 0) {
              __n = 0x14;
              __src = param_2;
            }
            else {
              puVar5 = __dest;
              puVar6 = param_2;
              puVar9 = param_2;
              do {
                puVar9 = puVar9 + 1;
                __src = puVar6 + 1;
                __dest = puVar5 + 1;
                *puVar5 = *puVar6;
                puVar5 = __dest;
                puVar6 = __src;
              } while (((ulong)puVar9 & 7) != 0);
              __n = ((ulong)param_2 | 0xfffffffffffffff8) + 0x14;
            }
            memcpy(__dest,__src,__n);
          }
          else {
            lVar8 = lVar8 + (ulong)(*puVar2 & 0xff) * 0x14;
            *__dest = *param_2;
            *(undefined1 *)(lVar10 + 0x41) = param_2[1];
            *(undefined1 *)(lVar8 + 0x42) = param_2[2];
            *(undefined1 *)(lVar8 + 0x43) = param_2[3];
            *(undefined1 *)(lVar8 + 0x44) = param_2[4];
            *(undefined1 *)(lVar8 + 0x45) = param_2[5];
            *(undefined1 *)(lVar8 + 0x46) = param_2[6];
            *(undefined1 *)(lVar8 + 0x47) = param_2[7];
            *(undefined1 *)(lVar8 + 0x48) = param_2[8];
            *(undefined1 *)(lVar8 + 0x49) = param_2[9];
            *(undefined1 *)(lVar8 + 0x4a) = param_2[10];
            *(undefined1 *)(lVar8 + 0x4b) = param_2[0xb];
            *(undefined1 *)(lVar8 + 0x4c) = param_2[0xc];
            *(undefined1 *)(lVar8 + 0x4d) = param_2[0xd];
            *(undefined1 *)(lVar8 + 0x4e) = param_2[0xe];
            *(undefined1 *)(lVar8 + 0x4f) = param_2[0xf];
            *(undefined1 *)(lVar8 + 0x50) = param_2[0x10];
            *(undefined1 *)(lVar8 + 0x51) = param_2[0x11];
            *(undefined1 *)(lVar8 + 0x52) = param_2[0x12];
            *(undefined1 *)(lVar8 + 0x53) = param_2[0x13];
          }
          *puVar2 = *puVar2 + 1;
        }
        else {
          printk(&DAT_0010238e);
        }
        if (0x10 < (ulong)*(ushort *)(param_2 + 2)) {
LAB_001064e0:
                    /* WARNING: Subroutine does not return */
          __ubsan_handle_cfi_check_fail_abort(0x12);
        }
        __wake_up(*(undefined8 *)(param_1 + (ulong)*(ushort *)(param_2 + 2) * 8 + 0x298),1,0,0);
        uVar7 = (ulong)*(ushort *)(param_2 + 2);
        if (0x11 < uVar7) goto LAB_001064e0;
      }
    }
    piVar1 = (int *)(param_1 + uVar7 * 4 + 800);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar4) {
        *piVar1 = *piVar1 + -1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
  }
  return;
}



====================aud_smsg_register_dump_func@0010652c====================
// signature: undefined aud_smsg_register_dump_func(void)

undefined8 aud_smsg_register_dump_func(undefined8 param_1,undefined8 param_2)

{
  dump_func = param_1;
  dump_func_priv = param_2;
  return 0;
}



====================aud_smsg_recv@00106544====================
// signature: undefined aud_smsg_recv(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong aud_smsg_recv(byte param_1,undefined1 *param_2,uint param_3)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  bool bVar4;
  int iVar5;
  undefined *puVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined1 *__dest;
  undefined1 *__src;
  size_t __n;
  ulong uVar9;
  uint uVar10;
  long lVar11;
  long lVar12;
  undefined8 *in_x18;
  long lVar13;
  long lVar14;
  ulong uVar15;
  uint *puVar16;
  undefined8 unaff_x30;
  undefined8 local_80;
  undefined8 local_78;
  ulong local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  if (1 < param_1) goto LAB_00106c9c;
  lVar13 = (&aud_smsg_ipcs)[param_1];
  if (lVar13 == 0) {
    printk(&DAT_0010191b,"aud_smsg_recv");
    uVar9 = 0xffffffed;
    goto LAB_00106bf0;
  }
  if (0x11 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
  uVar15 = (ulong)param_3;
  piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
  Hint_Prefetch(piVar1,2,0,1);
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
    if (bVar4) {
      *piVar1 = *piVar1 + 1;
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  uVar9 = (ulong)*(ushort *)(param_2 + 2);
  if (0x10 < uVar9) goto LAB_00106c9c;
  lVar14 = *(long *)(lVar13 + uVar9 * 8 + 0x298);
  if (lVar14 == 0) {
    printk(&DAT_00101fd1,"aud_smsg_recv",uVar9);
    if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
      piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
      uVar9 = 0xffffffed;
      goto LAB_00106bf0;
    }
    goto LAB_00106c9c;
  }
  if (param_3 == 0) {
    iVar5 = mutex_trylock(lVar14 + 0x18);
    if (iVar5 == 0) {
      if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
        printk(&DAT_00101942,"aud_smsg_recv",
               *(undefined4 *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800));
        if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
          piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
          Hint_Prefetch(piVar1,2,0,1);
          do {
            cVar3 = '\x01';
            bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
            if (bVar4) {
              *piVar1 = *piVar1 + -1;
              cVar3 = ExclusiveMonitorsStatus();
            }
          } while (cVar3 != '\0');
          uVar9 = 0xfffffff0;
          goto LAB_00106bf0;
        }
      }
      goto LAB_00106c9c;
    }
    if (*(int *)(lVar14 + 0x38) == *(int *)(lVar14 + 0x3c)) {
      printk(&DAT_001021a9,"aud_smsg_recv");
      uVar9 = 0xffffffc3;
    }
    else {
      uVar9 = 0;
LAB_00106a70:
      puVar16 = (uint *)(lVar14 + 0x3c);
      uVar15 = (ulong)(*puVar16 & 0xff);
      lVar11 = lVar14 + uVar15 * 0x14;
      __src = (undefined1 *)(lVar11 + 0x40);
      if ((((uint)__src ^ (uint)param_2) & 7) == 0) {
        if (((ulong)__src & 7) == 0) {
          __n = 0x14;
          __dest = param_2;
        }
        else {
          lVar12 = lVar14 + uVar15 * 0x14;
          lVar11 = 0;
          do {
            iVar5 = (int)lVar11;
            param_2[lVar11] = *(undefined1 *)(lVar12 + 0x40 + lVar11);
            lVar11 = lVar11 + 1;
          } while (((int)lVar12 + 0x41 + iVar5 & 7U) != 0);
          __src = (undefined1 *)(lVar14 + uVar15 * 0x14 + lVar11 + 0x40);
          __n = 0x14 - lVar11;
          __dest = param_2 + lVar11;
        }
        memcpy(__dest,__src,__n);
      }
      else {
        lVar12 = lVar14 + uVar15 * 0x14;
        *param_2 = *__src;
        param_2[1] = *(undefined1 *)(lVar11 + 0x41);
        param_2[2] = *(undefined1 *)(lVar12 + 0x42);
        param_2[3] = *(undefined1 *)(lVar12 + 0x43);
        param_2[4] = *(undefined1 *)(lVar12 + 0x44);
        param_2[5] = *(undefined1 *)(lVar12 + 0x45);
        param_2[6] = *(undefined1 *)(lVar12 + 0x46);
        param_2[7] = *(undefined1 *)(lVar12 + 0x47);
        param_2[8] = *(undefined1 *)(lVar12 + 0x48);
        param_2[9] = *(undefined1 *)(lVar12 + 0x49);
        param_2[10] = *(undefined1 *)(lVar12 + 0x4a);
        param_2[0xb] = *(undefined1 *)(lVar12 + 0x4b);
        param_2[0xc] = *(undefined1 *)(lVar12 + 0x4c);
        param_2[0xd] = *(undefined1 *)(lVar12 + 0x4d);
        param_2[0xe] = *(undefined1 *)(lVar12 + 0x4e);
        param_2[0xf] = *(undefined1 *)(lVar12 + 0x4f);
        param_2[0x10] = *(undefined1 *)(lVar12 + 0x50);
        param_2[0x11] = *(undefined1 *)(lVar12 + 0x51);
        param_2[0x12] = *(undefined1 *)(lVar12 + 0x52);
        param_2[0x13] = *(undefined1 *)(lVar12 + 0x53);
      }
      *puVar16 = *puVar16 + 1;
    }
  }
  else {
    mutex_lock();
    piVar1 = (int *)(lVar14 + 0x38);
    piVar2 = (int *)(lVar14 + 0x3c);
    if ((int)param_3 < 0) {
      if (*piVar1 == *piVar2) {
        uVar9 = (ulong)*(ushort *)(param_2 + 2);
        if (0x10 < uVar9) goto LAB_00106c9c;
        if (((*(char *)(lVar13 + uVar9 + 0x3a8) != '\x03') && (*(int *)(lVar13 + 0x3cc) != 0)) &&
           (*(char *)(lVar13 + uVar9 + 0x3b9) == '\0')) {
          local_68 = 0;
          uStack_60 = 0;
          local_78 = 0;
          local_70 = 0;
          local_80 = 0;
          init_wait_entry(&local_80,0);
          uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
          if (*piVar1 == *piVar2) {
            do {
              uVar15 = (ulong)*(ushort *)(param_2 + 2);
              if (0x10 < uVar15) goto LAB_00106c9c;
              if (((*(char *)(lVar13 + uVar15 + 0x3a8) == '\x03') || (*(int *)(lVar13 + 0x3cc) == 0)
                  ) || (*(char *)(lVar13 + uVar15 + 0x3b9) != '\0')) break;
              if (uVar9 != 0) {
                if (-1 < (int)uVar9) goto LAB_00106820;
                puVar6 = &DAT_00101337;
                goto LAB_001068ec;
              }
              schedule();
              uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
            } while (*piVar1 == *piVar2);
          }
          finish_wait(lVar14,&local_80);
        }
      }
      uVar9 = 0;
LAB_00106820:
      uVar15 = (ulong)*(ushort *)(param_2 + 2);
      if (0x10 < uVar15) goto LAB_00106c9c;
      if (*(char *)(lVar13 + uVar15 + 0x3a8) != '\x03') {
        if (*piVar1 != *piVar2) goto LAB_00106a70;
        uVar15 = (ulong)*(ushort *)(param_2 + 2);
      }
      printk(&DAT_00101258,"aud_smsg_recv",uVar15,*(undefined4 *)(lVar13 + 0x3cc));
      uVar10 = 0xffffffe0;
      if (*(int *)(lVar13 + 0x3cc) != 0) {
        uVar10 = 0xfffffffb;
      }
      uVar9 = (ulong)uVar10;
    }
    else {
      uVar9 = uVar15;
      if (*piVar1 == *piVar2) {
        if (0x10 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
        if ((*(char *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3a8) != '\x03') &&
           (*(int *)(lVar13 + 0x3cc) != 0)) {
          local_68 = 0;
          uStack_60 = 0;
          local_78 = 0;
          local_70 = 0;
          local_80 = 0;
          init_wait_entry(&local_80,0);
          uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
          if (*piVar1 == *piVar2) {
            while( true ) {
              if (0x10 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
              if (*(char *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3a8) == '\x03') break;
              if (*(int *)(lVar13 + 0x3cc) == 0 && uVar15 == 0) {
                uVar15 = 1;
              }
              if ((*(int *)(lVar13 + 0x3cc) == 0) || (uVar15 == 0)) goto LAB_001066cc;
              if (uVar9 != 0) goto LAB_001066e0;
              uVar15 = schedule_timeout(uVar15);
              uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
              if (*piVar1 != *piVar2) break;
            }
          }
          if (uVar15 == 0) {
            uVar15 = 1;
          }
LAB_001066cc:
          finish_wait(lVar14,&local_80);
          uVar9 = uVar15;
        }
      }
LAB_001066e0:
      if ((int)uVar9 < 0) {
        puVar6 = &DAT_00101456;
LAB_001068ec:
        printk(puVar6,"aud_smsg_recv");
      }
      else if ((int)uVar9 == 0) {
        local_70 = local_70 & 0xffffffff00000000;
        local_80 = 0;
        local_78 = 0;
        printk(&DAT_00101ec5,"aud_smsg_recv");
        agdsp_access_dumpreg();
        printk(&DAT_001022d6);
        uVar7 = _raw_spin_lock_irqsave(lVar13 + 0x68);
        uVar8 = _raw_spin_lock_irqsave(lVar13 + 0x6c);
        *(undefined4 *)(lVar13 + 0x3cc) = 0;
        aud_smsg_dump_func(1);
        **(undefined4 **)(lVar13 + 0x28) = 0;
        **(undefined4 **)(lVar13 + 0x20) = 0;
        **(undefined4 **)(lVar13 + 0x48) = 0;
        **(undefined4 **)(lVar13 + 0x40) = 0;
        local_80 = 0x26000b0026;
        aud_smsg_ch_send(lVar13,&local_80);
        local_80._0_4_ = CONCAT22(2,(undefined2)local_80);
        aud_smsg_ch_send(lVar13,&local_80);
        _raw_spin_unlock_irqrestore(lVar13 + 0x6c,uVar8);
        _raw_spin_unlock_irqrestore(lVar13 + 0x68,uVar7);
        lVar11 = 0;
        do {
          if (*(char *)(lVar13 + 0x298 + lVar11 + 0x110) == '\x02') {
            __wake_up(*(undefined8 *)(lVar13 + 0x298 + lVar11 * 8),1,0,0);
          }
          lVar11 = lVar11 + 1;
        } while (lVar11 != 0x11);
        uVar9 = 0xffffffc2;
      }
      else {
        uVar15 = (ulong)*(ushort *)(param_2 + 2);
        if (0x10 < uVar15) goto LAB_00106c9c;
        if (*(char *)(lVar13 + uVar15 + 0x3a8) != '\x03') {
          if (*piVar1 != *piVar2) goto LAB_00106a70;
          uVar15 = (ulong)*(ushort *)(param_2 + 2);
        }
        printk(&DAT_00101700,"aud_smsg_recv",*(undefined4 *)(lVar13 + 0x3cc),uVar15);
        uVar9 = 0xfffffffb;
      }
    }
  }
  if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
    *(undefined1 *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3b9) = 0;
    mutex_unlock(lVar14 + 0x18);
    if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
      piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
LAB_00106bf0:
      if (___stack_chk_guard == local_58) {
        return uVar9 & 0xffffffff;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
  }
LAB_00106c9c:
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



====================aud_smsg_ipc_destroy@00106d98====================
// signature: undefined aud_smsg_ipc_destroy(void)

undefined8 aud_smsg_ipc_destroy(byte param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 2) {
    (&aud_smsg_ipcs)[param_1] = 0;
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



====================aud_smsg_ipc_create@00106dd8====================
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



====================aud_smsg_ch_open@00106fc0====================
// signature: undefined aud_smsg_ch_open(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 aud_smsg_ch_open(byte param_1,ushort param_2)

{
  long lVar1;
  int *piVar2;
  long lVar3;
  char cVar4;
  bool bVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 *in_x18;
  long lVar8;
  ulong uVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  if (param_2 < 0x11) {
    lVar8 = (&aud_smsg_ipcs)[param_1];
    if (lVar8 == 0) {
      printk(&DAT_00101a91,"aud_smsg_ch_open");
      uVar6 = 0xfffffdfb;
    }
    else {
      uVar9 = (ulong)param_2;
      lVar1 = lVar8 + uVar9 * 0x20 + 0x78;
      mutex_lock(lVar1);
      if (*(char *)(lVar8 + uVar9 + 0x3a8) != '\x02') {
        lVar7 = kmem_cache_alloc_trace(_DAT_00109198,0xdc0,0x1440);
        if (lVar7 == 0) {
          mutex_unlock(lVar1);
          return 0xfffffff4;
        }
        lVar3 = lVar8 + uVar9 * 4;
        piVar2 = (int *)(lVar3 + 800);
        *(undefined4 *)(lVar3 + 800) = 1;
        __init_waitqueue_head(lVar7,"&(ch->rxwait)",&audio_sipc_driver);
        __mutex_init(lVar7 + 0x18,"&(ch->rxlock)",&audio_sipc_driver);
        *(long *)(lVar8 + uVar9 * 8 + 0x298) = lVar7;
        *(undefined1 *)(lVar8 + uVar9 + 0x3a8) = 2;
        Hint_Prefetch(piVar2,2,0,1);
        do {
          cVar4 = '\x01';
          bVar5 = (bool)ExclusiveMonitorPass(piVar2,0x10);
          if (bVar5) {
            *piVar2 = *piVar2 + -1;
            cVar4 = ExclusiveMonitorsStatus();
          }
        } while (cVar4 != '\0');
      }
      piVar2 = (int *)(lVar8 + uVar9 * 4 + 0x364);
      Hint_Prefetch(piVar2,2,0,1);
      do {
        cVar4 = '\x01';
        bVar5 = (bool)ExclusiveMonitorPass(piVar2,0x10);
        if (bVar5) {
          *piVar2 = *piVar2 + 1;
          cVar4 = ExclusiveMonitorsStatus();
        }
      } while (cVar4 != '\0');
      mutex_unlock(lVar1);
      uVar6 = 0;
    }
  }
  else {
    uVar6 = 0xffffffff;
  }
  return uVar6;
}



====================aud_smsg_ch_close@00107164====================
// signature: undefined aud_smsg_ch_close(void)

undefined8 aud_smsg_ch_close(byte param_1,ushort param_2)

{
  long lVar1;
  long lVar2;
  int *piVar3;
  long *plVar4;
  int iVar5;
  char cVar6;
  bool bVar7;
  undefined8 uVar8;
  undefined8 *in_x18;
  long lVar9;
  long lVar10;
  uint uVar11;
  ulong uVar12;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 < 0x11) {
    if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
      __ubsan_handle_cfi_check_fail_abort(0x12);
    }
    lVar10 = (&aud_smsg_ipcs)[param_1];
    if (lVar10 != 0) {
      lVar1 = lVar10 + (ulong)param_2 * 8;
      lVar9 = *(long *)(lVar1 + 0x298);
      if (lVar9 != 0) {
        uVar12 = (ulong)param_2;
        lVar2 = lVar10 + uVar12 * 0x20 + 0x78;
        mutex_lock(lVar2);
        piVar3 = (int *)(lVar10 + uVar12 * 4 + 0x364);
        Hint_Prefetch(piVar3,2,0,1);
        do {
          iVar5 = *piVar3;
          cVar6 = '\x01';
          bVar7 = (bool)ExclusiveMonitorPass(piVar3,0x10);
          if (bVar7) {
            *piVar3 = iVar5 + -1;
            cVar6 = ExclusiveMonitorsStatus();
          }
        } while (cVar6 != '\0');
        DataMemoryBarrier(2,3);
        if (iVar5 + -1 == 0) {
          plVar4 = (long *)(lVar1 + 0x298);
          *(undefined1 *)(lVar10 + uVar12 + 0x3a8) = 3;
          if (*plVar4 != 0) {
            lVar1 = lVar10 + uVar12 * 4;
            if (*(int *)(lVar1 + 800) != 0) {
              uVar11 = 0;
              do {
                printk(&DAT_001021d4,param_2);
                __wake_up(lVar9,3,1,0);
                msleep(1000);
                if (0x62 < uVar11) break;
                uVar11 = uVar11 + 1;
              } while (*(int *)(lVar1 + 800) != 0);
            }
            kfree(lVar9);
            *plVar4 = 0;
          }
          *(undefined1 *)(lVar10 + uVar12 + 0x3a8) = 0;
        }
        mutex_unlock(lVar2);
      }
    }
    uVar8 = 0;
  }
  else {
    uVar8 = 0xffffffff;
  }
  return uVar8;
}



====================aud_smsg_wakeup_all_ch@001072d8====================
// signature: undefined aud_smsg_wakeup_all_ch(void)

undefined8 aud_smsg_wakeup_all_ch(long param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = 0;
  do {
    if (*(char *)(param_1 + 0x298 + lVar1 + 0x110) == '\x02') {
      __wake_up(*(undefined8 *)(param_1 + 0x298 + lVar1 * 8),1,0,0);
    }
    lVar1 = lVar1 + 1;
  } while (lVar1 != 0x11);
  return 0;
}



====================aud_smsg_init_debugfs@0010733c====================
// signature: undefined aud_smsg_init_debugfs(void)

undefined8 aud_smsg_init_debugfs(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    debugfs_create_file("audio-smsg-sipc",0x124,param_1,0,&aud_smsg_debug_fops);
    return 0;
  }
  return 0xfffffffa;
}



====================aud_smsg_debug_open@00107384====================
// signature: undefined aud_smsg_debug_open(void)

void aud_smsg_debug_open(long param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  single_open(param_2,&LAB_001076b4,*(undefined8 *)(param_1 + 0x280));
  return;
}



====================aud_smsg_debug_show@001073b0====================
// signature: undefined aud_smsg_debug_show(void)

undefined8 aud_smsg_debug_show(undefined8 param_1)

{
  bool bVar1;
  bool bVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 *puVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  bVar2 = false;
  do {
    puVar4 = (undefined8 *)(&aud_smsg_ipcs)[lVar3];
    if (puVar4 != (undefined8 *)0x0) {
      seq_printf(param_1,"sipc: %s:\n",*puVar4);
      seq_printf(param_1,"dst: 0x%0x\n",*(undefined1 *)(puVar4 + 1));
      seq_printf(param_1,
                 "txbufAddr: 0x%0x, txbufsize: 0x%0x,txbufrdptr: [0x%0x]=%u, txbufwrptr: [0x%0x]=%u\n"
                 ,*(undefined4 *)(puVar4 + 2),*(undefined4 *)(puVar4 + 3),(undefined4 *)puVar4[4],
                 *(undefined4 *)puVar4[4],(undefined4 *)puVar4[5],*(undefined4 *)puVar4[5]);
      seq_printf(param_1,
                 "rxbufAddr: 0x%0x, rxbufsize: 0x%0x,rxbufrdptr: [0x%0x]=%u, rxbufwrptr: [0x%0x]=%u\n"
                 ,*(undefined4 *)(puVar4 + 6),*(undefined4 *)(puVar4 + 7),(undefined4 *)puVar4[8],
                 *(undefined4 *)puVar4[8],(undefined4 *)puVar4[9],*(undefined4 *)puVar4[9]);
      seq_printf(param_1,"channel[%d] states: %d\n",0,*(undefined1 *)(puVar4 + 0x75));
      seq_printf(param_1,"channel[%d] states: %d\n",1,*(undefined1 *)((long)puVar4 + 0x3a9));
      seq_printf(param_1,"channel[%d] states: %d\n",2,*(undefined1 *)((long)puVar4 + 0x3aa));
      seq_printf(param_1,"channel[%d] states: %d\n",3,*(undefined1 *)((long)puVar4 + 0x3ab));
      seq_printf(param_1,"channel[%d] states: %d\n",4,*(undefined1 *)((long)puVar4 + 0x3ac));
      seq_printf(param_1,"channel[%d] states: %d\n",5,*(undefined1 *)((long)puVar4 + 0x3ad));
      seq_printf(param_1,"channel[%d] states: %d\n",6,*(undefined1 *)((long)puVar4 + 0x3ae));
      seq_printf(param_1,"channel[%d] states: %d\n",7,*(undefined1 *)((long)puVar4 + 0x3af));
      seq_printf(param_1,"channel[%d] states: %d\n",8,*(undefined1 *)(puVar4 + 0x76));
      seq_printf(param_1,"channel[%d] states: %d\n",9,*(undefined1 *)((long)puVar4 + 0x3b1));
      seq_printf(param_1,"channel[%d] states: %d\n",10,*(undefined1 *)((long)puVar4 + 0x3b2));
      seq_printf(param_1,"channel[%d] states: %d\n",0xb,*(undefined1 *)((long)puVar4 + 0x3b3));
      seq_printf(param_1,"channel[%d] states: %d\n",0xc,*(undefined1 *)((long)puVar4 + 0x3b4));
      seq_printf(param_1,"channel[%d] states: %d\n",0xd,*(undefined1 *)((long)puVar4 + 0x3b5));
      seq_printf(param_1,"channel[%d] states: %d\n",0xe,*(undefined1 *)((long)puVar4 + 0x3b6));
      seq_printf(param_1,"channel[%d] states: %d\n",0xf,*(undefined1 *)((long)puVar4 + 0x3b7));
      seq_printf(param_1,"channel[%d] states: %d\n",0x10,*(undefined1 *)(puVar4 + 0x77));
    }
    lVar3 = 1;
    bVar1 = !bVar2;
    bVar2 = true;
  } while (bVar1);
  return 0;
}



====================aud_smsg_suspend_init@00107604====================
// signature: undefined aud_smsg_suspend_init(void)

undefined8 aud_smsg_suspend_init(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  register_syscore_ops(&aud_smsg_syscore_ops);
  return 0;
}



====================aud_smsg_syscore_suspend@0010762c====================
// signature: undefined aud_smsg_syscore_suspend(void)

undefined8 aud_smsg_syscore_suspend(void)

{
  return 0;
}



====================init_module@001076c0====================
// signature: undefined init_module(void)

void init_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  __platform_driver_register(&audio_sipc_driver,&__this_module);
  return;
}



====================cleanup_module@001076ec====================
// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&audio_sipc_driver);
  return;
}



====================printk@00109000====================
// signature: undefined printk(void)

/* WARNING: Control flow encountered bad instruction data */

void printk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_lock@00109008====================
// signature: undefined mutex_lock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_lock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_unlock@00109010====================
// signature: undefined mutex_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__platform_driver_register@00109018====================
// signature: undefined __platform_driver_register(void)

/* WARNING: Control flow encountered bad instruction data */

void __platform_driver_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mbox_request_channel@00109020====================
// signature: undefined mbox_request_channel(void)

/* WARNING: Control flow encountered bad instruction data */

void mbox_request_channel(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_dev_err@00109028====================
// signature: undefined _dev_err(void)

/* WARNING: Control flow encountered bad instruction data */

void _dev_err(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================agdsp_set_mboxchan@00109030====================
// signature: undefined agdsp_set_mboxchan(void)

/* WARNING: Control flow encountered bad instruction data */

void agdsp_set_mboxchan(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__mutex_init@00109038====================
// signature: undefined __mutex_init(void)

/* WARNING: Control flow encountered bad instruction data */

void __mutex_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================audio_mem_alloc@00109040====================
// signature: undefined audio_mem_alloc(void)

/* WARNING: Control flow encountered bad instruction data */

void audio_mem_alloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================audio_mem_vmap@00109048====================
// signature: undefined audio_mem_vmap(void)

/* WARNING: Control flow encountered bad instruction data */

void audio_mem_vmap(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__memset_io@00109050====================
// signature: undefined __memset_io(void)

/* WARNING: Control flow encountered bad instruction data */

void __memset_io(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================platform_driver_unregister@00109058====================
// signature: undefined platform_driver_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void platform_driver_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_raw_spin_lock_irqsave@00109068====================
// signature: undefined _raw_spin_lock_irqsave(void)

/* WARNING: Control flow encountered bad instruction data */

void _raw_spin_lock_irqsave(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================agdsp_access_dumpreg@00109070====================
// signature: undefined agdsp_access_dumpreg(void)

/* WARNING: Control flow encountered bad instruction data */

void agdsp_access_dumpreg(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__init_waitqueue_head@00109078====================
// signature: undefined __init_waitqueue_head(void)

/* WARNING: Control flow encountered bad instruction data */

void __init_waitqueue_head(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__wake_up@00109080====================
// signature: undefined __wake_up(void)

/* WARNING: Control flow encountered bad instruction data */

void __wake_up(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================msleep@00109088====================
// signature: undefined msleep(void)

/* WARNING: Control flow encountered bad instruction data */

void msleep(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================kfree@00109090====================
// signature: undefined kfree(void)

/* WARNING: Control flow encountered bad instruction data */

void kfree(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mbox_send_message@00109098====================
// signature: undefined mbox_send_message(void)

/* WARNING: Control flow encountered bad instruction data */

void mbox_send_message(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mbox_chan_txdone@001090a0====================
// signature: undefined mbox_chan_txdone(void)

/* WARNING: Control flow encountered bad instruction data */

void mbox_chan_txdone(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_trylock@001090a8====================
// signature: undefined mutex_trylock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_trylock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================init_wait_entry@001090b0====================
// signature: undefined init_wait_entry(void)

/* WARNING: Control flow encountered bad instruction data */

void init_wait_entry(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================prepare_to_wait_event@001090b8====================
// signature: undefined prepare_to_wait_event(void)

/* WARNING: Control flow encountered bad instruction data */

void prepare_to_wait_event(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================schedule@001090c0====================
// signature: undefined schedule(void)

/* WARNING: Control flow encountered bad instruction data */

void schedule(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================finish_wait@001090c8====================
// signature: undefined finish_wait(void)

/* WARNING: Control flow encountered bad instruction data */

void finish_wait(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================schedule_timeout@001090d0====================
// signature: undefined schedule_timeout(void)

/* WARNING: Control flow encountered bad instruction data */

void schedule_timeout(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================debugfs_create_file@001090d8====================
// signature: undefined debugfs_create_file(void)

/* WARNING: Control flow encountered bad instruction data */

void debugfs_create_file(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================register_syscore_ops@001090e0====================
// signature: undefined register_syscore_ops(void)

/* WARNING: Control flow encountered bad instruction data */

void register_syscore_ops(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__cfi_slowpath@001090e8====================
// signature: undefined __cfi_slowpath(void)

/* WARNING: Control flow encountered bad instruction data */

void __cfi_slowpath(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_raw_spin_unlock_irqrestore@001090f0====================
// signature: undefined _raw_spin_unlock_irqrestore(void)

/* WARNING: Control flow encountered bad instruction data */

void _raw_spin_unlock_irqrestore(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================kmem_cache_alloc_trace@001090f8====================
// signature: undefined kmem_cache_alloc_trace(void)

/* WARNING: Control flow encountered bad instruction data */

void kmem_cache_alloc_trace(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================seq_lseek@00109100====================
// signature: undefined seq_lseek(void)

/* WARNING: Control flow encountered bad instruction data */

void seq_lseek(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================seq_read@00109108====================
// signature: undefined seq_read(void)

/* WARNING: Control flow encountered bad instruction data */

void seq_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================single_open@00109118====================
// signature: undefined single_open(void)

/* WARNING: Control flow encountered bad instruction data */

void single_open(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================seq_printf@00109120====================
// signature: undefined seq_printf(void)

/* WARNING: Control flow encountered bad instruction data */

void seq_printf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__stack_chk_fail@00109148====================
// signature: noreturn undefined __stack_chk_fail(void)

/* WARNING: Control flow encountered bad instruction data */

void __stack_chk_fail(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__ubsan_handle_cfi_check_fail_abort@00109158====================
// signature: noreturn undefined __ubsan_handle_cfi_check_fail_abort(void)

/* WARNING: Control flow encountered bad instruction data */

void __ubsan_handle_cfi_check_fail_abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================memcpy@00109160====================
// signature: void * memcpy(void * __dest, void * __src, size_t __n)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



