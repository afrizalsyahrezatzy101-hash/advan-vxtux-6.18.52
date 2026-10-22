// signature: undefined ump96xx_adc_scale_cal(void)

uint ump96xx_adc_scale_cal(long param_1,ulong param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == 0) {
    uVar2 = 0xffffffea;
  }
  else {
    uVar6 = -(param_2 >> 0x1f & 1) & 0xfffffff800000000 | (param_2 & 0xffffffff) << 3;
    uVar5 = *(undefined8 *)((long)&PTR_s_big_scale_calib1_001020a8 + uVar6);
    uVar2 = adc_nvmem_cell_calib_data(param_1,uVar5);
    if ((int)uVar2 < 0) {
      _dev_err(*(undefined8 *)(param_1 + 8),"err! %s:%d\n",uVar5,uVar2);
    }
    else {
      uVar5 = *(undefined8 *)((long)&PTR_s_big_scale_calib2_001020c0 + uVar6);
      uVar3 = adc_nvmem_cell_calib_data(param_1,uVar5);
      if ((int)uVar3 < 0) {
        _dev_err(*(undefined8 *)(param_1 + 8),"err! %s:%d\n",uVar5,uVar3);
        uVar2 = uVar3;
      }
      else {
        uVar1 = uVar2 >> 4 & 0xfff;
        uVar2 = 0;
        if ((uVar1 != 0) && (uVar3 = uVar3 >> 4 & 0xfff, uVar3 != 0)) {
          lVar4 = *(long *)((long)&PTR_big_scale_graph_00102090 + uVar6);
          *(uint *)(lVar4 + 4) = uVar1;
          *(uint *)(lVar4 + 0xc) = uVar3;
          uVar2 = 0;
        }
      }
    }
  }
  return uVar2;
}



