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



