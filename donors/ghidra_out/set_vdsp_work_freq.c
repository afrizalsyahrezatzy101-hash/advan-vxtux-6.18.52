// set_vdsp_work_freq @ 00114a70
// signature: undefined set_vdsp_work_freq(void)
// calling convention: unknown


int set_vdsp_work_freq(long param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  undefined8 *in_x18;
  undefined8 uVar4;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar3 = 2;
  if (param_2 != 936000000) {
    uVar3 = (uint)(param_2 == 0x249f0000);
  }
  uVar4 = (&clk_pll)[uVar3];
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  iVar1 = clk_prepare(uVar4);
  if (iVar1 == 0) {
    iVar1 = clk_enable(uVar4);
    if (iVar1 == 0) {
      if (DAT_0010166c == param_2) {
        uVar2 = 0;
      }
      else if (DAT_00101680 == param_2) {
        uVar2 = 1;
      }
      else if (DAT_00101694 == param_2) {
        uVar2 = 2;
      }
      else if (DAT_001016a8 == param_2) {
        uVar2 = 3;
      }
      else if (DAT_001016bc == param_2) {
        uVar2 = 4;
      }
      else {
        if (DAT_001016d0 != param_2) {
          return 0;
        }
        uVar2 = 5;
      }
      *(undefined4 *)(lVar5 + 0x114) = uVar2;
      return 0;
    }
    clk_unprepare(uVar4);
  }
  printk(&DAT_001024c3,"set_vdsp_work_freq",iVar1,uVar3);
  return iVar1;
}


