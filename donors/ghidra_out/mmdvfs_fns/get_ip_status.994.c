====================get_ip_status.994@001145c8====================
// signature: undefined get_ip_status.994(void)

undefined8 get_ip_status_994(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 8 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00104210) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00104248) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00104280) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001042b8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_001042f0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00104328) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00104360) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00104398) goto LAB_001146cc;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_0010420c)[lVar3 * 0xe];
LAB_001146cc:
  *(uint *)(param_2 + 8) = uVar1 >> 0xc & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}


