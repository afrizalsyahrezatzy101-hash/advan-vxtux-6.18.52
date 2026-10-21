// get_gsp_dvfs_status @ 0011793c
// signature: undefined get_gsp_dvfs_status(void)
// calling convention: unknown


void get_gsp_dvfs_status(long param_1,long param_2)

{
  ulong uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = (ulong)*(ushort *)(lVar3 + 0x56) & 7;
  if ((uint)uVar1 < 5) {
    puVar2 = (&PTR_s_0_55v_00105d30)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  *(undefined **)(param_2 + 0x10) = puVar2;
  *(undefined **)(param_2 + 0x40) = (&PTR_DAT_00105e68)[(ulong)*(uint *)(lVar3 + 0x74) & 3];
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


