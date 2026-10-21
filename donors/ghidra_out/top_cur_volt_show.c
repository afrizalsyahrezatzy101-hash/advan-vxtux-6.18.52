// top_cur_volt_show @ 00106b50
// signature: undefined top_cur_volt_show(void)
// calling convention: unknown


undefined8 top_cur_volt_show(long param_1,undefined8 param_2,void *param_3)

{
  int iVar1;
  undefined8 uVar2;
  char *__src;
  code *pcVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((*(long *)(param_1 + 0x3d0) == 0) ||
     (pcVar3 = *(code **)(*(long *)(param_1 + 0x3d0) + 0x40), pcVar3 == (code *)0x0)) {
    printk(&DAT_0010264a,"top_cur_volt_show");
LAB_00106c00:
    memcpy(param_3,"undefined\n",0xb);
    uVar2 = 10;
  }
  else {
    if (4 < ((ulong)(pcVar3 + -0x117db0) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x472928817137fb4e,pcVar3);
    }
    iVar1 = (*pcVar3)(param_1);
    if (iVar1 == 2) {
      __src = "0.8v\n";
    }
    else {
      if (iVar1 == 1) {
        memcpy(param_3,"0.75v\n",7);
        return 6;
      }
      if (iVar1 != 0) goto LAB_00106c00;
      __src = "0.7v\n";
    }
    memcpy(param_3,__src,6);
    uVar2 = 5;
  }
  return uVar2;
}


