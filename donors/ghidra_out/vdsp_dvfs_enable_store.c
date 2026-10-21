// vdsp_dvfs_enable_store @ 0010ae04
// signature: undefined vdsp_dvfs_enable_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 vdsp_dvfs_enable_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  undefined8 *in_x18;
  undefined4 *puVar2;
  undefined8 unaff_x30;
  undefined4 local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  puVar2 = *(undefined4 **)(*(long *)(param_1 + 0x60) + 0x98);
  local_2c = 0;
  iVar1 = sscanf(param_3,"%d\n",&local_2c);
  if (iVar1 == 0) {
    param_4 = 0xffffffffffffffea;
  }
  else {
    *puVar2 = local_2c;
  }
  if (___stack_chk_guard == local_28) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


