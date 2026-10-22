====================mmsys_power_ctrl@001173f8====================
// signature: undefined mmsys_power_ctrl(void)

void mmsys_power_ctrl(undefined8 param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = DAT_00100c38;
  *DAT_00100c30 = 0x3208004;
  *puVar1 = 0x3c280;
  printk(&DAT_0010480c,"mmsys_power_ctrl");
  uVar2 = 1;
  if (param_2 == 0) {
    uVar2 = 2;
  }
  mmsys_notify_call_chain(uVar2);
  return;
}


