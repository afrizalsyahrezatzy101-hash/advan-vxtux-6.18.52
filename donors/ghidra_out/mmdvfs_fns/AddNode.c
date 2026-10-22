====================AddNode@0010c9f4====================
// signature: undefined AddNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void AddNode(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  undefined8 *in_x18;
  long lVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = ___stack_chk_guard;
  uVar1 = param_2[1];
  uVar2 = param_2[2];
  uVar5 = *param_2;
  puVar4 = (undefined8 *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  *puVar4 = uVar5;
  puVar4[1] = uVar1;
  puVar4[2] = uVar2;
  puVar4[3] = 0;
  do {
    lVar6 = param_1;
    param_1 = *(long *)(lVar6 + 0x18);
    if (param_1 == 0) goto LAB_0010ca70;
  } while ((ulong)param_2[1] <= *(ulong *)(param_1 + 8));
  puVar4[3] = param_1;
LAB_0010ca70:
  *(undefined8 **)(lVar6 + 0x18) = puVar4;
  if (___stack_chk_guard == lVar3) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


