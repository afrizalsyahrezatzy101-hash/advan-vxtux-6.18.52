// trusty_log_alloc_pages @ 00101a0c
// signature: undefined trusty_log_alloc_pages(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long trusty_log_alloc_pages(ulong param_1,long *param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  long lVar3;
  ulong uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 - 1 < 0x20) {
    lVar3 = __kmalloc(param_1 << 3,0xcc0);
    if (lVar3 != 0) {
      uVar4 = 0;
      do {
        lVar1 = __alloc_pages_nodemask(0xcc2,0,0,0);
        if (lVar1 == 0) {
          param_1 = uVar4 & 0xffffffff;
          goto LAB_00101ad4;
        }
        *(long *)(lVar3 + uVar4 * 8) = lVar1;
        uVar4 = uVar4 + 1;
      } while (param_1 != uVar4);
      uVar2 = 0x68000000000f13;
      if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
         (uVar2 = 0x68000000000713, _kimage_vaddr != -0x3ff0000000)) {
        uVar2 = 0x68000000000f13;
      }
      lVar1 = vm_map_ram(lVar3,param_1 & 0xffffffff,0xffffffff,uVar2);
      if (lVar1 != 0) {
        *param_2 = lVar1;
        return lVar3;
      }
LAB_00101ad4:
      if ((int)param_1 != 0) {
        lVar1 = (param_1 & 0xffffffff) << 3;
        do {
          __free_pages(*(undefined8 *)(lVar3 + -8 + lVar1),0);
          lVar1 = lVar1 + -8;
        } while (lVar1 != 0);
      }
      kfree(lVar3);
    }
    lVar3 = -0xc;
  }
  else {
    lVar3 = -0x16;
  }
  return lVar3;
}


