// sprd_pcm_mmap @ 00104628
// signature: undefined sprd_pcm_mmap(void)
// calling convention: unknown


void sprd_pcm_mmap(long param_1,long *param_2)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(param_1 + 0x140);
  param_2[9] = param_2[9] & 0xffffffffffffffefU | 0x6000000000000c;
  remap_pfn_range(param_2,*param_2,*(ulong *)(lVar1 + 0x2c0) >> 0xc,param_2[1] - *param_2);
  return;
}


