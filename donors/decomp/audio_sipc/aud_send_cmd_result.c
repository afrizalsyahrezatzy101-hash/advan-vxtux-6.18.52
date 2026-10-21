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



