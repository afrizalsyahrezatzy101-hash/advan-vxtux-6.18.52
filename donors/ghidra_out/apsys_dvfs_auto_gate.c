// apsys_dvfs_auto_gate @ 0010eb50
// signature: undefined apsys_dvfs_auto_gate(void)
// calling convention: unknown


void apsys_dvfs_auto_gate(long param_1,int param_2)

{
  *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) =
       *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) & 0xfffffffe | (uint)(param_2 != 0);
  return;
}


