// sprd_soc_platform_probe @ 00102030
// signature: undefined sprd_soc_platform_probe(void)
// calling convention: unknown


undefined8 sprd_soc_platform_probe(long param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_1 + 0x310) == 0) {
    printk(&DAT_00100b23);
    uVar3 = 0xffffffff;
  }
  else {
    lVar2 = devm_kmalloc(param_1 + 0x10,0x70,0xdc0);
    pm_dma = lVar2;
    if (lVar2 == 0) {
      uVar3 = 0xfffffff4;
    }
    else {
      *(undefined4 *)(lVar2 + 0x28) = 0;
      __mutex_init(lVar2 + 0x30,"&pm_dma->pm_mtx_dma_prot",&init_pm_dma___key_3);
      __mutex_init(lVar2 + 0x50,"&pm_dma->pm_mtx_cnt",&init_pm_dma___key_3);
      *(undefined8 *)(lVar2 + 0x10) = &LAB_0010632c;
      iVar1 = register_pm_notifier((undefined8 *)(lVar2 + 0x10));
      if (iVar1 != 0) {
        printk(&DAT_00100b4d);
      }
      uVar3 = snd_soc_register_component(param_1 + 0x10,&sprd_soc_platform,0,0);
    }
  }
  return uVar3;
}


