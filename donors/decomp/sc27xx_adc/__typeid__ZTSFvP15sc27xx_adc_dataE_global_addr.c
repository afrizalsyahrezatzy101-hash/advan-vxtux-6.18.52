// signature: undefined __typeid__ZTSFvP15sc27xx_adc_dataE_global_addr(void)

void __typeid__ZTSFvP15sc27xx_adc_dataE_global_addr(long param_1)

{
  uint uVar1;
  long lVar2;
  undefined4 uVar3;
  
  lVar2 = 0;
  do {
    if ((uint)lVar2 < 0x20) {
      uVar1 = 1 << (ulong)((uint)lVar2 & 0x1f);
      if ((uVar1 & 0xc0080400) == 0) {
        if ((uVar1 & 0x280) == 0) {
          if ((uVar1 & 0x2001) == 0) goto LAB_00103714;
          uVar3 = 1;
        }
        else {
          uVar3 = 2;
        }
      }
      else {
        uVar3 = 3;
      }
    }
    else {
LAB_00103714:
      uVar3 = 0;
    }
    *(undefined4 *)(param_1 + 0x28 + lVar2 * 4) = uVar3;
    lVar2 = lVar2 + 1;
    if (lVar2 == 0x20) {
      return;
    }
  } while( true );
}



