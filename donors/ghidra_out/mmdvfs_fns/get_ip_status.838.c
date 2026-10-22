====================get_ip_status.838@001124dc====================
// signature: undefined get_ip_status.838(void)

undefined8 get_ip_status_838(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = *(uint *)(g_mmreg_map + 0x38);
  uVar1 = *(uint *)(g_mmreg_map + 0x48) & 7;
  *(uint *)(param_2 + 0xc) = uVar1;
  if (uVar1 == DAT_001043d0) {
    lVar3 = 0;
  }
  else if (uVar1 == DAT_00104408) {
    lVar3 = 1;
  }
  else if (uVar1 == DAT_00104440) {
    lVar3 = 2;
  }
  else if (uVar1 == DAT_00104478) {
    lVar3 = 3;
  }
  else if (uVar1 == DAT_001044b0) {
    lVar3 = 4;
  }
  else if (uVar1 == DAT_001044e8) {
    lVar3 = 5;
  }
  else if (uVar1 == DAT_00104520) {
    lVar3 = 6;
  }
  else {
    if (uVar1 != DAT_00104558) goto LAB_001125e0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_001043cc)[lVar3 * 0xe];
LAB_001125e0:
  *(uint *)(param_2 + 8) = uVar2 >> 3 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}


