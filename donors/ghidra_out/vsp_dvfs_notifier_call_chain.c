// vsp_dvfs_notifier_call_chain @ 00108e44
// signature: undefined vsp_dvfs_notifier_call_chain(void)
// calling convention: unknown


void vsp_dvfs_notifier_call_chain(undefined8 param_1,ulong param_2)

{
  undefined1 *puVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((param_2 & 1) == 0) {
    puVar1 = &vsp_dvfs_chain;
  }
  else {
    puVar1 = &vpuenc_dvfs_chain;
  }
  blocking_notifier_call_chain(puVar1,0,param_1);
  return;
}


