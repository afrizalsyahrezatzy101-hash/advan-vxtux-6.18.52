====================get_ip_status.782@001113dc====================
// signature: undefined get_ip_status.782(void)

undefined8 get_ip_status_782(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 10 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103b10) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103b48) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103b80) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103bb8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103bf0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103c28) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103c60) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103c98) goto LAB_001114e0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)((long)&DAT_00103b08 + lVar3 * 0x38 + 4);
LAB_001114e0:
  *(uint *)(param_2 + 8) = uVar1 >> 0xf & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}


