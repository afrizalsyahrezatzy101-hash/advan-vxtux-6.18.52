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



