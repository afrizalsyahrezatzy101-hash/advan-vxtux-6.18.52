// set_vdsp_work_index @ 00114990
// signature: undefined set_vdsp_work_index(void)
// calling convention: unknown


int set_vdsp_work_index(long param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 uVar3;
  long lVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = 2;
  if (param_2 != 5) {
    uVar2 = (uint)(param_2 == 4);
  }
  uVar3 = (&clk_pll)[uVar2];
  lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  iVar1 = clk_prepare(uVar3);
  if (iVar1 == 0) {
    iVar1 = clk_enable(uVar3);
    if (iVar1 == 0) {
      *(int *)(lVar4 + 0x114) = param_2;
      return 0;
    }
    clk_unprepare(uVar3);
  }
  printk(&DAT_001024c3,"set_vdsp_work_index",iVar1,uVar2);
  return iVar1;
}


