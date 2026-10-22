// __typeid__ZTSFbvE_global_addr @ 00104394
// signature: undefined __typeid__ZTSFbvE_global_addr(void)
// calling convention: unknown


uint __typeid__ZTSFbvE_global_addr(void)

{
  DataMemoryBarrier(0,1);
  return *(uint *)(sprd_outbox_base + 0x18) & 1;
}


