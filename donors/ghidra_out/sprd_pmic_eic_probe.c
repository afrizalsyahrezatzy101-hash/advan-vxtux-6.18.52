// sprd_pmic_eic_probe @ 00101030
// signature: undefined sprd_pmic_eic_probe(void)
// calling convention: unknown


int sprd_pmic_eic_probe(long param_1)

{
  long *plVar1;
  int iVar2;
  long *plVar3;
  long lVar4;
  char *pcVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar1 = (long *)(param_1 + 0x10);
  plVar3 = (long *)devm_kmalloc(plVar1,0x378,0xdc0);
  if (plVar3 == (long *)0x0) {
    iVar2 = -0xc;
  }
  else {
    __mutex_init(plVar3 + 0x6a,"&pmic_eic->buslock",0x100000);
    iVar2 = platform_get_irq(param_1,0);
    *(int *)(plVar3 + 0x6e) = iVar2;
    if (-1 < iVar2) {
      lVar4 = dev_get_regmap(*(undefined8 *)(param_1 + 0x70),0);
      plVar3[0x62] = lVar4;
      if (lVar4 == 0) {
        iVar2 = -0x13;
      }
      else {
        iVar2 = of_property_read_variable_u32_array
                          (*(undefined8 *)(param_1 + 0x310),&DAT_0010017d,plVar3 + 99,1,0);
        if (iVar2 < 0) {
          pcVar5 = "Failed to get PMIC EIC base address.\n";
        }
        else {
          lVar4 = *(long *)(param_1 + 0x80);
          if (lVar4 == 0) {
            lVar4 = *plVar1;
          }
          iVar2 = devm_request_threaded_irq
                            (plVar1,(int)plVar3[0x6e],0,&LAB_001021f0,0x6008,lVar4,plVar3);
          if (iVar2 == 0) {
            lVar6 = *(long *)(param_1 + 0x80);
            lVar4 = lVar6;
            if (lVar6 == 0) {
              lVar4 = *plVar1;
            }
            *plVar3 = lVar4;
            plVar3[2] = (long)plVar1;
            *(undefined2 *)((long)plVar3 + 0x8c) = 0x10;
            *(undefined4 *)(plVar3 + 0x11) = 0xffffffff;
            lVar4 = *(long *)(param_1 + 0x310);
            plVar3[4] = (long)&LAB_00102204;
            plVar3[5] = (long)&LAB_001021f4;
            plVar3[0xd] = (long)&LAB_001021f8;
            plVar3[0xb] = (long)&LAB_001021fc;
            plVar3[7] = (long)&LAB_00102200;
            plVar3[0x39] = lVar4;
            plVar3[9] = (long)&LAB_00102208;
            if (lVar6 == 0) {
              lVar6 = *plVar1;
            }
            plVar3[0x3f] = lVar6;
            plVar3[0x4b] = (long)&LAB_0010220c;
            plVar3[0x14] = (long)(plVar3 + 0x3e);
            plVar3[0x61] = 0x10;
            plVar3[0x45] = (long)&LAB_00102210;
            plVar3[0x47] = (long)&LAB_00102214;
            plVar3[0x4d] = (long)&LAB_00102218;
            plVar3[0x4e] = (long)&LAB_0010221c;
            *(undefined1 *)(plVar3 + 0x2f) = 1;
            iVar2 = devm_gpiochip_add_data(plVar1,plVar3,plVar3);
            if (-1 < iVar2) {
              *(long **)(param_1 + 0xa8) = plVar3;
              return 0;
            }
            _dev_err(plVar1,"Could not register gpiochip %d.\n",iVar2);
            return iVar2;
          }
          pcVar5 = "Failed to request PMIC EIC IRQ.\n";
        }
        _dev_err(plVar1,pcVar5);
      }
    }
  }
  return iVar2;
}


