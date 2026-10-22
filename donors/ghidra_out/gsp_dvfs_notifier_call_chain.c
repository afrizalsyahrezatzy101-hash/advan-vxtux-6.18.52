// gsp_dvfs_notifier_call_chain @ 00115db0
// signature: undefined gsp_dvfs_notifier_call_chain(void)
// calling convention: unknown


void gsp_dvfs_notifier_call_chain(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  atomic_notifier_call_chain(&gsp_dvfs_chain,0,param_1);
  return;
}


