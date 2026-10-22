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



