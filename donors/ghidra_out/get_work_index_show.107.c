// get_work_index_show.107 @ 001094d4
// signature: undefined get_work_index_show.107(void)
// calling convention: unknown


long get_work_index_show_107(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  code *pcVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_0010291a);
    lVar5 = -0x16;
  }
  else {
    lVar3 = *(long *)(lVar5 + 1000);
    if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x78), pcVar4 == (code *)0x0)) {
      memcpy(param_3,"undefined\n",0xb);
      lVar5 = 10;
    }
    else {
      if (0x17 < ((ulong)(pcVar4 + -0x117efc) >> 2 | (long)pcVar4 << 0x3e)) {
        __cfi_slowpath(0x70ac3a0dddbe0e7e,pcVar4);
      }
      uVar2 = (*pcVar4)(lVar5);
      iVar1 = sprintf(param_3,"%d\n",uVar2 & 0xffffffff);
      lVar5 = (long)iVar1;
    }
  }
  return lVar5;
}


