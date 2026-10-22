// trusty_log_register_buf @ 00101b34
// signature: undefined trusty_log_register_buf(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong trusty_log_register_buf(undefined8 param_1,long param_2,ulong param_3)

{
  uint uVar1;
  int iVar2;
  ulong uVar3;
  undefined8 *in_x18;
  ulong uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((-(param_3 >> 0x1f & 1) & 0xfffffff800000000 | (param_3 & 0xffffffff) << 3) < 0x1001) {
    uVar4 = __get_free_pages(0xdc0,0);
    if (uVar4 == 0) {
      uVar4 = 0xfffffffffffffff4;
    }
    else {
      if ((int)param_3 != 0) {
        uVar1 = (int)param_3 - 1;
        do {
          uVar3 = -(ulong)(uVar1 >> 0x1f) & 0xfffffff800000000 | (ulong)uVar1 << 3;
          uVar1 = uVar1 - 1;
          *(long *)(uVar4 + uVar3) =
               *(long *)(param_2 + uVar3) + (_memstart_addr >> 0xc) * 0x40 + 0x100200000 >> 6;
        } while (uVar1 != 0xffffffff);
      }
      if ((uVar4 ^ 0xffffff8000000000) < (-1L << (_vabits_actual - 1U & 0x3f)) + 0x8000000000U) {
        uVar3 = _memstart_addr + (uVar4 & 0x7fffffffff);
      }
      else {
        uVar3 = uVar4 - _kimage_voffset;
      }
      iVar2 = trusty_std_call32(param_1,0x33000001,uVar3,uVar3 >> 0x20,param_3 & 0xffffffff);
      if (iVar2 < 0) {
        printk(&DAT_00100580,iVar2);
        free_pages(uVar4,0);
        uVar4 = (ulong)iVar2;
      }
    }
  }
  else {
    uVar4 = 0xffffffffffffffea;
  }
  return uVar4;
}


