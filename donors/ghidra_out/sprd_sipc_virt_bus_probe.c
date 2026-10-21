// sprd_sipc_virt_bus_probe @ 00101030
// signature: undefined sprd_sipc_virt_bus_probe(void)
// calling convention: unknown


int sprd_sipc_virt_bus_probe(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  param_1 = param_1 + 0x10;
  iVar1 = devm_of_platform_populate(param_1);
  if (iVar1 == 0) {
    _dev_info(param_1,"Populate sub sipc-virt-bus success\n");
  }
  else {
    _dev_err(param_1,"Failed to populate sub sipc-virt-bus\n");
  }
  return iVar1;
}


