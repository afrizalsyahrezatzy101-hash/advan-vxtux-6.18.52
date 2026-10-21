// vdsp_dvfs_notifier_call_chain @ 0010a1ec
// signature: undefined vdsp_dvfs_notifier_call_chain(void)
// calling convention: unknown


void vdsp_dvfs_notifier_call_chain(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  blocking_notifier_call_chain(&vdsp_dvfs_chain,0,param_1);
  return;
}


