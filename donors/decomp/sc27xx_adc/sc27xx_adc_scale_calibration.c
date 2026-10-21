// signature: undefined sc27xx_adc_scale_calibration(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong sc27xx_adc_scale_calibration(undefined8 param_1,long param_2,ulong param_3)

{
  char *pcVar1;
  size_t __n;
  undefined4 *puVar2;
  bool bVar3;
  ulong uVar4;
  void *__src;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  ulong local_48;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  bVar3 = (param_3 & 1) == 0;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  local_48 = 0;
  lVar5 = 0x18;
  if (bVar3) {
    lVar5 = 0x20;
  }
  lVar5 = *(long *)(param_2 + lVar5);
  pcVar1 = "big_scale_calib";
  if (bVar3) {
    pcVar1 = "small_scale_calib";
  }
  uVar4 = nvmem_cell_get(param_1,pcVar1);
  if (uVar4 < 0xfffffffffffff001) {
    __src = (void *)nvmem_cell_read(uVar4,&local_48);
    nvmem_cell_put(uVar4);
    if (__src < (void *)0xfffffffffffff001) {
      __n = local_48;
      if (3 < local_48) {
        __n = 4;
      }
      memcpy(&local_3c,__src,__n);
      puVar2 = &big_scale_graph;
      if ((param_3 & 1) == 0) {
        puVar2 = &small_scale_graph;
      }
      puVar2[1] = (*(int *)(lVar5 + 4) + (local_3c & 0xff)) * 4 + -0x200;
      puVar2[3] = (*(int *)(lVar5 + 0xc) + (local_3c >> 8 & 0xff)) * 4 + -0x200;
      kfree(__src);
      uVar4 = 0;
    }
    else {
      uVar4 = (ulong)__src & 0xffffffff;
    }
  }
  if (___stack_chk_guard == local_38) {
    return uVar4 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



