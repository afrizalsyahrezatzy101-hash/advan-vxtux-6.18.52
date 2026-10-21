// sprd_pcm_new @ 00102358
// signature: undefined sprd_pcm_new(void)
// calling convention: unknown


undefined8 sprd_pcm_new(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 8) + 0x90);
  get_sp_audio_debug_flag();
  lVar1 = *(long *)(lVar2 + 0x5f8);
  if (*(long *)(lVar1 + 0x2b0) == 0) {
    dma_set_mask(lVar1,0xffffffffffffffff);
    lVar1 = *(long *)(lVar2 + 0x5f8);
  }
  *(undefined8 *)(lVar1 + 0x2b8) = 0xffffffffffffffff;
  get_sp_audio_debug_flag();
  return 0;
}


