====================get_ip_status@0010f4bc====================
// signature: undefined get_ip_status(void)

undefined8 get_ip_status(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 0x17 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103e90) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103ec8) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103f00) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103f38) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103f70) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103fa8) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103fe0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00104018) goto LAB_0010f5c0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_00103e8c)[lVar3 * 0xe];
LAB_0010f5c0:
  *(uint *)(param_2 + 8) = uVar1 >> 0x15 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}


