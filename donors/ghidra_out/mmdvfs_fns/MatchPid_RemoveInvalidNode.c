====================MatchPid_RemoveInvalidNode@0010c8b8====================
// signature: undefined MatchPid_RemoveInvalidNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 MatchPid_RemoveInvalidNode(long *param_1,long *param_2)

{
  ulong uVar1;
  long *plVar2;
  long *plVar3;
  undefined8 *in_x18;
  long lVar4;
  long *plVar5;
  long lVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar2 = param_1;
  do {
    plVar5 = plVar2;
    plVar2 = (long *)plVar5[3];
    if (plVar2 == (long *)0x0) {
      if ((int)param_2[2] == 0) {
        return 0;
      }
      lVar6 = *param_2;
      uVar1 = param_2[1];
      lVar4 = param_2[2];
      plVar2 = (long *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
      *plVar2 = lVar6;
      plVar2[1] = uVar1;
      plVar2[2] = lVar4;
      plVar2[3] = 0;
      goto LAB_0010c978;
    }
    lVar6 = *plVar2;
  } while (lVar6 != *param_2);
  if ((int)param_2[2] == 0) {
    if (plVar5 == (long *)0x0) {
      return 0;
    }
    plVar5[3] = plVar2[3];
    kfree();
    return 0;
  }
  if (plVar5 != (long *)0x0) {
    plVar5[3] = plVar2[3];
    kfree();
    lVar6 = *param_2;
  }
  uVar1 = param_2[1];
  lVar4 = param_2[2];
  plVar2 = (long *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  *plVar2 = lVar6;
  plVar2[1] = uVar1;
  plVar2[2] = lVar4;
  plVar2[3] = 0;
  do {
    plVar3 = plVar5;
    plVar5 = (long *)plVar3[3];
    if (plVar5 == (long *)0x0) goto LAB_0010c994;
  } while (uVar1 <= (ulong)plVar5[1]);
  plVar2[3] = (long)plVar5;
LAB_0010c994:
  plVar3[3] = (long)plVar2;
  return 0;
  while (uVar1 <= (ulong)param_1[1]) {
LAB_0010c978:
    plVar3 = param_1;
    param_1 = (long *)plVar3[3];
    if (param_1 == (long *)0x0) goto LAB_0010c994;
  }
  plVar2[3] = (long)param_1;
  goto LAB_0010c994;
}


