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



