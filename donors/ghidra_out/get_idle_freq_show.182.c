// get_idle_freq_show.182 @ 0010a964
// signature: undefined get_idle_freq_show.182(void)
// calling convention: unknown


long get_idle_freq_show_182(long param_1,undefined8 param_2,char *param_3)

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
  if ((lVar4 == 0) || (pcVar5 = *(code **)(lVar4 + 0x58), pcVar5 == (code *)0x0)) {
    memcpy(param_3,"undefined\n",0xb);
    lVar2 = 10;
  }
  else {
    if (3 < ((ulong)(pcVar5 + -0x117fb0) >> 2 | (long)pcVar5 << 0x3e)) {
      __cfi_slowpath(0xc61b8c9ed5796c39,pcVar5);
    }
    uVar3 = (*pcVar5)(lVar2);
    iVar1 = sprintf(param_3,"%d\n",uVar3 & 0xffffffff);
    lVar2 = (long)iVar1;
  }
  return lVar2;
}


