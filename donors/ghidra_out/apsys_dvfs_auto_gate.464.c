// apsys_dvfs_auto_gate.464 @ 0010f580
// signature: undefined apsys_dvfs_auto_gate.464(void)
// calling convention: unknown


void apsys_dvfs_auto_gate_464(long param_1,int param_2)

{
  *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) =
       *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) & 0xfffffffe | (uint)(param_2 != 0);
  return;
}


