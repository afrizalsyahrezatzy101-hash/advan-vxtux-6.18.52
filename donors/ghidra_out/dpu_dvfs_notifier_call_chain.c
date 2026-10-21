// dpu_dvfs_notifier_call_chain @ 001079a8
// signature: undefined dpu_dvfs_notifier_call_chain(void)
// calling convention: unknown


void dpu_dvfs_notifier_call_chain(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  atomic_notifier_call_chain(&dpu_dvfs_chain,0,param_1);
  return;
}


