====================get_ip_status.1058@0011567c====================
// signature: undefined get_ip_status.1058(void)

undefined8 get_ip_status_1058(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 6 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103950) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103988) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_001039c0) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001039f8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103a30) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103a68) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103aa0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103ad8) goto LAB_00115780;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_0010394c)[lVar3 * 0xe];
LAB_00115780:
  *(uint *)(param_2 + 8) = uVar1 >> 9 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}


