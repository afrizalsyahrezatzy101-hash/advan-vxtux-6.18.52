// get_convert_para @ 00101138
// signature: undefined get_convert_para(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void get_convert_para(undefined8 *param_1)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  do {
    iVar1 = systimer_seq;
    DataMemoryBarrier(2,1);
    *param_1 = cnter_to_boottime;
    uVar4 = _DAT_001000a8;
    uVar3 = DAT_00100098;
    uVar2 = DAT_00100090;
    param_1[3] = _DAT_001000a0;
    param_1[4] = uVar4;
    param_1[1] = uVar2;
    param_1[2] = uVar3;
    DataMemoryBarrier(2,1);
  } while (systimer_seq != iVar1);
  return;
}


