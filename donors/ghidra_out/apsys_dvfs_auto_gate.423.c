// apsys_dvfs_auto_gate.423 @ 0010f150
// signature: undefined apsys_dvfs_auto_gate.423(void)
// calling convention: unknown


void apsys_dvfs_auto_gate_423(long param_1,int param_2)

{
  *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50) =
       *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50) & 0xfffffffe | (uint)(param_2 != 0);
  return;
}


