// sprd_mbox_index_xlate @ 00102658
// signature: undefined sprd_mbox_index_xlate(void)
// calling convention: unknown


long sprd_mbox_index_xlate(long param_1,long param_2)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = *(int *)(param_2 + 0xc);
  if (iVar1 < *(int *)(param_1 + 0x18)) {
    if (*(int *)(param_2 + 0x10) != 0) {
      *(long *)(param_1 + 0xd8) = (long)iVar1;
      *(ulong *)(param_1 + 200) =
           *(long *)(param_1 + 0x98) + (ulong)(uint)(*(int *)(param_1 + 0xb4) * iVar1);
    }
    lVar2 = *(long *)(param_1 + 0x10) + (long)iVar1 * 0xf8;
  }
  else {
    lVar2 = -0x16;
  }
  return lVar2;
}


