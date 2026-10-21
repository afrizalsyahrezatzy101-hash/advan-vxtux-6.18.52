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



