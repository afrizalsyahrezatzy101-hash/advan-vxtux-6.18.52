// get_work_index_show.177 @ 0010a78c
// signature: undefined get_work_index_show.177(void)
// calling convention: unknown


long get_work_index_show_177(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  code *pcVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  lVar4 = *(long *)(lVar2 + 0x480);
  if ((lVar4 == 0) || (pcVar5 = *(code **)(lVar4 + 0x28), pcVar5 == (code *)0x0)) {
    memcpy(param_3,"undefined\n",0xb);
    lVar2 = 10;
  }
  else {
    if (5 < ((ulong)(pcVar5 + -0x117f90) >> 2 | (long)pcVar5 << 0x3e)) {
      __cfi_slowpath(0x9974caa7c4cff8b7,pcVar5);
    }
    uVar3 = (*pcVar5)(lVar2);
    iVar1 = sprintf(param_3,"%d\n",uVar3 & 0xffffffff);
    lVar2 = (long)iVar1;
  }
  return lVar2;
}


