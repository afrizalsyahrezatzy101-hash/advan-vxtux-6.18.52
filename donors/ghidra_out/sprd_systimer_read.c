// sprd_systimer_read @ 001010e0
// signature: undefined sprd_systimer_read(void)
// calling convention: unknown


undefined4 sprd_systimer_read(void)

{
  if (sprd_systimer_addr_base != 0) {
    return *(undefined4 *)(sprd_systimer_addr_base + 0xc);
  }
  return 0;
}


