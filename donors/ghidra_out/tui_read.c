// tui_read @ 0010116c
// signature: undefined tui_read(void)
// calling convention: unknown


undefined8 tui_read(long param_1,undefined8 param_2,long param_3)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined4 *puVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_3 == 4) {
    puVar3 = *(undefined4 **)(param_1 + 200);
    lVar1 = _copy_to_user(param_2,puVar3);
    if (lVar1 == 0) {
      *puVar3 = 0xffffffff;
      printk(&DAT_001002ed);
      uVar2 = 4;
    }
    else {
      uVar2 = 0xfffffffffffffff2;
    }
  }
  else {
    printk(&DAT_001002be,"tui_read",4,param_3);
    uVar2 = 0xffffffffffffffea;
  }
  return uVar2;
}


