====================dvfs_probe@001080cc====================
// signature: undefined dvfs_probe(void)

int dvfs_probe(long param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;
  char *pcVar5;
  undefined8 *in_x18;
  undefined8 uVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar6 = *(undefined8 *)(param_1 + 0x310);
  lVar1 = param_1 + 0x10;
  printk(&DAT_0010665a);
  lVar3 = devm_kmalloc(lVar1,0x328,0xdc0);
  if (lVar3 == 0) {
    iVar2 = -0xc;
  }
  else {
    __mutex_init(lVar3 + 0x2f0,"&module->lock",&g_mmreg_map);
    iVar2 = dev_pm_opp_of_add_table(lVar1);
    if (iVar2 == 0) {
      iVar2 = of_property_read_variable_u32_array
                        (uVar6,"sprd,dvfs-work-index-def",lVar3 + 0x120,1,0);
      if (iVar2 < 0) {
        printk(&DAT_0010647b);
      }
      *(long *)(param_1 + 0xa8) = lVar3;
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile,"mtx_dvfs",0);
      *(ulong *)(lVar3 + 0x50) = uVar4;
      if (uVar4 < 0xfffffffffffff001) {
        *(undefined4 *)(lVar3 + 800) = 0;
        *(undefined1 **)(lVar3 + 0x310) = &LAB_0011750c;
        if (lVar3 + 0x310 == 0) {
          iVar2 = -0x16;
        }
        else {
          iVar2 = blocking_notifier_chain_register(&mmsys_notifier_list,lVar3 + 0x310);
          if (iVar2 == 0) {
            return 0;
          }
        }
        printk(&DAT_00104951);
        return iVar2;
      }
      pcVar5 = "failed to add devfreq dev with module-dvfs governor\n";
    }
    else {
      pcVar5 = "dvfs: Invalid operating-points in device tree.\n";
    }
    _dev_err(lVar1,pcVar5);
    dev_pm_opp_of_remove_table(lVar1);
    iVar2 = -0x16;
  }
  return iVar2;
}


