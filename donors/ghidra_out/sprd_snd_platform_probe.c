// sprd_snd_platform_probe @ 001022ec
// signature: undefined sprd_snd_platform_probe(void)
// calling convention: unknown


undefined8 sprd_snd_platform_probe(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x20) + 0x90);
  lVar1 = snd_info_create_card_entry(lVar1,"sprd-dmaengine",*(undefined8 *)(lVar1 + 0x5c0));
  if (lVar1 != 0) {
    *(long *)(lVar1 + 0x40) = param_1;
    *(undefined1 **)(lVar1 + 0x20) = &LAB_00106368;
  }
  *(undefined1 **)(lVar1 + 0x28) = &LAB_0010636c;
  *(ushort *)(lVar1 + 8) = *(ushort *)(lVar1 + 8) | 0x80;
  return 0;
}


