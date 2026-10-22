// mmc_hsq_swcq_init @ 00107af0
// signature: undefined mmc_hsq_swcq_init(void)
// calling convention: unknown


undefined8 mmc_hsq_swcq_init(long param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (((*(uint *)(*(long *)(param_1 + 0x40) + 0x3f4) ^ 0xffffffff) & 0x280000) == 0) {
    lVar1 = devm_kmalloc(param_2 + 0x10,0xb018,0xdc0);
    if (lVar1 != 0) {
      uVar2 = mmc_swcq_init(lVar1,*(undefined8 *)(param_1 + 0x40));
      *(undefined4 *)(lVar1 + 0x3f60) = 1000000;
      return uVar2;
    }
  }
  else {
    lVar1 = devm_kmalloc(param_2 + 0x10,0x180,0xdc0);
    if (lVar1 != 0) {
      uVar2 = mmc_hsq_init(lVar1,*(undefined8 *)(param_1 + 0x40));
      return uVar2;
    }
  }
  return 0xfffffff4;
}


