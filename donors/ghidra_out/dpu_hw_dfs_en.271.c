// dpu_hw_dfs_en.271 @ 0010cd90
// signature: undefined dpu_hw_dfs_en.271(void)
// calling convention: unknown


void dpu_hw_dfs_en_271(long param_1,uint param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 8) = *(uint *)(lVar1 + 8) & 0xfffffffe | param_2 & 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


