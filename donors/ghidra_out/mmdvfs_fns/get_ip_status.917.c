====================get_ip_status.917@00113554====================
// signature: undefined get_ip_status.917(void)

undefined8 get_ip_status_917(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 0xc & 7;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103cd0) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103d08) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103d40) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103d78) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103db0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103de8) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103e20) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103e58) goto LAB_00113658;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_00103ccc)[lVar3 * 0xe];
LAB_00113658:
  *(uint *)(param_2 + 8) = uVar1 >> 0x12 & 7;
  printk(&DAT_001059f0,"get_ip_status",uVar1,uVar1);
  return 1;
}


