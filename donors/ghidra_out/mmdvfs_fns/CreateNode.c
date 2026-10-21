====================CreateNode@0010c778====================
// signature: undefined CreateNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void CreateNode(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = (undefined8 *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  uVar1 = param_1[1];
  uVar3 = param_1[2];
  *puVar2 = *param_1;
  puVar2[1] = uVar1;
  puVar2[2] = uVar3;
  puVar2[3] = 0;
  return;
}


