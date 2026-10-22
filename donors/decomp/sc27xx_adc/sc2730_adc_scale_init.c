// signature: undefined sc2730_adc_scale_init(void)

void sc2730_adc_scale_init(long param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  
  lVar4 = 0;
  lVar1 = param_1 + 0x28;
  do {
    uVar3 = (uint)lVar4;
    if (uVar3 < 0x20) {
      uVar2 = 1 << (ulong)(uVar3 & 0x1f);
      if ((uVar2 & 0xc0080420) == 0) {
        if ((uVar2 & 0x280) == 0) {
          if (uVar3 != 0xd) goto LAB_001039a4;
          *(undefined4 *)(param_1 + 0x5c) = 1;
        }
        else {
          *(undefined4 *)(lVar1 + lVar4 * 4) = 2;
        }
      }
      else {
        *(undefined4 *)(lVar1 + lVar4 * 4) = 3;
      }
    }
    else {
LAB_001039a4:
      *(undefined4 *)(lVar1 + lVar4 * 4) = 0;
    }
    lVar4 = lVar4 + 1;
    if (lVar4 == 0x20) {
      return;
    }
  } while( true );
}



