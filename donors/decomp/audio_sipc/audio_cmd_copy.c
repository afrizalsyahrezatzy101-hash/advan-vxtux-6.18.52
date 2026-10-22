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



