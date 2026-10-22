====================mmsys_dvfs_probe@00109568====================
// signature: undefined mmsys_dvfs_probe(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int mmsys_dvfs_probe(long param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  ulong uVar5;
  undefined *puVar6;
  undefined8 uVar7;
  undefined8 *in_x18;
  undefined8 *puVar8;
  undefined8 uVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar9 = *(undefined8 *)(param_1 + 0x310);
  lVar1 = param_1 + 0x10;
  printk(&DAT_001062ec);
  lVar3 = devm_kmalloc(lVar1,0x80,0xdc0);
  if (lVar3 == 0) {
    return -0xc;
  }
  __mutex_init(lVar3 + 0x48,"&mmsys->lock",&g_mmreg_map);
  iVar2 = dev_pm_opp_of_add_table(lVar1);
  if (iVar2 == 0) {
    if ((((uint)_cpu_hwcaps >> 0x17 & 1) != 0) ||
       (uVar7 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
      uVar7 = 0x68000000000f07;
    }
    lVar4 = __ioremap(0x62600000,0x23d,uVar7);
    if (lVar4 == 0) {
      puVar6 = &DAT_00106611;
    }
    else {
      if ((((uint)_cpu_hwcaps >> 0x17 & 1) != 0) ||
         (uVar7 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
        uVar7 = 0x68000000000f07;
      }
      g_mmreg_map = lVar4;
      lVar4 = __ioremap(0x322a0000,0x151,uVar7);
      if (lVar4 == 0) {
        puVar6 = &DAT_0010475b;
      }
      else {
        lVar4 = of_parse_phandle(uVar9,"sprd,topdvfs_controller",0);
        if (lVar4 == 0) {
          printk(&DAT_00106306);
          return -0x16;
        }
        DAT_00100c20 = syscon_node_to_regmap();
        if (0xfffffffffffff000 < DAT_00100c20) {
          printk(&DAT_001054dc);
          return -0x13;
        }
        uVar9 = 0x68000000000f07;
        if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
           (uVar9 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
          uVar9 = 0x68000000000f07;
        }
        lVar4 = __ioremap(0x62200000,0x151,uVar9);
        if (lVar4 != 0) {
          uVar9 = 0x68000000000f07;
          if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
             (uVar9 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
            uVar9 = 0x68000000000f07;
          }
          DAT_00100c28 = lVar4;
          lVar4 = __ioremap(0x327d0000,5,uVar9);
          if (lVar4 != 0) {
            uVar9 = 0x68000000000f07;
            if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
               (uVar9 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
              uVar9 = 0x68000000000f07;
            }
            DAT_00100c38 = lVar4;
            lVar4 = __ioremap(0x327e0024,5,uVar9);
            if (lVar4 != 0) {
              DAT_00100c30 = lVar4;
              iVar2 = dev_pm_opp_of_add_table(lVar1);
              if (iVar2 == 0) {
                *(long *)(param_1 + 0xa8) = lVar3;
                uVar5 = devm_devfreq_add_device(lVar1,&mmsys_dvfs_profile,"mmsys_dvfs",0);
                *(ulong *)(lVar3 + 0x10) = uVar5;
                if (0xfffffffffffff000 < uVar5) {
                  _dev_err(lVar1,"failed to add devfreq dev with mmsys-dvfs governor\n");
                  return *(int *)(lVar3 + 0x10);
                }
                puVar8 = (undefined8 *)(lVar3 + 0x68);
                *puVar8 = &LAB_00117504;
                *(undefined4 *)(lVar3 + 0x78) = 0;
                if (puVar8 == (undefined8 *)0x0) {
                  return 0;
                }
                blocking_notifier_chain_register(&mmsys_notifier_list,puVar8);
                return 0;
              }
              goto LAB_001097c0;
            }
          }
        }
        puVar6 = &DAT_00105fa4;
      }
    }
    printk(puVar6);
    return 0;
  }
LAB_001097c0:
  _dev_err(lVar1,"mmsys_dvfs: Invalid operating-points in device tree.\n");
  return iVar2;
}


