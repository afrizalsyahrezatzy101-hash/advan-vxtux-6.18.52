// signature: undefined adc_nvmem_cell_calib_data(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong adc_nvmem_cell_calib_data(long param_1)

{
  size_t __n;
  ulong uVar1;
  void *__src;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  ulong local_38;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  local_38 = 0;
  if (param_1 == 0) {
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = nvmem_cell_get(*(undefined8 *)(param_1 + 8));
    if ((uVar1 != 0) && (uVar1 < 0xfffffffffffff001)) {
      __src = (void *)nvmem_cell_read(uVar1,&local_38);
      if ((__src == (void *)0x0) || ((void *)0xfffffffffffff000 < __src)) {
        nvmem_cell_put(uVar1);
        uVar1 = (ulong)__src & 0xffffffff;
      }
      else {
        __n = local_38;
        if (3 < local_38) {
          __n = 4;
        }
        memcpy(&local_2c,__src,__n);
        kfree(__src);
        nvmem_cell_put(uVar1);
        uVar1 = (ulong)local_2c;
      }
    }
  }
  if (___stack_chk_guard == local_28) {
    return uVar1 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



