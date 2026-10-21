// vser_set_inst_name @ 00102828
// signature: undefined vser_set_inst_name(void)
// calling convention: unknown


undefined8 vser_set_inst_name(long param_1,char *param_2)

{
  int iVar1;
  size_t sVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  sVar2 = strlen(param_2);
  iVar1 = (int)sVar2 + 1;
  if (iVar1 < 0x29) {
    lVar4 = kstrndup(param_2,(long)iVar1,0xcc0);
    if (lVar4 == 0) {
      uVar3 = 0xfffffff4;
    }
    else {
      uVar3 = 0;
      *(long *)(param_1 + 0xb8) = lVar4;
    }
  }
  else {
    uVar3 = 0xffffffdc;
  }
  return uVar3;
}


