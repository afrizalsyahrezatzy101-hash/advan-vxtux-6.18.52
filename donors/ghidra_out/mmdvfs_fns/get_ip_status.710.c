====================get_ip_status.710@001103e0====================
// signature: undefined get_ip_status.710(void)

undefined8 get_ip_status_710(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 3 & 7;
  *(uint *)(param_2 + 8) = uVar1 >> 6 & 7;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00104050) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00104088) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_001040c0) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001040f8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00104130) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00104168) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_001041a0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_001041d8) goto LAB_001104e8;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)((long)&DAT_00104048 + lVar3 * 0x38 + 4);
LAB_001104e8:
  printk(&DAT_001059f0,"get_ip_status",uVar1,uVar1);
  return 0;
}


