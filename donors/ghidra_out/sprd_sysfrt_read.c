// sprd_sysfrt_read @ 00101104
// signature: undefined sprd_sysfrt_read(void)
// calling convention: unknown


undefined8 sprd_sysfrt_read(void)

{
  if (sprd_sysfrt_addr_base != 0) {
    return CONCAT44(*(undefined4 *)(sprd_sysfrt_addr_base + 0xc),
                    *(undefined4 *)(sprd_sysfrt_addr_base + 4));
  }
  return 0;
}


