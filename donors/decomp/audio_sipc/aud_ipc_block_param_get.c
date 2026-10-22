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



