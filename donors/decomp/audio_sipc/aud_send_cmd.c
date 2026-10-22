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



