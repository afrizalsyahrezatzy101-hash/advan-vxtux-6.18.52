// tui_write @ 001011ec
// signature: undefined tui_write(void)
// calling convention: unknown


undefined8 tui_write(long param_1,undefined8 param_2,long param_3)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_3 == 4) {
    lVar3 = *(long *)(param_1 + 200);
    lVar1 = _copy_from_user((int *)(lVar3 + 4));
    if (lVar1 == 0) {
      if (*(int *)(lVar3 + 4) == 0) {
        *(undefined4 *)(lVar3 + 8) = 1;
        __wake_up(lVar3 + 0x10,1,1,0);
      }
      uVar2 = 4;
    }
    else {
      uVar2 = 0xfffffffffffffff2;
    }
  }
  else {
    printk(&DAT_0010027b,"tui_write",4,param_3);
    uVar2 = 0xffffffffffffffea;
  }
  return uVar2;
}


