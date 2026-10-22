// signature: undefined sc2720_adc_scale_init(void)

void sc2720_adc_scale_init(long param_1)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined4 uVar4;
  
  lVar3 = 0;
  do {
    uVar2 = (uint)lVar3;
    if (uVar2 < 0x20) {
      uVar1 = 1 << (ulong)(uVar2 & 0x1f);
      if ((uVar1 & 0xc0080020) == 0) {
        if ((uVar1 & 0x280) == 0) {
          if (uVar2 != 0xd) goto LAB_00103800;
          uVar4 = 1;
        }
        else {
          uVar4 = 2;
        }
      }
      else {
        uVar4 = 3;
      }
    }
    else {
LAB_00103800:
      uVar4 = 0;
    }
    *(undefined4 *)(param_1 + 0x28 + lVar3 * 4) = uVar4;
    lVar3 = lVar3 + 1;
    if (lVar3 == 0x20) {
      return;
    }
  } while( true );
}



