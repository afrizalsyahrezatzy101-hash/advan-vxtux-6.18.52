// trusty_encode_page_info @ 00103418
// signature: undefined trusty_encode_page_info(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 trusty_encode_page_info(ulong *param_1,long param_2,ulong param_3)

{
  ulong uVar1;
  undefined8 uVar2;
  
  uVar2 = 0xffffffea;
  if ((param_1 != (ulong *)0x0) && (param_2 != 0)) {
    uVar1 = mair_el1;
    uVar2 = 0;
    *param_1 = ((param_2 + (_memstart_addr >> 0xc) * 0x40) * 0x40 + 0x4008000000 | param_3) &
               0xffffffffffff | (uVar1 >> (((uint)param_3 & 0x1c) << 1) & 0xff) << 0x30;
  }
  return uVar2;
}


