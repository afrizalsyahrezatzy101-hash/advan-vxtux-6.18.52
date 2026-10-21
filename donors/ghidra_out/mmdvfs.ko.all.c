====================__cfi_check_fail@00108000====================
// signature: undefined __cfi_check_fail(void)

byte * __cfi_check_fail(byte *param_1)

{
  uint uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((param_1 != (byte *)0x0) && (4 < *param_1)) {
    return param_1;
  }
  __ubsan_handle_cfi_check_fail_abort(2);
  *extraout_x18 = mtx_dvfs_init;
  uVar1 = register_ip_dvfs_ops(&mtx_dvfs_ops);
  if (uVar1 == 0) {
    uVar1 = devfreq_add_governor(&mtx_dvfs_gov);
    if (uVar1 == 0) {
      uVar1 = __platform_driver_register(&mtx_dvfs_driver,&__this_module);
      if (uVar1 == 0) {
        return (byte *)0x0;
      }
      devfreq_remove_governor(&mtx_dvfs_gov);
      return (byte *)(ulong)uVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"mtx_dvfs_init",(byte *)(ulong)uVar1);
  return (byte *)(ulong)uVar1;
}



====================mtx_dvfs_init@00108030====================
// signature: undefined mtx_dvfs_init(void)

int mtx_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&mtx_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&mtx_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&mtx_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&mtx_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"mtx_dvfs_init",iVar1);
  return iVar1;
}



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



====================dvfs_remove@00108220====================
// signature: undefined dvfs_remove(void)

undefined8 dvfs_remove(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target@00108250====================
// signature: undefined dvfs_gov_get_target(void)

undefined8 dvfs_gov_get_target(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_00104ee6,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_001062be,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler@001082f0====================
// signature: undefined dvfs_gov_event_handler(void)

undefined8 dvfs_gov_event_handler(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("MTX_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group);
    return uVar1;
  }
  return 0;
}



====================mtx_dvfs_exit@001083a4====================
// signature: undefined mtx_dvfs_exit(void)

void mtx_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&mtx_dvfs_driver);
  iVar1 = devfreq_remove_governor(&mtx_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"mtx_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&mtx_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"mtx_dvfs_exit",iVar1);
  }
  return;
}



====================__cfi_check@00109000====================
// signature: undefined __cfi_check(void)

void __cfi_check(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  code *pcVar2;
  undefined1 *puVar3;
  
  if (0x2b3a43e29242444 < param_1) {
    if (param_1 < 0x362db8165d0ff4ec) {
      if (0x24a34366d538acc5 < param_1) {
        if (param_1 != 0x24a34366d538acc6) {
          if (param_1 != 0x2e12bb038fa0cb35) {
            if ((param_1 == 0x2ef299c34abb1f58) &&
               ((param_2 - 0x11782cU >> 2 | param_2 << 0x3e) < 0xf)) {
              return;
            }
            goto LAB_0010947c;
          }
          pcVar2 = __typeid__ZTSFiP7devfreqPjE_global_addr;
LAB_00109434:
          if (((ulong)(param_2 - (long)pcVar2) >> 2 | param_2 << 0x3e) < 0xe) {
            return;
          }
          goto LAB_0010947c;
        }
        pcVar2 = __typeid__ZTSFvP7devfreqE_global_addr;
LAB_001093f0:
        if (((ulong)(param_2 - (long)pcVar2) >> 2 | param_2 << 0x3e) < 0x10) {
          return;
        }
        goto LAB_0010947c;
      }
      if (param_1 == 0x2b3a43e29242445) {
        lVar1 = 0x117464;
LAB_00109358:
        if (param_2 == lVar1) {
          return;
        }
        goto LAB_0010947c;
      }
      if (param_1 != 0x82c2068e03d2969) {
        if (param_1 != 0x1e3d0ffe3d0499e9) goto LAB_0010947c;
        puVar3 = &LAB_00117510;
        goto LAB_0010933c;
      }
      pcVar2 = (code *)&LAB_001174cc;
    }
    else if (param_1 < 0x6fcfedb2e945b2c5) {
      if (param_1 == 0x362db8165d0ff4ec) {
        if ((param_2 - 0x1176d4U >> 2 | param_2 << 0x3e) < 0x1c) {
          return;
        }
        goto LAB_0010947c;
      }
      if (param_1 == 0x3837a203ab2c4d30) {
        pcVar2 = (code *)&LAB_0011777c;
      }
      else {
        if (param_1 != 0x67ed40f9fdf2c861) goto LAB_0010947c;
        pcVar2 = __typeid__ZTSFiP7devfreqP14ip_dvfs_statusE_global_addr;
      }
    }
    else {
      if (param_1 != 0x6fcfedb2e945b2c5) {
        if (param_1 == 0x7d57972cd3914fb1) {
          if ((param_2 - 0x117504U >> 2 | param_2 << 0x3e) < 3) {
            return;
          }
          goto LAB_0010947c;
        }
        if (param_1 != 0x7e04a0fb7ad8bcd5) goto LAB_0010947c;
        lVar1 = 0x117468;
        goto LAB_00109358;
      }
      pcVar2 = (code *)&LAB_001174ac;
    }
    if (((ulong)(param_2 - (long)pcVar2) >> 2 | param_2 << 0x3e) < 8) {
      return;
    }
    goto LAB_0010947c;
  }
  if (param_1 < -0x3557318fb8d07b71) {
    if (param_1 < -0x65f71bf4e00bf722) {
      if (param_1 != -0x6bc05774523e60e8) {
        if (param_1 != -0x66d421a09845f9f8) goto LAB_0010947c;
        puVar3 = &LAB_00117568;
LAB_0010933c:
        if (((ulong)(param_2 - (long)puVar3) >> 2 | param_2 << 0x3e) < 0x16) {
          return;
        }
        goto LAB_0010947c;
      }
      puVar3 = &LAB_001174ec;
    }
    else {
      if (param_1 != -0x65f71bf4e00bf722) {
        if (param_1 == -0x5bbe2f46531c8194) {
          if ((param_2 - 0x1175f8U >> 2 | param_2 << 0x3e) < 0x22) {
            return;
          }
          goto LAB_0010947c;
        }
        if (param_1 != -0x54ee0a7d1e226559) goto LAB_0010947c;
        pcVar2 = (code *)&LAB_00117680;
        goto LAB_00109384;
      }
      puVar3 = &LAB_001174fc;
    }
LAB_00109498:
    if (((ulong)(param_2 - (long)puVar3) >> 2 | param_2 << 0x3e) < 2) {
      return;
    }
  }
  else {
    if (param_1 < -0x1ea9651699bc9307) {
      if (param_1 == -0x3557318fb8d07b71) {
        pcVar2 = (code *)&LAB_0011746c;
        goto LAB_001093f0;
      }
      if (param_1 != -0x2c093afe658a0552) {
        if (param_1 != -0x253e7d793bb14cad) goto LAB_0010947c;
        puVar3 = &LAB_001174f4;
        goto LAB_00109498;
      }
      pcVar2 = (code *)&LAB_0011769c;
    }
    else {
      if (param_1 == -0x1ea9651699bc9307) {
        if ((param_2 - 0x1177fcU >> 2 | param_2 << 0x3e) < 0xc) {
          return;
        }
        goto LAB_0010947c;
      }
      if (param_1 == -0x1a3896a5f53eb4f8) {
        pcVar2 = __typeid__ZTSFiP7devfreqP15ip_dvfs_map_cfgE_global_addr;
        goto LAB_00109434;
      }
      if (param_1 != -0xee11ff251fad17b) goto LAB_0010947c;
      pcVar2 = __typeid__ZTSFiP7devfreqmmjE_global_addr;
    }
LAB_00109384:
    if (((ulong)(param_2 - (long)pcVar2) >> 2 | param_2 << 0x3e) < 7) {
      return;
    }
  }
LAB_0010947c:
  __cfi_check_fail(param_3);
  return;
}



====================mmsys_dvfs_init@001094ac====================
// signature: undefined mmsys_dvfs_init(void)

int mmsys_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&mmsys_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = sprd_mm_pw_notify_register(&dvfs_power_notifier);
    if (iVar1 == 0) {
      iVar1 = devfreq_add_governor(&mmsys_dvfs_gov);
      if (iVar1 == 0) {
        iVar1 = __platform_driver_register(&mmsys_dvfs_driver,&__this_module);
        if (iVar1 == 0) {
          return 0;
        }
        devfreq_remove_governor(&mmsys_dvfs_gov);
        return iVar1;
      }
      puVar2 = &DAT_00106638;
    }
    else {
      puVar2 = &DAT_001065ed;
    }
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"mmsys_dvfs_init",iVar1);
  return iVar1;
}



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



====================mmsys_dvfs_remove@001098b8====================
// signature: undefined mmsys_dvfs_remove(void)

undefined8 mmsys_dvfs_remove(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104ca4,"mmsys_dvfs_remove");
  return 0;
}



====================mmsys_dvfs_notify_callback@001098e8====================
// signature: undefined mmsys_dvfs_notify_callback(void)

undefined8 mmsys_dvfs_notify_callback(long param_1,long param_2)

{
  undefined8 *in_x18;
  code *pcVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010670c);
  if (param_2 == 2) {
    if (*(long *)(param_1 + -0x48) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x48) + 0x90);
  }
  else {
    if (param_2 != 1) {
      return 0xffffffea;
    }
    if (*(long *)(param_1 + -0x48) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x48) + 0x88);
  }
  if (pcVar1 != (code *)0x0) {
    if (0xf < ((ulong)(pcVar1 + -0x1177bc) >> 2 | (long)pcVar1 << 0x3e)) {
      __cfi_slowpath(0x24a34366d538acc6,pcVar1);
    }
    (*pcVar1)(*(undefined8 *)(param_1 + -0x58));
  }
  return 1;
}



====================mmsys_dvfs_target@00109998====================
// signature: undefined mmsys_dvfs_target(void)

undefined8 mmsys_dvfs_target(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105db0,"mmsys_dvfs_target");
  return 0;
}



====================mmsys_dvfs_get_dev_status@001099c8====================
// signature: undefined mmsys_dvfs_get_dev_status(void)

undefined8 mmsys_dvfs_get_dev_status(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105589);
  return 0;
}



====================mmsys_dvfs_get_cur_freq@001099f0====================
// signature: undefined mmsys_dvfs_get_cur_freq(void)

undefined8 mmsys_dvfs_get_cur_freq(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001056ac);
  return 0;
}



====================mmsys_dvfs_power@00109a18====================
// signature: undefined mmsys_dvfs_power(void)

undefined8 mmsys_dvfs_power(undefined8 param_1,long param_2)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104c1d,"mmsys_dvfs_power",param_2);
  uVar1 = 1;
  if (param_2 != 1) {
    uVar1 = 2;
  }
  mmsys_notify_call_chain(uVar1);
  printk(&DAT_0010616e,"mmsys_dvfs_power");
  return 0;
}



====================mmsys_dvfs_exit@00109a7c====================
// signature: undefined mmsys_dvfs_exit(void)

void mmsys_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&mmsys_dvfs_driver);
  iVar1 = devfreq_remove_governor(&mmsys_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"mmsys_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&mmsys_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"mmsys_dvfs_exit",iVar1);
  }
  iVar1 = sprd_mm_pw_notify_unregister(&dvfs_power_notifier);
  if (iVar1 != 0) {
    printk(&DAT_001060cb,"mmsys_dvfs_exit",iVar1);
  }
  return;
}



====================cpp_dvfs_init@00109b24====================
// signature: undefined cpp_dvfs_init(void)

int cpp_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&cpp_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&cpp_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&cpp_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&cpp_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"cpp_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.71@00109bc0====================
// signature: undefined dvfs_probe.71(void)

int dvfs_probe_71(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_81,"cpp_dvfs",0);
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



====================dvfs_remove.72@00109d14====================
// signature: undefined dvfs_remove.72(void)

undefined8 dvfs_remove_72(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.86@00109d44====================
// signature: undefined dvfs_gov_get_target.86(void)

undefined8 dvfs_gov_get_target_86(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_0010620e,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_00105175,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.87@00109de4====================
// signature: undefined dvfs_gov_event_handler.87(void)

undefined8 dvfs_gov_event_handler_87(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_90);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_91);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("CPP_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_90);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_91);
    return uVar1;
  }
  return 0;
}



====================cpp_dvfs_exit@00109e98====================
// signature: undefined cpp_dvfs_exit(void)

void cpp_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&cpp_dvfs_driver);
  iVar1 = devfreq_remove_governor(&cpp_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"cpp_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&cpp_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"cpp_dvfs_exit",iVar1);
  }
  return;
}



====================fd_dvfs_init@00109f14====================
// signature: undefined fd_dvfs_init(void)

int fd_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&fd_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&fd_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&fd_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&fd_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"fd_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.142@00109fb0====================
// signature: undefined dvfs_probe.142(void)

int dvfs_probe_142(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_152,"fd_dvfs",0);
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



====================dvfs_remove.143@0010a104====================
// signature: undefined dvfs_remove.143(void)

undefined8 dvfs_remove_143(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.157@0010a134====================
// signature: undefined dvfs_gov_get_target.157(void)

undefined8 dvfs_gov_get_target_157(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_00105383,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_00105928,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.158@0010a1d4====================
// signature: undefined dvfs_gov_event_handler.158(void)

undefined8 dvfs_gov_event_handler_158(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_161);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_162);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("FD_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_161);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_162);
    return uVar1;
  }
  return 0;
}



====================fd_dvfs_exit@0010a288====================
// signature: undefined fd_dvfs_exit(void)

void fd_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&fd_dvfs_driver);
  iVar1 = devfreq_remove_governor(&fd_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"fd_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&fd_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"fd_dvfs_exit",iVar1);
  }
  return;
}



====================dcam_axi_dvfs_init@0010a304====================
// signature: undefined dcam_axi_dvfs_init(void)

int dcam_axi_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&dcam_axi_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&dcam_axi_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&dcam_axi_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&dcam_axi_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"dcam_axi_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.213@0010a3a0====================
// signature: undefined dvfs_probe.213(void)

int dvfs_probe_213(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_223,"dcam_axi_dvfs",0);
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



====================dvfs_remove.214@0010a4f4====================
// signature: undefined dvfs_remove.214(void)

undefined8 dvfs_remove_214(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.228@0010a524====================
// signature: undefined dvfs_gov_get_target.228(void)

undefined8 dvfs_gov_get_target_228(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_0010673b,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_001048f9,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.229@0010a5c4====================
// signature: undefined dvfs_gov_event_handler.229(void)

undefined8 dvfs_gov_event_handler_229(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_232);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_233);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("AXI_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_232);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_233);
    return uVar1;
  }
  return 0;
}



====================dcam_axi_dvfs_exit@0010a678====================
// signature: undefined dcam_axi_dvfs_exit(void)

void dcam_axi_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&dcam_axi_dvfs_driver);
  iVar1 = devfreq_remove_governor(&dcam_axi_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"dcam_axi_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&dcam_axi_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"dcam_axi_dvfs_exit",iVar1);
  }
  return;
}



====================jpg_dvfs_init@0010a6f4====================
// signature: undefined jpg_dvfs_init(void)

int jpg_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&jpg_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&jpg_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&jpg_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&jpg_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"jpg_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.284@0010a790====================
// signature: undefined dvfs_probe.284(void)

int dvfs_probe_284(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_294,"jpg_dvfs",0);
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



====================dvfs_remove.285@0010a8e4====================
// signature: undefined dvfs_remove.285(void)

undefined8 dvfs_remove_285(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.299@0010a914====================
// signature: undefined dvfs_gov_get_target.299(void)

undefined8 dvfs_gov_get_target_299(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_0010491c,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_001053a4,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.300@0010a9b4====================
// signature: undefined dvfs_gov_event_handler.300(void)

undefined8 dvfs_gov_event_handler_300(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_303);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_304);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("JPG_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_303);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_304);
    return uVar1;
  }
  return 0;
}



====================jpg_dvfs_exit@0010aa68====================
// signature: undefined jpg_dvfs_exit(void)

void jpg_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&jpg_dvfs_driver);
  iVar1 = devfreq_remove_governor(&jpg_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"jpg_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&jpg_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"jpg_dvfs_exit",iVar1);
  }
  return;
}



====================get_mmsys_power_show@0010aae4====================
// signature: undefined get_mmsys_power_show(void)

long get_mmsys_power_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*plVar4 == 0) {
    iVar2 = sprintf(param_3,"%lu\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_mmsys_power_store@0010ab74====================
// signature: undefined set_mmsys_power_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_mmsys_power_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  int iVar1;
  code *pcVar2;
  undefined8 *in_x18;
  ulong *puVar3;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  printk(&DAT_0010519c,"set_mmsys_power_store");
  puVar3 = *(ulong **)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar1 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar1 == 1) {
    if (puVar3 != (ulong *)0x0) {
      *puVar3 = local_50;
      if ((puVar3[4] != 0) && (pcVar2 = *(code **)(puVar3[4] + 0x128), pcVar2 != (code *)0x0)) {
        if (0xe < ((ulong)(pcVar2 + -0x11782c) >> 2 | (long)pcVar2 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar2);
        }
        (*pcVar2)(param_1 + -0x30,local_50 & 0xffffffff);
        goto LAB_0010ac3c;
      }
    }
    printk(&DAT_001047b2,"set_mmsys_power_store");
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
LAB_0010ac3c:
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_status_show@0010acb8====================
// signature: undefined get_sys_status_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_sys_status_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  int iVar10;
  code *pcVar11;
  undefined8 *in_x18;
  long lVar12;
  undefined8 unaff_x30;
  undefined8 uVar13;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  ulong local_58;
  ulong local_50;
  ulong local_48;
  ulong uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_48 = 0;
  uStack_40 = 0;
  local_58 = 0;
  local_50 = 0;
  local_68 = 0;
  local_60 = 0;
  local_78 = 0;
  local_70 = 0;
  local_88 = 0;
  local_80 = 0;
  local_98 = 0;
  local_90 = 0;
  local_a8 = 0;
  local_a0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  lVar12 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  if (((lVar12 != 0) && (lVar12 = *(long *)(lVar12 + 0x20), lVar12 != 0)) &&
     (*(long *)(lVar12 + 0x128) != 0)) {
    pcVar11 = *(code **)(lVar12 + 0x80);
    if (7 < ((ulong)(pcVar11 + -0x11779c) >> 2 | (long)pcVar11 << 0x3e)) {
      __cfi_slowpath(0x67ed40f9fdf2c861,pcVar11);
    }
    iVar10 = (*pcVar11)(param_1 + -0x30,&local_b8);
    if (0 < iVar10) {
      param_3[0x58] = '\n';
      uVar9 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._80_8_;
      uVar8 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._72_8_;
      uVar7 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._64_8_;
      uVar13 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._56_8_;
      uVar6 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._48_8_;
      uVar5 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._40_8_;
      uVar4 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._32_8_;
      uVar3 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._24_8_;
      uVar2 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._16_8_;
      uVar1 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._8_8_;
      param_3[0x59] = '\0';
      *(undefined8 *)param_3 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._0_8_;
      *(undefined8 *)(param_3 + 8) = uVar1;
      *(undefined8 *)(param_3 + 0x20) = uVar4;
      *(undefined8 *)(param_3 + 0x28) = uVar5;
      *(undefined8 *)(param_3 + 0x10) = uVar2;
      *(undefined8 *)(param_3 + 0x18) = uVar3;
      *(undefined8 *)(param_3 + 0x48) = uVar8;
      *(undefined8 *)(param_3 + 0x50) = uVar9;
      *(undefined8 *)(param_3 + 0x38) = uVar13;
      *(undefined8 *)(param_3 + 0x40) = uVar7;
      *(undefined8 *)(param_3 + 0x30) = uVar6;
      uVar13 = local_90;
      iVar10 = sprintf(param_3 + 0x59,"%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
                       local_70,local_a0,local_88,local_80,local_78,local_98,local_90,local_68,
                       local_60);
      uVar6 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._40_8_;
      uVar5 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._32_8_;
      uVar4 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._24_8_;
      uVar3 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._16_8_;
      uVar2 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._8_8_;
      uVar1 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._0_8_;
      lVar12 = (long)iVar10;
      param_3[lVar12 + 0x89] = '\0';
      *(undefined8 *)(param_3 + lVar12 + 0x59) = uVar1;
      *(undefined8 *)(param_3 + lVar12 + 0x61) = uVar2;
      *(undefined8 *)(param_3 + lVar12 + 0x69) = uVar3;
      *(undefined8 *)(param_3 + lVar12 + 0x81) = uVar6;
      *(undefined8 *)(param_3 + lVar12 + 0x79) = uVar5;
      *(undefined8 *)(param_3 + lVar12 + 0x71) = uVar4;
      iVar10 = sprintf(param_3 + lVar12 + 0x89,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",uStack_40 & 0xffffffff
                       ,local_58 & 0xffffffff,local_50 >> 0x20,local_48 & 0xffffffff,
                       local_48 >> 0x20,local_58 >> 0x20,
                       CONCAT44((int)((ulong)uVar13 >> 0x20),(undefined4)local_50));
      lVar12 = lVar12 + 0x89 + (long)iVar10;
      goto LAB_0010ae68;
    }
  }
  uVar6 = s_Error_Get_ip_status_at_wrong_tim_001060f5._56_8_;
  uVar5 = s_Error_Get_ip_status_at_wrong_tim_001060f5._48_8_;
  uVar3 = s_Error_Get_ip_status_at_wrong_tim_001060f5._24_8_;
  uVar2 = s_Error_Get_ip_status_at_wrong_tim_001060f5._8_8_;
  uVar1 = s_Error_Get_ip_status_at_wrong_tim_001060f5._0_8_;
  *(undefined8 *)(param_3 + 0x10) = s_Error_Get_ip_status_at_wrong_tim_001060f5._16_8_;
  *(undefined8 *)(param_3 + 0x18) = uVar3;
  uVar13 = s_Error_Get_ip_status_at_wrong_tim_001060f5._64_8_;
  uVar4 = s_Error_Get_ip_status_at_wrong_tim_001060f5._40_8_;
  uVar3 = s_Error_Get_ip_status_at_wrong_tim_001060f5._32_8_;
  *(undefined8 *)param_3 = uVar1;
  *(undefined8 *)(param_3 + 8) = uVar2;
  *(undefined8 *)(param_3 + 0x38) = uVar6;
  *(undefined8 *)(param_3 + 0x40) = uVar13;
  *(undefined8 *)(param_3 + 0x28) = uVar4;
  *(undefined8 *)(param_3 + 0x30) = uVar5;
  *(undefined8 *)(param_3 + 0x20) = uVar3;
  lVar12 = 0x47;
LAB_0010ae68:
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_38) {
    return lVar12;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================set_sys_status_store@0010aec8====================
// signature: undefined set_sys_status_store(void)

undefined8 set_sys_status_store(void)

{
  return 0;
}



====================set_sys_dvfs_hold_en_store@0010aed0====================
// signature: undefined set_sys_dvfs_hold_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_dvfs_hold_en_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x2c) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x108), pcVar3 == (code *)0x0)) {
      printk(&DAT_001047b2,"set_sys_dvfs_hold_en_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x2c);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_dvfs_hold_en_show@0010affc====================
// signature: undefined get_sys_dvfs_hold_en_show(void)

long get_sys_dvfs_hold_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x2c) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_sys_sw_dvfs_en_store@0010b08c====================
// signature: undefined set_sys_sw_dvfs_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_sw_dvfs_en_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x28) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x100), pcVar3 == (code *)0x0)) {
      printk(&DAT_001047b2,"set_sys_sw_dvfs_en_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x28);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_sw_dvfs_en_show@0010b1b8====================
// signature: undefined get_sys_sw_dvfs_en_show(void)

long get_sys_sw_dvfs_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x28) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_sys_dvfs_clk_gate_ctrl_store@0010b248====================
// signature: undefined set_sys_dvfs_clk_gate_ctrl_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_dvfs_clk_gate_ctrl_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x30) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x110), pcVar3 == (code *)0x0)) {
      printk(&DAT_001047b2,"set_sys_dvfs_clk_gate_ctrl_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x30);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_dvfs_clk_gate_ctrl_show@0010b374====================
// signature: undefined get_sys_dvfs_clk_gate_ctrl_show(void)

long get_sys_dvfs_clk_gate_ctrl_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x30) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_sys_dvfs_wait_window_store@0010b404====================
// signature: undefined set_sys_dvfs_wait_window_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_dvfs_wait_window_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x34) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x118), pcVar3 == (code *)0x0)) {
      printk(&DAT_001047b2,"set_sys_dvfs_wait_window_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x34);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_dvfs_wait_window_show@0010b530====================
// signature: undefined get_sys_dvfs_wait_window_show(void)

long get_sys_dvfs_wait_window_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x34) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_sys_dvfs_min_volt_store@0010b5c0====================
// signature: undefined set_sys_dvfs_min_volt_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
set_sys_dvfs_min_volt_store(long param_1,undefined8 param_2,char *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  iVar2 = sscanf(param_3,"%lu\n",&local_50);
  if (iVar2 == 1) {
    *(undefined4 *)(lVar4 + 0x38) = (undefined4)local_50;
    if ((*(long *)(lVar4 + 0x20) == 0) ||
       (pcVar3 = *(code **)(*(long *)(lVar4 + 0x20) + 0x120), pcVar3 == (code *)0x0)) {
      printk(&DAT_001059ba,"set_sys_dvfs_min_volt_store");
    }
    else {
      uVar1 = (undefined4)local_50;
      if (0xb < ((ulong)(pcVar3 + -0x1177fc) >> 2 | (long)pcVar3 << 0x3e)) {
        __cfi_slowpath(0xe1569ae966436cf9,pcVar3);
        uVar1 = *(undefined4 *)(lVar4 + 0x38);
      }
      (*pcVar3)(param_1 + -0x30,uVar1);
    }
  }
  else {
    param_4 = 0xffffffffffffffea;
  }
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_48) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_sys_dvfs_min_volt_show@0010b6ec====================
// signature: undefined get_sys_dvfs_min_volt_show(void)

long get_sys_dvfs_min_volt_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x28) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================mmsys_dvfs_gov_get_target@0010b77c====================
// signature: undefined mmsys_dvfs_gov_get_target(void)

undefined8 mmsys_dvfs_gov_get_target(long param_1,ulong *param_2)

{
  ulong uVar1;
  ulong uVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  printk(&DAT_0010523e);
  if (*(int *)(lVar3 + 0x28) == 0) {
    uVar2 = *(ulong *)(param_1 + 0x4d0);
    if (*param_2 <= uVar2 || uVar2 == 0) {
      uVar2 = *param_2;
    }
    uVar1 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar2) {
      uVar1 = uVar2;
    }
    *param_2 = uVar1;
  }
  else {
    printk(&DAT_00104c2f,"mmsys_dvfs_gov_get_target");
    uVar1 = *param_2;
  }
  printk(&DAT_001045a4,"mmsys_dvfs_gov_get_target",uVar1);
  return 0;
}



====================mmsys_dvfs_gov_event_handler@0010b820====================
// signature: undefined mmsys_dvfs_gov_event_handler(void)

undefined8 mmsys_dvfs_gov_event_handler(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_366);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_367);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("MMSYS_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x20) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_366);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_367);
    return uVar1;
  }
  return 0;
}



====================dcam_if_dvfs_init@0010b8d4====================
// signature: undefined dcam_if_dvfs_init(void)

int dcam_if_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&dcam_if_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&dcam_if_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&dcam_if_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&dcam_if_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"dcam_if_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.389@0010b970====================
// signature: undefined dvfs_probe.389(void)

int dvfs_probe_389(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_399,"dcam_if_dvfs",0);
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



====================dvfs_remove.390@0010bac4====================
// signature: undefined dvfs_remove.390(void)

undefined8 dvfs_remove_390(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.404@0010baf4====================
// signature: undefined dvfs_gov_get_target.404(void)

undefined8 dvfs_gov_get_target_404(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_00105fc3,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  if (uVar2 < 0x124f8000) {
    *param_2 = 0x124f8000;
    printk(&DAT_0010481c,"dvfs_gov_get_target",0x124f8000);
    uVar2 = *param_2;
  }
  printk(&DAT_0010643f,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.405@0010bbc8====================
// signature: undefined dvfs_gov_event_handler.405(void)

undefined8 dvfs_gov_event_handler_405(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_408);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_409);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("DCAM_IF_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_408);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_409);
    return uVar1;
  }
  return 0;
}



====================dcam_if_dvfs_exit@0010bc7c====================
// signature: undefined dcam_if_dvfs_exit(void)

void dcam_if_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&dcam_if_dvfs_driver);
  iVar1 = devfreq_remove_governor(&dcam_if_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"dcam_if_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&dcam_if_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"dcam_if_dvfs_exit",iVar1);
  }
  return;
}



====================isp_dvfs_init@0010bcf8====================
// signature: undefined isp_dvfs_init(void)

int isp_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&isp_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&isp_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&isp_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&isp_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"isp_dvfs_init",iVar1);
  return iVar1;
}



====================dvfs_probe.461@0010bd94====================
// signature: undefined dvfs_probe.461(void)

int dvfs_probe_461(long param_1)

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
      uVar4 = devm_devfreq_add_device(lVar1,&dvfs_profile_471,"isp_dvfs",0);
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



====================dvfs_remove.462@0010bee8====================
// signature: undefined dvfs_remove.462(void)

undefined8 dvfs_remove_462(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104aa7,"dvfs_remove");
  return 0;
}



====================dvfs_gov_get_target.476@0010bf18====================
// signature: undefined dvfs_gov_get_target.476(void)

undefined8 dvfs_gov_get_target_476(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_001064dd,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_0010486e,"dvfs_gov_get_target",uVar2);
  return 0;
}



====================dvfs_gov_event_handler.477@0010bfb8====================
// signature: undefined dvfs_gov_event_handler.477(void)

undefined8 dvfs_gov_event_handler_477(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_480);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_481);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("ISP_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_480);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_481);
    return uVar1;
  }
  return 0;
}



====================isp_dvfs_exit@0010c06c====================
// signature: undefined isp_dvfs_exit(void)

void isp_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&isp_dvfs_driver);
  iVar1 = devfreq_remove_governor(&isp_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"isp_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&isp_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"isp_dvfs_exit",iVar1);
  }
  return;
}



====================mmsys_adjust_target_freq@0010c0e8====================
// signature: undefined mmsys_adjust_target_freq(void)

undefined8 mmsys_adjust_target_freq(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  return 0;
}



====================mmsys_set_fix_dvfs_value@0010c118====================
// signature: undefined mmsys_set_fix_dvfs_value(void)

undefined8 mmsys_set_fix_dvfs_value(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,param_1 >> 0x1f | (param_1 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  return 0;
}



====================top_mm_dvfs_current_volt@0010c174====================
// signature: undefined top_mm_dvfs_current_volt(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void top_mm_dvfs_current_volt(void)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_1c;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  local_1c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_1c);
  if (iVar1 == 0) {
    local_1c = local_1c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_1c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  if (___stack_chk_guard == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}



====================mm_dvfs_read_current_volt@0010c21c====================
// signature: undefined mm_dvfs_read_current_volt(void)

uint mm_dvfs_read_current_volt(void)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  return uVar1 >> 0x18 & 7;
}



====================register_ip_dvfs_ops@0010c270====================
// signature: undefined register_ip_dvfs_ops(void)

undefined4 register_ip_dvfs_ops(undefined8 *param_1)

{
  ulong uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined4 uVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == (undefined8 *)0x0) {
    uVar3 = 0xffffffea;
    puVar2 = &DAT_00105966;
  }
  else {
    printk(&DAT_00105ee7,"register_ip_dvfs_ops");
    uVar1 = get_ip_dvfs_ops(param_1 + 2);
    if (uVar1 < 0xfffffffffffff001) {
      printk(&DAT_00105df5,"register_ip_dvfs_ops",uVar1 + 0x10);
      uVar3 = 0xffffffea;
    }
    else {
      mutex_lock(&ip_dvfsops_list_lock);
      puVar2 = ip_dvfs_ops_list;
      uVar1 = __list_add_valid(param_1,&ip_dvfs_ops_list,ip_dvfs_ops_list);
      if ((uVar1 & 1) != 0) {
        *(undefined8 **)(puVar2 + 8) = param_1;
        *param_1 = puVar2;
        param_1[1] = &ip_dvfs_ops_list;
        ip_dvfs_ops_list = (undefined *)param_1;
      }
      mutex_unlock(&ip_dvfsops_list_lock);
      uVar3 = 0;
    }
    puVar2 = &DAT_00105144;
  }
  printk(puVar2,"register_ip_dvfs_ops");
  return uVar3;
}



====================get_ip_dvfs_ops@0010c358====================
// signature: undefined get_ip_dvfs_ops(void)

undefined ** get_ip_dvfs_ops(char *param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined **ppuVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105e1e,"get_ip_dvfs_ops");
  if ((param_1 == (char *)0x0) || ((char *)0xfffffffffffff000 < param_1)) {
    printk(&DAT_0010638d,"get_ip_dvfs_ops");
    ppuVar2 = (undefined **)0xffffffffffffffea;
  }
  else {
    mutex_lock(&ip_dvfsops_list_lock);
    for (ppuVar2 = (undefined **)ip_dvfs_ops_list; ppuVar2 != &ip_dvfs_ops_list;
        ppuVar2 = (undefined **)*ppuVar2) {
      iVar1 = strncmp((char *)(ppuVar2 + 2),param_1,0x10);
      if (iVar1 == 0) {
        printk(&DAT_00104979,"get_ip_dvfs_ops");
        mutex_unlock(&ip_dvfsops_list_lock);
        return ppuVar2;
      }
    }
    mutex_unlock(&ip_dvfsops_list_lock);
    ppuVar2 = (undefined **)0xffffffffffffffed;
  }
  return ppuVar2;
}



====================unregister_ip_dvfs_ops@0010c438====================
// signature: undefined unregister_ip_dvfs_ops(void)

ulong unregister_ip_dvfs_ops(long *param_1)

{
  long lVar1;
  long *plVar2;
  undefined *puVar3;
  undefined8 *in_x18;
  ulong uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == (long *)0x0) {
    uVar4 = 0xffffffea;
    puVar3 = &DAT_00105966;
  }
  else {
    printk(&DAT_0010488c,"unregister_ip_dvfs_ops");
    uVar4 = get_ip_dvfs_ops(param_1 + 2);
    if (uVar4 < 0xfffffffffffff001) {
      mutex_lock(&ip_dvfsops_list_lock);
      uVar4 = __list_del_entry_valid(param_1);
      if ((uVar4 & 1) != 0) {
        lVar1 = *param_1;
        plVar2 = (long *)param_1[1];
        *(long **)(lVar1 + 8) = plVar2;
        *plVar2 = lVar1;
      }
      *param_1 = -0x2152ffffffffff00;
      param_1[1] = -0x2152fffffffffede;
      mutex_unlock(&ip_dvfsops_list_lock);
      uVar4 = 0;
    }
    else {
      printk(&DAT_00104ac5,"unregister_ip_dvfs_ops",param_1 + 2);
    }
    puVar3 = &DAT_001051bb;
  }
  printk(puVar3,"unregister_ip_dvfs_ops");
  return uVar4 & 0xffffffff;
}



====================mmsys_register_notifier@0010c520====================
// signature: undefined mmsys_register_notifier(void)

undefined8 mmsys_register_notifier(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    uVar1 = blocking_notifier_chain_register(&mmsys_notifier_list,param_1);
    return uVar1;
  }
  return 0xffffffea;
}



====================mmsys_unregister_notifier@0010c554====================
// signature: undefined mmsys_unregister_notifier(void)

undefined8 mmsys_unregister_notifier(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    uVar1 = blocking_notifier_chain_unregister(&mmsys_notifier_list,param_1);
    return uVar1;
  }
  return 0xffffffea;
}



====================mmsys_notify_call_chain@0010c588====================
// signature: undefined mmsys_notify_call_chain(void)

undefined8 mmsys_notify_call_chain(int param_1)

{
  int iVar1;
  char cVar2;
  bool bVar3;
  undefined *puVar4;
  undefined8 *in_x18;
  undefined8 uVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010489d,"mmsys_notify_call_chain",param_1);
  printk(&DAT_001057b7,"mmsys_notify_call_chain",mmsys_notify_call_chain_pw_opened);
  if (param_1 == 2) {
    Hint_Prefetch(0x100c40,2,0,1);
    do {
      iVar1 = mmsys_notify_call_chain_pw_opened + -1;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
      if (bVar3) {
        cVar2 = ExclusiveMonitorsStatus();
        mmsys_notify_call_chain_pw_opened = iVar1;
      }
    } while (cVar2 != '\0');
    DataMemoryBarrier(2,3);
    uVar5 = 2;
    if (iVar1 == 0) goto LAB_0010c6b4;
    puVar4 = &DAT_00105c42;
  }
  else {
    if (param_1 != 1) {
      return 0xffffffea;
    }
    Hint_Prefetch(0x100c40,2,0,1);
    do {
      iVar1 = mmsys_notify_call_chain_pw_opened + 1;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
      if (bVar3) {
        cVar2 = ExclusiveMonitorsStatus();
        mmsys_notify_call_chain_pw_opened = iVar1;
      }
    } while (cVar2 != '\0');
    DataMemoryBarrier(2,3);
    if (1 < iVar1) {
      printk(&DAT_001057d3);
      Hint_Prefetch(0x100c40,2,0,1);
      do {
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
        if (bVar3) {
          cVar2 = ExclusiveMonitorsStatus();
          mmsys_notify_call_chain_pw_opened = mmsys_notify_call_chain_pw_opened + -1;
        }
      } while (cVar2 != '\0');
      return 0xffffffe8;
    }
    puVar4 = &DAT_00105748;
    uVar5 = 1;
  }
  printk(puVar4,"mmsys_notify_call_chain",mmsys_notify_call_chain_pw_opened);
LAB_0010c6b4:
  blocking_notifier_call_chain(&mmsys_notifier_list,uVar5,0);
  printk(&DAT_00105ff4,"mmsys_notify_call_chain",param_1);
  return 0;
}



====================InitPQueue@0010c76c====================
// signature: undefined InitPQueue(void)

void InitPQueue(long param_1)

{
  if (param_1 != 0) {
    *(undefined8 *)(param_1 + 0x18) = 0;
  }
  return;
}



====================CreateNode@0010c778====================
// signature: undefined CreateNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void CreateNode(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = (undefined8 *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  uVar1 = param_1[1];
  uVar3 = param_1[2];
  *puVar2 = *param_1;
  puVar2[1] = uVar1;
  puVar2[2] = uVar3;
  puVar2[3] = 0;
  return;
}



====================GetFrontNodeFreq@0010c7c4====================
// signature: undefined GetFrontNodeFreq(void)

undefined8 GetFrontNodeFreq(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_1 + 0x18) == 0) {
    printk(&DAT_00105b07);
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x18) + 8);
  }
  return uVar1;
}



====================Destroy@0010c7fc====================
// signature: undefined Destroy(void)

void Destroy(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(param_1 + 0x18);
  while (lVar1 != 0) {
    lVar1 = *(long *)(lVar1 + 0x18);
    kfree();
  }
  return;
}



====================ShowQueue@0010c834====================
// signature: undefined ShowQueue(void)

void ShowQueue(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  for (puVar1 = *(undefined8 **)(param_1 + 0x18); puVar1 != (undefined8 *)0x0;
      puVar1 = (undefined8 *)puVar1[3]) {
    printk(&DAT_00105421,puVar1[1]);
    printk(&DAT_00106689,*puVar1);
    printk(&DAT_00105991,*(undefined4 *)(puVar1 + 2));
  }
  return;
}



====================IsEmpty@0010c8a8====================
// signature: undefined IsEmpty(void)

bool IsEmpty(long param_1)

{
  return *(long *)(param_1 + 0x18) == 0;
}



====================MatchPid_RemoveInvalidNode@0010c8b8====================
// signature: undefined MatchPid_RemoveInvalidNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 MatchPid_RemoveInvalidNode(long *param_1,long *param_2)

{
  ulong uVar1;
  long *plVar2;
  long *plVar3;
  undefined8 *in_x18;
  long lVar4;
  long *plVar5;
  long lVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar2 = param_1;
  do {
    plVar5 = plVar2;
    plVar2 = (long *)plVar5[3];
    if (plVar2 == (long *)0x0) {
      if ((int)param_2[2] == 0) {
        return 0;
      }
      lVar6 = *param_2;
      uVar1 = param_2[1];
      lVar4 = param_2[2];
      plVar2 = (long *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
      *plVar2 = lVar6;
      plVar2[1] = uVar1;
      plVar2[2] = lVar4;
      plVar2[3] = 0;
      goto LAB_0010c978;
    }
    lVar6 = *plVar2;
  } while (lVar6 != *param_2);
  if ((int)param_2[2] == 0) {
    if (plVar5 == (long *)0x0) {
      return 0;
    }
    plVar5[3] = plVar2[3];
    kfree();
    return 0;
  }
  if (plVar5 != (long *)0x0) {
    plVar5[3] = plVar2[3];
    kfree();
    lVar6 = *param_2;
  }
  uVar1 = param_2[1];
  lVar4 = param_2[2];
  plVar2 = (long *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  *plVar2 = lVar6;
  plVar2[1] = uVar1;
  plVar2[2] = lVar4;
  plVar2[3] = 0;
  do {
    plVar3 = plVar5;
    plVar5 = (long *)plVar3[3];
    if (plVar5 == (long *)0x0) goto LAB_0010c994;
  } while (uVar1 <= (ulong)plVar5[1]);
  plVar2[3] = (long)plVar5;
LAB_0010c994:
  plVar3[3] = (long)plVar2;
  return 0;
  while (uVar1 <= (ulong)param_1[1]) {
LAB_0010c978:
    plVar3 = param_1;
    param_1 = (long *)plVar3[3];
    if (param_1 == (long *)0x0) goto LAB_0010c994;
  }
  plVar2[3] = (long)param_1;
  goto LAB_0010c994;
}



====================DelNode@0010c9c4====================
// signature: undefined DelNode(void)

void DelNode(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    lVar1 = *(long *)(param_1 + 0x18);
    *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(lVar1 + 0x18);
    kfree(lVar1);
  }
  return;
}



====================AddNode@0010c9f4====================
// signature: undefined AddNode(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void AddNode(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  undefined8 *in_x18;
  long lVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = ___stack_chk_guard;
  uVar1 = param_2[1];
  uVar2 = param_2[2];
  uVar5 = *param_2;
  puVar4 = (undefined8 *)kmem_cache_alloc_trace(___dynamic_pr_debug,0x100,0x20);
  *puVar4 = uVar5;
  puVar4[1] = uVar1;
  puVar4[2] = uVar2;
  puVar4[3] = 0;
  do {
    lVar6 = param_1;
    param_1 = *(long *)(lVar6 + 0x18);
    if (param_1 == 0) goto LAB_0010ca70;
  } while ((ulong)param_2[1] <= *(ulong *)(param_1 + 8));
  puVar4[3] = param_1;
LAB_0010ca70:
  *(undefined8 **)(lVar6 + 0x18) = puVar4;
  if (___stack_chk_guard == lVar3) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================dvfs_target@0010caa0====================
// signature: undefined dvfs_target(void)

ulong dvfs_target(long param_1,long *param_2,undefined4 param_3)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar5 = *(long **)(param_1 + 0x98);
  printk(&DAT_00106012,*param_2,plVar5[4]);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = devfreq_recommended_opp(param_1,param_2,param_3);
    if (uVar1 < 0xfffffffffffff001) {
      lVar2 = dev_pm_opp_get_freq(uVar1);
      lVar3 = dev_pm_opp_get_voltage(uVar1);
      dev_pm_opp_put(uVar1);
      if (plVar5[4] != *param_2) {
        mutex_lock(plVar5 + 0x5e);
        pcVar4 = *(code **)(plVar5[0xc] + 0xe8);
        if (6 < ((ulong)(pcVar4 + -0x1176b8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xf11ee00dae052e85,pcVar4);
        }
        (*pcVar4)(plVar5[10],lVar3,lVar2,0);
        plVar5[4] = lVar2;
        plVar5[6] = lVar3;
        mutex_unlock(plVar5 + 0x5e);
      }
      uVar1 = 0;
    }
    else {
      _dev_err(param_1,"Failed to find opp for %lu KHz\n",*param_2);
    }
  }
  return uVar1 & 0xffffffff;
}



====================dvfs_get_dev_status@0010cbec====================
// signature: undefined dvfs_get_dev_status(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void dvfs_get_dev_status(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  long *plVar3;
  undefined8 unaff_x30;
  undefined8 local_38;
  undefined8 local_30;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  plVar3 = *(long **)(param_1 + 0x98);
  local_38 = 0;
  local_30 = 0;
  printk(&DAT_00105589);
  if (*plVar3 == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = devfreq_event_get_event(plVar3[0xb],&local_38);
    if (-1 < (int)uVar1) {
      lVar2 = plVar3[4];
      param_2[1] = local_38;
      param_2[2] = lVar2;
      *param_2 = local_30;
    }
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}



====================dvfs_get_cur_freq@0010cc90====================
// signature: undefined dvfs_get_cur_freq(void)

undefined8 dvfs_get_cur_freq(long param_1,long *param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (**(long **)(param_1 + 0x98) == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    lVar2 = (*(long **)(param_1 + 0x98))[4];
    *param_2 = lVar2;
    printk(&DAT_00104ae7,lVar2);
    uVar1 = 0;
  }
  return uVar1;
}



====================dvfs_notify_callback@0010cce4====================
// signature: undefined dvfs_notify_callback(void)

undefined8 dvfs_notify_callback(long param_1,long param_2)

{
  code *pcVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 == 2) {
    if (*(long *)(param_1 + -0x2b0) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x2b0) + 0x90);
  }
  else {
    if (param_2 != 1) {
      return 0xffffffea;
    }
    if (*(long *)(param_1 + -0x2b0) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x2b0) + 0x88);
  }
  if (pcVar1 != (code *)0x0) {
    if (0xf < ((ulong)(pcVar1 + -0x1177bc) >> 2 | (long)pcVar1 << 0x3e)) {
      __cfi_slowpath(0x24a34366d538acc6,pcVar1);
    }
    (*pcVar1)(*(undefined8 *)(param_1 + -0x2c0));
  }
  return 1;
}



====================get_dvfs_enable_show@0010cd8c====================
// signature: undefined get_dvfs_enable_show(void)

long get_dvfs_enable_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)((long)plVar4 + 0x6c));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}



====================set_dvfs_enable_store@0010ce38====================
// signature: undefined set_dvfs_enable_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_enable_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  code *pcVar4;
  undefined8 *in_x18;
  ulong *puVar5;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  puVar5 = *(ulong **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*puVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      printk(&DAT_00105d0f,"set_dvfs_enable_store",1,local_50);
      *(int *)((long)puVar5 + 0x6c) = (int)local_50;
      *puVar5 = local_50;
      if ((puVar5[0xc] == 0) || (pcVar4 = *(code **)(puVar5[0xc] + 0x30), pcVar4 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_dvfs_enable_store");
      }
      else {
        uVar3 = local_50;
        if (0xb < ((ulong)(pcVar4 + -0x1177fc) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xe1569ae966436cf9,pcVar4);
          uVar3 = (ulong)*(uint *)((long)puVar5 + 0x6c);
        }
        (*pcVar4)(param_1 + -0x30,uVar3);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010ceb4;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010ceb4:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_auto_tune_en_show@0010cfa0====================
// signature: undefined get_auto_tune_en_show(void)

long get_auto_tune_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(plVar4 + 0xe));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}



====================set_auto_tune_en_store@0010d04c====================
// signature: undefined set_auto_tune_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_auto_tune_en_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined8 *in_x18;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      *(undefined4 *)(plVar5 + 0xe) = (undefined4)local_50;
      if ((plVar5[0xc] == 0) || (pcVar3 = *(code **)(plVar5[0xc] + 0x38), pcVar3 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_auto_tune_en_store");
      }
      else {
        uVar4 = (undefined4)local_50;
        if (0x1b < ((ulong)(pcVar3 + -0x1176d4) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0x362db8165d0ff4ec,pcVar3);
          uVar4 = (undefined4)plVar5[0xe];
        }
        (*pcVar3)(param_1 + -0x30,uVar4);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010d114;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010d114:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_work_freq_show@0010d198====================
// signature: undefined get_work_freq_show(void)

long get_work_freq_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x74));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_work_freq_store@0010d224====================
// signature: undefined set_work_freq_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_work_freq_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  long *plVar3;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar3 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      printk(&DAT_00105434,"set_work_freq_store",local_50);
      *(int *)((long)plVar3 + 0x74) = (int)local_50;
      iVar1 = update_devfreq(param_1 + -0x30);
      if (iVar1 != 0) {
        param_4 = iVar1;
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010d2a0;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010d2a0:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_idle_freq_show@0010d324====================
// signature: undefined get_idle_freq_show(void)

long get_idle_freq_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(plVar4 + 0xf));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}



====================set_idle_freq_store@0010d3d0====================
// signature: undefined set_idle_freq_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_idle_freq_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  undefined8 *in_x18;
  code *pcVar5;
  long *plVar6;
  undefined8 unaff_x30;
  ulong local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 != 0) {
      printk(&DAT_00105775,"set_idle_freq_store");
      lVar4 = plVar6[0xc];
      if ((lVar4 == 0) || (*(long *)(lVar4 + 0x50) == 0)) {
        printk(&DAT_001047b2,"set_idle_freq_store");
      }
      else {
        uVar3 = local_50 & 0xffffffff;
        *(undefined4 *)(plVar6 + 0xf) = (undefined4)local_50;
        pcVar5 = *(code **)(lVar4 + 0x50);
        if (0x1b < ((ulong)(pcVar5 + -0x1176d4) >> 2 | (long)pcVar5 << 0x3e)) {
          __cfi_slowpath(0x362db8165d0ff4ec,pcVar5);
          uVar3 = (ulong)*(uint *)(plVar6 + 0xf);
        }
        iVar1 = (*pcVar5)(param_1 + -0x30,uVar3);
      }
      if (iVar1 != 0) {
        param_4 = iVar1;
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010d448;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010d448:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_work_index_show@0010d538====================
// signature: undefined get_work_index_show(void)

long get_work_index_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)((long)plVar4 + 0x7c));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}



====================set_work_index_store@0010d5e4====================
// signature: undefined set_work_index_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_work_index_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  code *pcVar5;
  undefined8 *in_x18;
  long *plVar6;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar3 = param_1 + -0x20;
    mutex_lock(lVar3);
    iVar2 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar2 == 1) {
      printk(&DAT_0010623c,"set_work_index_store",param_4);
      printk(&DAT_00105ef9,"set_work_index_store",local_50);
      lVar4 = plVar6[0xc];
      if ((lVar4 == 0) || (*(long *)(lVar4 + 0xc0) == 0)) {
        printk(&DAT_001047b2,"set_work_index_store");
      }
      else {
        *(undefined4 *)((long)plVar6 + 0x7c) = (undefined4)local_50;
        pcVar5 = *(code **)(lVar4 + 0xc0);
        uVar1 = (undefined4)local_50;
        if (0xe < ((ulong)(pcVar5 + -0x11782c) >> 2 | (long)pcVar5 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar5);
          uVar1 = *(undefined4 *)((long)plVar6 + 0x7c);
        }
        (*pcVar5)(param_1 + -0x30,uVar1);
      }
      mutex_unlock(lVar3);
      lVar3 = (long)param_4;
      goto LAB_0010d660;
    }
    mutex_unlock(lVar3);
  }
  lVar3 = -0x16;
LAB_0010d660:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================get_idle_index_show@0010d760====================
// signature: undefined get_idle_index_show(void)

long get_idle_index_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(plVar4 + 0x10));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}



====================set_idle_index_store@0010d80c====================
// signature: undefined set_idle_index_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_idle_index_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  code *pcVar4;
  undefined8 *in_x18;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar3 = param_1 + -0x20;
    mutex_lock(lVar3);
    iVar2 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar2 == 1) {
      *(undefined4 *)(plVar5 + 0x10) = (undefined4)local_50;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0xd0), pcVar4 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_idle_index_store");
      }
      else {
        uVar1 = (undefined4)local_50;
        if (0xe < ((ulong)(pcVar4 + -0x11782c) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0x2ef299c34abb1f58,pcVar4);
          uVar1 = (undefined4)plVar5[0x10];
        }
        (*pcVar4)(param_1 + -0x30,uVar1);
      }
      mutex_unlock(lVar3);
      lVar3 = (long)param_4;
      goto LAB_0010d8d0;
    }
    mutex_unlock(lVar3);
  }
  lVar3 = -0x16;
LAB_0010d8d0:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================get_fix_dvfs_show@0010d954====================
// signature: undefined get_fix_dvfs_show(void)

long get_fix_dvfs_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x84));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_fix_dvfs_store@0010d9e0====================
// signature: undefined set_fix_dvfs_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_fix_dvfs_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined8 *in_x18;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_50 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%lu\n",&local_50);
    if (iVar1 == 1) {
      *(undefined4 *)((long)plVar5 + 0x84) = (undefined4)local_50;
      if ((plVar5[0xc] == 0) || (pcVar3 = *(code **)(plVar5[0xc] + 0xf8), pcVar3 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_fix_dvfs_store");
      }
      else {
        uVar4 = (undefined4)local_50;
        if (0x1b < ((ulong)(pcVar3 + -0x1176d4) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0x362db8165d0ff4ec,pcVar3);
          uVar4 = *(undefined4 *)((long)plVar5 + 0x84);
        }
        (*pcVar3)(param_1 + -0x30,uVar4);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010daa8;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010daa8:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_dvfs_coffe_show@0010db2c====================
// signature: undefined get_dvfs_coffe_show(void)

long get_dvfs_coffe_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar2 = s_IP_dvfs_coffe_show_0010624c._8_8_;
  uVar1 = s_IP_dvfs_coffe_show_0010624c._0_8_;
  param_3[0x10] = 'o';
  param_3[0x11] = 'w';
  param_3[0x12] = '\n';
  param_3[0x13] = '\0';
  *(undefined8 *)param_3 = uVar1;
  *(undefined8 *)(param_3 + 8) = uVar2;
  iVar3 = sprintf(param_3 + 0x13,"%d\n",(ulong)*(uint *)(lVar5 + 0x11c));
  iVar4 = sprintf(param_3 + (iVar3 + 0x13),"%d\n",(ulong)*(uint *)(lVar5 + 0x110));
  iVar4 = iVar4 + iVar3 + 0x13;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x114));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x10c));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x108));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x124));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x124));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x120));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x118));
  mutex_unlock(param_1 + -0x20);
  return (long)(iVar3 + iVar4);
}



====================set_dvfs_coffe_store@0010dc58====================
// signature: undefined set_dvfs_coffe_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_coffe_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  code *pcVar3;
  long *plVar4;
  undefined8 unaff_x30;
  long local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
  }
  else {
    lVar2 = param_1 + -0x20;
    mutex_lock(lVar2);
    iVar1 = sscanf(param_3,"%d,%d,%d,%d,%d,%d,%d,%d\n",&local_68,(ulong)&local_68 | 4,&uStack_60,
                   (long)&uStack_60 + 4,&local_58,(long)&local_58 + 4,&uStack_50,
                   (long)&uStack_50 + 4);
    plVar4[0x23] = local_58;
    plVar4[0x24] = uStack_50;
    plVar4[0x21] = local_68;
    plVar4[0x22] = uStack_60;
    if (iVar1 == 8) {
      if ((plVar4[0xc] == 0) || (pcVar3 = *(code **)(plVar4[0xc] + 0x78), pcVar3 == (code *)0x0)) {
        printk(&DAT_001047b2,"set_dvfs_coffe_store");
      }
      else {
        __cfi_slowpath(0xa337d2e020122362,pcVar3);
        (*pcVar3)(param_1 + -0x30,&local_68);
      }
      mutex_unlock(lVar2);
      lVar2 = (long)param_4;
      goto LAB_0010dd50;
    }
    mutex_unlock(lVar2);
  }
  lVar2 = -0x16;
LAB_0010dd50:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_ip_status_show@0010ddac====================
// signature: undefined get_ip_status_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_ip_status_show(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  long lVar4;
  undefined8 *in_x18;
  long lVar5;
  long *plVar6;
  undefined8 unaff_x30;
  undefined8 local_d0;
  undefined8 local_c8;
  ulong local_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  local_c8 = 0;
  local_c0 = 0;
  local_d0 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
    lVar5 = -0x16;
    goto LAB_0010df18;
  }
  lVar5 = param_1 + -0x30;
  mutex_lock(param_1 + -0x20);
  if ((plVar6[0xc] == 0) || (pcVar3 = *(code **)(plVar6[0xc] + 0x80), pcVar3 == (code *)0x0)) {
    printk(&DAT_00105b1c,"get_ip_status_show");
  }
  else {
    if (7 < ((ulong)(pcVar3 + -0x11779c) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x67ed40f9fdf2c861,pcVar3);
    }
    (*pcVar3)(lVar5,&local_c8);
  }
  if ((plVar6[0xc] == 0) || (pcVar3 = *(code **)(plVar6[0xc] + 0x130), pcVar3 == (code *)0x0)) {
    printk(&DAT_001047c6,"get_ip_status_show");
    lVar4 = plVar6[0xc];
    if (lVar4 != 0) goto LAB_0010de8c;
LAB_0010df80:
    printk(&DAT_001047c6,"get_ip_status_show");
  }
  else {
    if (0xd < ((ulong)(pcVar3 + -0x117744) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x2e12bb038fa0cb35,pcVar3);
    }
    (*pcVar3)(lVar5,(long)&local_d0 + 4);
    lVar4 = plVar6[0xc];
    if (lVar4 == 0) goto LAB_0010df80;
LAB_0010de8c:
    pcVar3 = *(code **)(lVar4 + 0x138);
    if (pcVar3 == (code *)0x0) goto LAB_0010df80;
    if (0xd < ((ulong)(pcVar3 + -0x117744) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x2e12bb038fa0cb35,pcVar3);
    }
    (*pcVar3)(lVar5,&local_d0);
  }
  iVar1 = sprintf(param_3,"module_dvfs_read_clk=%d\n",local_c0 >> 0x20);
  iVar2 = sprintf(param_3 + iVar1,"module_dvfs_read_volt=%d\n",local_c0 & 0xffffffff);
  lVar5 = (long)iVar1 + (long)iVar2;
  iVar1 = sprintf(param_3 + lVar5,"module_dvfs_read_top_volt=%d\n",local_d0 >> 0x20);
  lVar5 = lVar5 + iVar1;
  iVar1 = sprintf(param_3 + lVar5,"module_dvfs_read_mm_volt=%d\n",local_d0 & 0xffffffff);
  lVar5 = lVar5 + iVar1;
  mutex_unlock(param_1 + -0x20);
LAB_0010df18:
  if (___stack_chk_guard == local_48) {
    return lVar5;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================get_ip_status_store@0010e01c====================
// signature: undefined get_ip_status_store(void)

undefined8 get_ip_status_store(void)

{
  return 0;
}



====================get_dvfs_table_info_show@0010e024====================
// signature: undefined get_dvfs_table_info_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_dvfs_table_info_show(long param_1,undefined8 param_2,long param_3)

{
  int iVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  code *pcVar4;
  long lVar5;
  undefined8 unaff_x30;
  uint local_230 [2];
  undefined8 local_228;
  uint local_21c [7];
  undefined8 local_200 [50];
  long local_70;
  
  *in_x18 = unaff_x30;
  local_70 = ___stack_chk_guard;
  memset(local_230,0,0x1c0);
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  lVar3 = *(long *)(lVar3 + 0x60);
  if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x60), pcVar4 == (code *)0x0)) {
    printk(&DAT_001059ba,"get_dvfs_table_info_show");
  }
  else {
    if (0xd < ((ulong)(pcVar4 + -0x1175c0) >> 2 | (long)pcVar4 << 0x3e)) {
      __cfi_slowpath(0xe5c7695a0ac14b08,pcVar4);
    }
    (*pcVar4)(param_1 + -0x30,local_230);
  }
  lVar5 = 0;
  lVar3 = 0;
  do {
    iVar1 = sprintf((char *)(param_3 + lVar3),"volt=%s\n",*(undefined8 *)((long)local_200 + lVar5));
    iVar2 = sprintf((char *)(param_3 + lVar3 + iVar1),"clk=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5));
    lVar3 = lVar3 + iVar1 + (long)iVar2;
    iVar1 = sprintf((char *)(param_3 + lVar3),"map_index=%d\n",
                    (ulong)*(uint *)((long)local_230 + lVar5));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"reg_add=%lu\n",
                    *(undefined8 *)((long)&local_228 + lVar5));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"fdiv_denom=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 8));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"fdiv_num=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0xc));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"axi_index=%d\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0x10));
    lVar3 = lVar3 + iVar1;
    iVar1 = sprintf((char *)(param_3 + lVar3),"mtx_index=%d\n\n",
                    (ulong)*(uint *)((long)local_21c + lVar5 + 0x14));
    lVar5 = lVar5 + 0x38;
    lVar3 = lVar3 + iVar1;
  } while (lVar5 != 0x1c0);
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_70) {
    return lVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================set_dvfs_table_info_store@0010e22c====================
// signature: undefined set_dvfs_table_info_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dvfs_table_info_store(long param_1,undefined8 param_2,char *param_3,undefined8 *param_4)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  code *extraout_x8;
  undefined8 *in_x18;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined8 unaff_x30;
  ulong local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  lVar5 = param_1 + -0x20;
  local_68 = ___stack_chk_guard;
  local_78 = 0;
  uStack_70 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_a0 = 0;
  lVar6 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  puVar4 = &local_78;
  mutex_lock(lVar5);
  iVar1 = sscanf(param_3," %d,%lu,%d,%d,%d,%d,%d,%d,\n",&local_a0,&local_98,&uStack_90,&local_88,
                 (long)&local_88 + 4,&uStack_80,(long)&uStack_80 + 4,puVar4);
  if (iVar1 != 8) {
    mutex_unlock(lVar5);
    lVar5 = -0x16;
    goto LAB_0010e368;
  }
  if ((uint)local_a0 < 9) {
    lVar2 = lVar6 + (local_a0 & 0xffffffff) * 0x38;
    *(undefined8 *)(lVar2 + 0x130) = local_98;
    *(undefined8 *)(lVar2 + 0x138) = uStack_90;
    *(ulong *)(lVar2 + 0x128) = local_a0;
    *(undefined8 *)(lVar2 + 0x150) = local_78;
    *(undefined8 *)(lVar2 + 0x158) = uStack_70;
    *(undefined8 *)(lVar2 + 0x140) = local_88;
    *(undefined8 *)(lVar2 + 0x148) = uStack_80;
    lVar6 = *(long *)(lVar6 + 0x60);
    iVar1 = (int)param_4;
    if ((lVar6 != 0) && (pcVar3 = *(code **)(lVar6 + 0x68), pcVar3 != (code *)0x0)) {
      param_1 = param_1 + -0x30;
      puVar4 = param_4;
      if (0xd < ((ulong)(pcVar3 + -0x1175c0) >> 2 | (long)pcVar3 << 0x3e)) goto LAB_0010e3c8;
      goto LAB_0010e340;
    }
    printk(&DAT_001059ba,"set_dvfs_table_info_store");
  }
  else {
    __ubsan_handle_cfi_check_fail_abort(0x12);
    pcVar3 = extraout_x8;
LAB_0010e3c8:
    iVar1 = (int)puVar4;
    __cfi_slowpath(0xe5c7695a0ac14b08,pcVar3);
LAB_0010e340:
    (*pcVar3)(param_1,&local_a0);
  }
  mutex_unlock(lVar5);
  lVar5 = (long)iVar1;
LAB_0010e368:
  if (___stack_chk_guard == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar5);
}



====================get_gfree_wait_delay_show@0010e3ec====================
// signature: undefined get_gfree_wait_delay_show(void)

long get_gfree_wait_delay_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x108));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_gfree_wait_delay_store@0010e478====================
// signature: undefined set_gfree_wait_delay_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_gfree_wait_delay_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar2 = sscanf(param_3," %lu\n",&local_40);
    printk(&DAT_00105c69,"set_gfree_wait_delay_store",iVar2,local_40,param_4);
    if (iVar2 == 1) {
      *(undefined4 *)(plVar5 + 0x21) = (undefined4)local_40;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0x98), pcVar4 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_gfree_wait_delay_store");
      }
      else {
        uVar1 = (undefined4)local_40;
        if (0x21 < ((ulong)(pcVar4 + -0x1175f8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar4);
          uVar1 = (undefined4)plVar5[0x21];
        }
        (*pcVar4)(uVar1);
      }
      mutex_unlock(param_1);
      lVar3 = (long)param_4;
      goto LAB_0010e568;
    }
    mutex_unlock(param_1);
  }
  lVar3 = -0x16;
LAB_0010e568:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================set_freq_upd_hdsk_en_store@0010e5d0====================
// signature: undefined set_freq_upd_hdsk_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_freq_upd_hdsk_en_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar2 = sscanf(param_3,"%lu\n",&local_40);
    if (iVar2 == 1) {
      *(undefined4 *)((long)plVar5 + 0x10c) = (undefined4)local_40;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0xb0), pcVar4 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_freq_upd_hdsk_en_store");
      }
      else {
        uVar1 = (undefined4)local_40;
        if (0x21 < ((ulong)(pcVar4 + -0x1175f8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar4);
          uVar1 = *(undefined4 *)((long)plVar5 + 0x10c);
        }
        (*pcVar4)(uVar1);
      }
      mutex_unlock(param_1);
      lVar3 = (long)param_4;
      goto LAB_0010e68c;
    }
    mutex_unlock(param_1);
  }
  lVar3 = -0x16;
LAB_0010e68c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================get_freq_upd_hdsk_en_show@0010e704====================
// signature: undefined get_freq_upd_hdsk_en_show(void)

long get_freq_upd_hdsk_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x10c));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_freq_upd_delay_en_store@0010e790====================
// signature: undefined set_freq_upd_delay_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_freq_upd_delay_en_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar2 = sscanf(param_3," %lu\n",&local_40);
    if (iVar2 == 1) {
      *(undefined4 *)(plVar5 + 0x22) = (undefined4)local_40;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0xa8), pcVar4 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_freq_upd_delay_en_store");
      }
      else {
        uVar1 = (undefined4)local_40;
        if (0x21 < ((ulong)(pcVar4 + -0x1175f8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar4);
          uVar1 = (undefined4)plVar5[0x22];
        }
        (*pcVar4)(uVar1);
      }
      mutex_unlock(param_1);
      lVar3 = (long)param_4;
      goto LAB_0010e84c;
    }
    mutex_unlock(param_1);
  }
  lVar3 = -0x16;
LAB_0010e84c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================get_freq_upd_delay_en_show@0010e8c4====================
// signature: undefined get_freq_upd_delay_en_show(void)

long get_freq_upd_delay_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x110));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_freq_upd_en_byp_store@0010e950====================
// signature: undefined set_freq_upd_en_byp_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_freq_upd_en_byp_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar5 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar2 = sscanf(param_3," %lu\n",&local_40);
    if (iVar2 == 1) {
      *(undefined4 *)((long)plVar5 + 0x114) = (undefined4)local_40;
      if ((plVar5[0xc] == 0) || (pcVar4 = *(code **)(plVar5[0xc] + 0xa0), pcVar4 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_freq_upd_en_byp_store");
      }
      else {
        uVar1 = (undefined4)local_40;
        if (0x21 < ((ulong)(pcVar4 + -0x1175f8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar4);
          uVar1 = *(undefined4 *)((long)plVar5 + 0x114);
        }
        (*pcVar4)(uVar1);
      }
      mutex_unlock(param_1);
      lVar3 = (long)param_4;
      goto LAB_0010ea0c;
    }
    mutex_unlock(param_1);
  }
  lVar3 = -0x16;
LAB_0010ea0c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}



====================get_freq_upd_en_byp_show@0010ea84====================
// signature: undefined get_freq_upd_en_byp_show(void)

long get_freq_upd_en_byp_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x114));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================set_sw_trig_en_store@0010eb10====================
// signature: undefined set_sw_trig_en_store(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_sw_trig_en_store(long param_1,undefined8 param_2,char *param_3,int param_4)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  code *pcVar3;
  long *plVar4;
  undefined8 unaff_x30;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_40 = 0;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
  }
  else {
    param_1 = param_1 + -0x20;
    mutex_lock(param_1);
    iVar1 = sscanf(param_3,"%lu\n",&local_40);
    if (iVar1 == 1) {
      *(undefined4 *)((long)plVar4 + 0x114) = (undefined4)local_40;
      if ((plVar4[0xc] == 0) || (pcVar3 = *(code **)(plVar4[0xc] + 0xb8), pcVar3 == (code *)0x0)) {
        printk(&DAT_001059ba,"set_sw_trig_en_store");
      }
      else {
        if (0x21 < ((ulong)(pcVar3 + -0x1175f8) >> 2 | (long)pcVar3 << 0x3e)) {
          __cfi_slowpath(0xa441d0b9ace37e6c,pcVar3);
        }
        (*pcVar3)((undefined4)local_40);
      }
      mutex_unlock(param_1);
      lVar2 = (long)param_4;
      goto LAB_0010ebcc;
    }
    mutex_unlock(param_1);
  }
  lVar2 = -0x16;
LAB_0010ebcc:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}



====================get_sw_trig_en_show@0010ec44====================
// signature: undefined get_sw_trig_en_show(void)

long get_sw_trig_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (lVar3 == 0) {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  else {
    iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x118));
    lVar3 = (long)iVar2;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}



====================ip_dvfs_init@0010ecd0====================
// signature: undefined ip_dvfs_init(void)

undefined8 ip_dvfs_init(long param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar7 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_001059db);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00103e88 + lVar6);
      puVar2 = (uint *)((long)&DAT_00103ea0 + lVar6);
      plVar4 = (long *)((long)&DAT_00103e80 + lVar6);
      puVar3 = (uint *)((long)&DAT_00103e90 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00104014;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00103e8c;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffbf;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffdfff;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x1000;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x134;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffbf;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1f0) = *(uint *)(g_mmreg_map + 0x1f0) & 0xfffffff8 | 2;
    *(uint *)(g_mmreg_map + 500) = *(uint *)(g_mmreg_map + 500) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x40;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_00103efc;
    printk(&DAT_001049a5,1,0,2,0,1);
    uVar5 = 0;
  }
  return uVar5;
}



====================ip_hw_dvfs_en@0010f028====================
// signature: undefined ip_hw_dvfs_en(void)

undefined8 ip_hw_dvfs_en(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================ip_auto_tune_en@0010f0c8====================
// signature: undefined ip_auto_tune_en(void)

undefined8 ip_auto_tune_en(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  return 0;
}



====================set_work_freq@0010f108====================
// signature: undefined set_work_freq(void)

undefined8 set_work_freq(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103e8c < param_2) {
    if (DAT_00103ec4 < param_2) {
      if (DAT_00103efc < param_2) {
        if (DAT_00103f34 < param_2) {
          if (DAT_00103f6c < param_2) {
            if (DAT_00103fa4 < param_2) {
              uVar1 = 6;
              if (DAT_00103fdc < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1f0) = *(uint *)(g_mmreg_map + 0x1f0) & 0xfffffff8 | uVar1;
  printk(&DAT_00104b22,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq@0010f248====================
// signature: undefined set_idle_freq(void)

undefined8 set_idle_freq(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103e8c < param_2) {
    if (DAT_00103ec4 < param_2) {
      if (DAT_00103efc < param_2) {
        if (DAT_00103f34 < param_2) {
          if (DAT_00103f6c < param_2) {
            if (DAT_00103fa4 < param_2) {
              uVar1 = 6;
              if (DAT_00103fdc < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 500) = *(uint *)(g_mmreg_map + 500) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}



====================get_ip_dvfs_table@0010f388====================
// signature: undefined get_ip_dvfs_table(void)

undefined8 get_ip_dvfs_table(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3 * 0x38);
    lVar1 = lVar3 + 1;
    *puVar2 = (&fd_dvfs_config_table)[lVar3 * 0xe];
    puVar2[5] = (&DAT_00103e8c)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00103e90)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00103ea8)[lVar3 * 7];
    puVar2[4] = (&DAT_00103e88)[lVar3 * 0xe];
    puVar2[7] = (&DAT_00103e94)[lVar3 * 0xe];
    puVar2[8] = (&DAT_00103e98)[lVar3 * 0xe];
    puVar2[9] = (&DAT_00103e9c)[lVar3 * 0xe];
    puVar2[10] = (&DAT_00103ea0)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_00103e80)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}



====================set_ip_dvfs_table@0010f454====================
// signature: undefined set_ip_dvfs_table(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103e80 = param_2[1];
  _DAT_00103e88 = param_2[2];
  _fd_dvfs_config_table = *param_2;
  _DAT_00103ea0 = param_2[5];
  PTR_DAT_00103ea8 = (undefined *)param_2[6];
  _DAT_00103e90 = param_2[3];
  _DAT_00103e98 = param_2[4];
  return 0;
}



====================get_ip_status@0010f4bc====================
// signature: undefined get_ip_status(void)

undefined8 get_ip_status(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 0x17 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103e90) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103ec8) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103f00) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103f38) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103f70) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103fa8) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103fe0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00104018) goto LAB_0010f5c0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_00103e8c)[lVar3 * 0xe];
LAB_0010f5c0:
  *(uint *)(param_2 + 8) = uVar1 >> 0x15 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================power_on_nb@0010f5ec====================
// signature: undefined power_on_nb(void)

void power_on_nb(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init();
  return;
}



====================power_off_nb@0010f62c====================
// signature: undefined power_off_nb(void)

void power_off_nb(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_00104fb6);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay@0010f66c====================
// signature: undefined set_ip_gfree_wait_delay(void)

void set_ip_gfree_wait_delay(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = param_1 & 0x3ff;
  return;
}



====================set_ip_freq_upd_en_byp@0010f6bc====================
// signature: undefined set_ip_freq_upd_en_byp(void)

void set_ip_freq_upd_en_byp(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en@0010f754====================
// signature: undefined set_ip_freq_upd_delay_en(void)

void set_ip_freq_upd_delay_en(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffdfff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x2000;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en@0010f7ec====================
// signature: undefined set_ip_freq_upd_hdsk_en(void)

void set_ip_freq_upd_hdsk_en(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffefff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x1000;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en@0010f884====================
// signature: undefined set_ip_dvfs_swtrig_en(void)

void set_ip_dvfs_swtrig_en(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index@0010f91c====================
// signature: undefined set_ip_dvfs_work_index(void)

void set_ip_dvfs_work_index(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1f0) = *(uint *)(g_mmreg_map + 0x1f0) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index@0010f97c====================
// signature: undefined get_ip_dvfs_work_index(void)

void get_ip_dvfs_work_index(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1f0);
  return;
}



====================set_ip_dvfs_idle_index@0010f9cc====================
// signature: undefined set_ip_dvfs_idle_index(void)

void set_ip_dvfs_idle_index(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 500) = *(uint *)(g_mmreg_map + 500) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table@0010fa44====================
// signature: undefined get_ip_index_from_table(void)

void get_ip_index_from_table(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_00103e8c < param_2) {
    if (DAT_00103ec4 < param_2) {
      if (DAT_00103efc < param_2) {
        if (DAT_00103f34 < param_2) {
          if (DAT_00103f6c < param_2) {
            if (DAT_00103fa4 < param_2) {
              uVar1 = 6;
              if (DAT_00103fdc < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq@0010fb28====================
// signature: undefined updata_target_freq(void)

undefined8 updata_target_freq(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value@0010fb90====================
// signature: undefined set_fix_dvfs_value(void)

undefined8 set_fix_dvfs_value(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 0;
}



====================top_current_volt@0010fc0c====================
// signature: undefined top_current_volt(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt@0010fcdc====================
// signature: undefined mm_current_volt(void)

undefined8 mm_current_volt(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================ip_dvfs_init.703@0010fd50====================
// signature: undefined ip_dvfs_init.703(void)

undefined8 ip_dvfs_init_703(long param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar7 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_001052a7);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00104048 + lVar6);
      puVar2 = (uint *)((long)&DAT_00104060 + lVar6);
      plVar4 = (long *)((long)&DAT_00104040 + lVar6);
      puVar3 = (uint *)((long)&DAT_00104050 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 6 | (*puVar2 & 7) << 3 | *puVar3 & 7;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_001041d4;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00104048._4_4_;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffb;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffff7ff;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x400;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x13400000;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffd;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1c0) = *(uint *)(g_mmreg_map + 0x1c0) & 0xfffffff8 | 4;
    *(uint *)(g_mmreg_map + 0x1c4) = *(uint *)(g_mmreg_map + 0x1c4) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 2;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_0010412c;
    printk(&DAT_0010545f,1,0,4,0,1);
    uVar5 = 0;
  }
  return uVar5;
}



====================ip_hw_dvfs_en.704@00110080====================
// signature: undefined ip_hw_dvfs_en.704(void)

undefined8 ip_hw_dvfs_en_704(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xfffffffd;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 2;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================ip_auto_tune_en.705@00110120====================
// signature: undefined ip_auto_tune_en.705(void)

undefined8 ip_auto_tune_en_705(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  return 0;
}



====================set_work_freq.706@00110160====================
// signature: undefined set_work_freq.706(void)

undefined8 set_work_freq_706(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00104048._4_4_ < param_2) {
    if (DAT_00104084 < param_2) {
      if (DAT_001040bc < param_2) {
        if (DAT_001040f4 < param_2) {
          if (DAT_0010412c < param_2) {
            if (DAT_00104164 < param_2) {
              uVar1 = 6;
              if (DAT_0010419c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1c0) = *(uint *)(g_mmreg_map + 0x1c0) & 0xfffffff8 | uVar1;
  printk(&DAT_00105d2a,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq.707@001102a4====================
// signature: undefined set_idle_freq.707(void)

undefined8 set_idle_freq_707(void)

{
  return 0;
}



====================get_ip_dvfs_table.708@001102ac====================
// signature: undefined get_ip_dvfs_table.708(void)

undefined8 get_ip_dvfs_table_708(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    lVar4 = lVar3 * 0x38;
    puVar2 = (undefined4 *)(param_2 + lVar4);
    lVar1 = lVar3 + 1;
    *puVar2 = *(undefined4 *)(&isp_dvfs_config_table + lVar3 * 7);
    puVar2[5] = *(undefined4 *)((long)&DAT_00104048 + lVar4 + 4);
    puVar2[6] = (&DAT_00104050)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00104068)[lVar3 * 7];
    puVar2[4] = *(undefined4 *)(&DAT_00104048 + lVar3 * 7);
    puVar2[7] = *(undefined4 *)(&DAT_00104054 + lVar4);
    puVar2[8] = *(undefined4 *)(&DAT_00104058 + lVar3 * 7);
    puVar2[9] = *(undefined4 *)((long)&DAT_00104058 + lVar4 + 4);
    puVar2[10] = *(undefined4 *)(&DAT_00104060 + lVar3 * 7);
    *(undefined8 *)(puVar2 + 2) = (&DAT_00104040)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}



====================set_ip_dvfs_table.709@00110378====================
// signature: undefined set_ip_dvfs_table.709(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_709(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00104040 = param_2[1];
  DAT_00104048 = param_2[2];
  isp_dvfs_config_table = *param_2;
  DAT_00104060 = param_2[5];
  PTR_DAT_00104068 = (undefined *)param_2[6];
  _DAT_00104050 = param_2[3];
  DAT_00104058 = param_2[4];
  return 0;
}



====================get_ip_status.710@001103e0====================
// signature: undefined get_ip_status.710(void)

undefined8 get_ip_status_710(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 3 & 7;
  *(uint *)(param_2 + 8) = uVar1 >> 6 & 7;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00104050) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00104088) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_001040c0) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001040f8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00104130) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00104168) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_001041a0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_001041d8) goto LAB_001104e8;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)((long)&DAT_00104048 + lVar3 * 0x38 + 4);
LAB_001104e8:
  printk(&DAT_001059f0,"get_ip_status",uVar1,uVar1);
  return 0;
}



====================power_on_nb.711@00110510====================
// signature: undefined power_on_nb.711(void)

void power_on_nb_711(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_703();
  return;
}



====================power_off_nb.712@00110550====================
// signature: undefined power_off_nb.712(void)

void power_off_nb_712(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_00105448);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.713@00110590====================
// signature: undefined set_ip_gfree_wait_delay.713(void)

void set_ip_gfree_wait_delay_713(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = (param_1 & 0x3ff) << 0x14;
  return;
}



====================set_ip_freq_upd_en_byp.714@001105e0====================
// signature: undefined set_ip_freq_upd_en_byp.714(void)

void set_ip_freq_upd_en_byp_714(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.715@00110678====================
// signature: undefined set_ip_freq_upd_delay_en.715(void)

void set_ip_freq_upd_delay_en_715(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffff7ff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x800;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.716@00110710====================
// signature: undefined set_ip_freq_upd_hdsk_en.716(void)

void set_ip_freq_upd_hdsk_en_716(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffbff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x400;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en.717@001107a8====================
// signature: undefined set_ip_dvfs_swtrig_en.717(void)

void set_ip_dvfs_swtrig_en_717(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffd;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 2;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.718@00110840====================
// signature: undefined set_ip_dvfs_work_index.718(void)

void set_ip_dvfs_work_index_718(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1c0) = *(uint *)(g_mmreg_map + 0x1c0) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.719@001108a0====================
// signature: undefined get_ip_dvfs_work_index.719(void)

void get_ip_dvfs_work_index_719(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1c0);
  return;
}



====================set_ip_dvfs_idle_index.720@001108f0====================
// signature: undefined set_ip_dvfs_idle_index.720(void)

void set_ip_dvfs_idle_index_720(undefined8 param_1,uint param_2)

{
  *(uint *)(g_mmreg_map + 0x1c4) = *(uint *)(g_mmreg_map + 0x1c4) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table.721@00110918====================
// signature: undefined get_ip_index_from_table.721(void)

void get_ip_index_from_table_721(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_00104048._4_4_ < param_2) {
    if (DAT_00104084 < param_2) {
      if (DAT_001040bc < param_2) {
        if (DAT_001040f4 < param_2) {
          if (DAT_0010412c < param_2) {
            if (DAT_00104164 < param_2) {
              uVar1 = 6;
              if (DAT_0010419c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.722@00110a00====================
// signature: undefined updata_target_freq.722(void)

undefined8
updata_target_freq_722(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_706(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value.723@00110a68====================
// signature: undefined set_fix_dvfs_value.723(void)

undefined8 set_fix_dvfs_value_723(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 0;
}



====================top_current_volt.724@00110ae4====================
// signature: undefined top_current_volt.724(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_724(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.725@00110bb4====================
// signature: undefined mm_current_volt.725(void)

undefined8 mm_current_volt_725(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================ip_dvfs_init.775@00110c28====================
// signature: undefined ip_dvfs_init.775(void)

undefined8 ip_dvfs_init_775(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar2 == (undefined8 *)0x0) {
    printk(&DAT_00105b65);
    uVar1 = 0xffffffea;
  }
  else {
    *(uint *)(DAT_00103b00 + g_mmreg_map) = DAT_00103b10 & 3 | ((uint)DAT_00103b08 & 7) << 2;
    *(uint *)(DAT_00103b38 + g_mmreg_map) = DAT_00103b48 & 3 | (DAT_00103b40 & 7) << 2;
    *(uint *)(DAT_00103b70 + g_mmreg_map) = DAT_00103b80 & 3 | (DAT_00103b78 & 7) << 2;
    *(uint *)(DAT_00103ba8 + g_mmreg_map) = DAT_00103bb8 & 3 | (DAT_00103bb0 & 7) << 2;
    *(uint *)(DAT_00103be0 + g_mmreg_map) = DAT_00103bf0 & 3 | (DAT_00103be8 & 7) << 2;
    *(uint *)(DAT_00103c18 + g_mmreg_map) = DAT_00103c28 & 3 | (DAT_00103c20 & 7) << 2;
    *(uint *)(DAT_00103c50 + g_mmreg_map) = DAT_00103c60 & 3 | (DAT_00103c58 & 7) << 2;
    *(uint *)(DAT_00103c88 + g_mmreg_map) = DAT_00103c98 & 3 | (DAT_00103c90 & 7) << 2;
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103c94;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00103b08._4_4_;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffe;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 4;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x4d000;
    *(uint *)(g_mmreg_map + 0x1e8) = *(uint *)(g_mmreg_map + 0x1e8) & 0xfffffff8 | 2;
    *(uint *)(g_mmreg_map + 0x1ec) = *(uint *)(g_mmreg_map + 0x1ec) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x80;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar2 = 1;
    puVar2[4] = (ulong)DAT_00103b7c;
    printk(&DAT_00106531,1,0,2,0,1);
    uVar1 = 0;
  }
  return uVar1;
}



====================ip_hw_dvfs_en.776@00110fc0====================
// signature: undefined ip_hw_dvfs_en.776(void)

undefined8 ip_hw_dvfs_en_776(void)

{
  return 0;
}



====================ip_auto_tune_en.777@00110fc8====================
// signature: undefined ip_auto_tune_en.777(void)

undefined8 ip_auto_tune_en_777(undefined8 param_1,long param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffff7f;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x80;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================set_work_freq.778@00111040====================
// signature: undefined set_work_freq.778(void)

undefined8 set_work_freq_778(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103b08._4_4_ < param_2) {
    if (DAT_00103b44 < param_2) {
      if (DAT_00103b7c < param_2) {
        if (DAT_00103bb4 < param_2) {
          if (DAT_00103bec < param_2) {
            if (DAT_00103c24 < param_2) {
              uVar1 = 6;
              if (DAT_00103c5c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1e8) = *(uint *)(g_mmreg_map + 0x1e8) & 0xfffffff8 | uVar1;
  printk(&DAT_001055b1,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq.779@00111184====================
// signature: undefined set_idle_freq.779(void)

undefined8 set_idle_freq_779(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103b08._4_4_ < param_2) {
    if (DAT_00103b44 < param_2) {
      if (DAT_00103b7c < param_2) {
        if (DAT_00103bb4 < param_2) {
          if (DAT_00103bec < param_2) {
            if (DAT_00103c24 < param_2) {
              uVar1 = 6;
              if (DAT_00103c5c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1ec) = *(uint *)(g_mmreg_map + 0x1ec) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}



====================get_ip_dvfs_table.780@001112c8====================
// signature: undefined get_ip_dvfs_table.780(void)

undefined8 get_ip_dvfs_table_780(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010480c,"get_ip_dvfs_table");
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3);
    lVar1 = lVar3 + 0x38;
    *puVar2 = *(undefined4 *)((long)&dcamaxi_dvfs_config_table + lVar3);
    puVar2[5] = *(undefined4 *)((long)&DAT_00103b08 + lVar3 + 4);
    puVar2[6] = *(undefined4 *)((long)&DAT_00103b10 + lVar3);
    *(undefined8 *)(puVar2 + 0xc) = *(undefined8 *)((long)&PTR_DAT_00103b28 + lVar3);
    puVar2[4] = *(undefined4 *)((long)&DAT_00103b08 + lVar3);
    puVar2[7] = *(undefined4 *)(&DAT_00103b14 + lVar3);
    puVar2[8] = *(undefined4 *)((long)&DAT_00103b18 + lVar3);
    puVar2[9] = *(undefined4 *)((long)&DAT_00103b18 + lVar3 + 4);
    puVar2[10] = *(undefined4 *)((long)&DAT_00103b20 + lVar3);
    *(undefined8 *)(puVar2 + 2) = *(undefined8 *)((long)&DAT_00103b00 + lVar3);
    lVar3 = lVar1;
  } while (lVar1 != 0x1c0);
  return 0;
}



====================set_ip_dvfs_table.781@00111374====================
// signature: undefined set_ip_dvfs_table.781(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_781(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103b00 = param_2[1];
  DAT_00103b08 = param_2[2];
  dcamaxi_dvfs_config_table = *param_2;
  DAT_00103b20 = param_2[5];
  PTR_DAT_00103b28 = (undefined *)param_2[6];
  _DAT_00103b10 = param_2[3];
  DAT_00103b18 = param_2[4];
  return 0;
}



====================get_ip_status.782@001113dc====================
// signature: undefined get_ip_status.782(void)

undefined8 get_ip_status_782(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 10 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103b10) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103b48) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103b80) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103bb8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103bf0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103c28) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103c60) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103c98) goto LAB_001114e0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)((long)&DAT_00103b08 + lVar3 * 0x38 + 4);
LAB_001114e0:
  *(uint *)(param_2 + 8) = uVar1 >> 0xf & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================power_on_nb.783@0011150c====================
// signature: undefined power_on_nb.783(void)

void power_on_nb_783(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_775();
  return;
}



====================power_off_nb.784@0011154c====================
// signature: undefined power_off_nb.784(void)

void power_off_nb_784(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_001047f1);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.785@0011158c====================
// signature: undefined set_ip_gfree_wait_delay.785(void)

void set_ip_gfree_wait_delay_785(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = (param_1 & 0x3ff) << 10;
  return;
}



====================set_ip_freq_upd_en_byp.786@001115dc====================
// signature: undefined set_ip_freq_upd_en_byp.786(void)

void set_ip_freq_upd_en_byp_786(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.787@00111674====================
// signature: undefined set_ip_freq_upd_delay_en.787(void)

void set_ip_freq_upd_delay_en_787(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffff7;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 8;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.788@0011170c====================
// signature: undefined set_ip_freq_upd_hdsk_en.788(void)

void set_ip_freq_upd_hdsk_en_788(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.789@001117a4====================
// signature: undefined set_ip_dvfs_work_index.789(void)

void set_ip_dvfs_work_index_789(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1e8) = *(uint *)(g_mmreg_map + 0x1e8) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.790@00111804====================
// signature: undefined get_ip_dvfs_work_index.790(void)

void get_ip_dvfs_work_index_790(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1e8);
  return;
}



====================set_ip_dvfs_idle_index.791@00111854====================
// signature: undefined set_ip_dvfs_idle_index.791(void)

void set_ip_dvfs_idle_index_791(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1ec) = *(uint *)(g_mmreg_map + 0x1ec) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table.792@001118cc====================
// signature: undefined get_ip_index_from_table.792(void)

void get_ip_index_from_table_792(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_00103b08._4_4_ < param_2) {
    if (DAT_00103b44 < param_2) {
      if (DAT_00103b7c < param_2) {
        if (DAT_00103bb4 < param_2) {
          if (DAT_00103bec < param_2) {
            if (DAT_00103c24 < param_2) {
              uVar1 = 6;
              if (DAT_00103c5c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.793@001119b4====================
// signature: undefined updata_target_freq.793(void)

undefined8
updata_target_freq_793(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_778(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value.794@00111a1c====================
// signature: undefined set_fix_dvfs_value.794(void)

undefined8 set_fix_dvfs_value_794(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,param_2 >> 0x1f | (param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  return 0;
}



====================top_current_volt.795@00111a98====================
// signature: undefined top_current_volt.795(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_795(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.796@00111b68====================
// signature: undefined mm_current_volt.796(void)

undefined8 mm_current_volt_796(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================ip_dvfs_init.831@00111bdc====================
// signature: undefined ip_dvfs_init.831(void)

undefined8 ip_dvfs_init_831(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar2 == (undefined8 *)0x0) {
    printk(&DAT_00104a03);
    uVar1 = 0xffffffea;
  }
  else {
    *(uint *)(DAT_001043c0 + g_mmreg_map) = DAT_001043d0 & 7 | (DAT_001043c8 & 7) << 3;
    *(uint *)(DAT_001043f8 + g_mmreg_map) = DAT_00104408 & 7 | (DAT_00104400 & 7) << 3;
    *(uint *)(DAT_00104430 + g_mmreg_map) = DAT_00104440 & 7 | (DAT_00104438 & 7) << 3;
    *(uint *)(DAT_00104468 + g_mmreg_map) = DAT_00104478 & 7 | (DAT_00104470 & 7) << 3;
    *(uint *)(DAT_001044a0 + g_mmreg_map) = DAT_001044b0 & 7 | (DAT_001044a8 & 7) << 3;
    *(uint *)(DAT_001044d8 + g_mmreg_map) = DAT_001044e8 & 7 | (DAT_001044e0 & 7) << 3;
    *(uint *)(DAT_00104510 + g_mmreg_map) = DAT_00104520 & 7 | (DAT_00104518 & 7) << 3;
    *(uint *)(DAT_00104548 + g_mmreg_map) = DAT_00104558 & 7 | (DAT_00104550 & 7) << 3;
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00104554;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_001043cc;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffd;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x10;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x13400000;
    mutex_lock(&mmsys_glob_reg_lock);
    mutex_unlock(*(undefined4 *)(g_mmreg_map + 0xc),&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1d8) = *(uint *)(g_mmreg_map + 0x1d8) & 0xfffffff8 | 3;
    *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 1;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar2 = 1;
    puVar2[4] = (ulong)DAT_00104474;
    printk(&DAT_001067b7,1,0,3,0,1);
    uVar1 = 0;
  }
  return uVar1;
}



====================ip_hw_dvfs_en.832@00111fe0====================
// signature: undefined ip_hw_dvfs_en.832(void)

undefined8 ip_hw_dvfs_en_832(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================ip_auto_tune_en.833@00112080====================
// signature: undefined ip_auto_tune_en.833(void)

undefined8 ip_auto_tune_en_833(undefined8 param_1,long param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================set_work_freq.834@00112120====================
// signature: undefined set_work_freq.834(void)

undefined8 set_work_freq_834(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_001043cc < param_2) {
    if (DAT_00104404 < param_2) {
      if (DAT_0010443c < param_2) {
        if (DAT_00104474 < param_2) {
          if (DAT_001044ac < param_2) {
            if (DAT_001044e4 < param_2) {
              uVar1 = 6;
              if (DAT_0010451c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1d8) = *(uint *)(g_mmreg_map + 0x1d8) & 0xfffffff8 | uVar1;
  printk(&DAT_00105f36,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq.835@00112264====================
// signature: undefined set_idle_freq.835(void)

undefined8 set_idle_freq_835(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_001043cc < param_2) {
    if (DAT_00104404 < param_2) {
      if (DAT_0010443c < param_2) {
        if (DAT_00104474 < param_2) {
          if (DAT_001044ac < param_2) {
            if (DAT_001044e4 < param_2) {
              uVar1 = 6;
              if (DAT_0010451c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}



====================get_ip_dvfs_table.836@001123a8====================
// signature: undefined get_ip_dvfs_table.836(void)

undefined8 get_ip_dvfs_table_836(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3 * 0x38);
    lVar1 = lVar3 + 1;
    *puVar2 = (&mtx_dvfs_config_table)[lVar3 * 0xe];
    puVar2[5] = (&DAT_001043cc)[lVar3 * 0xe];
    puVar2[6] = (&DAT_001043d0)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_001043e8)[lVar3 * 7];
    puVar2[4] = (&DAT_001043c8)[lVar3 * 0xe];
    puVar2[7] = (&DAT_001043d4)[lVar3 * 0xe];
    puVar2[8] = (&DAT_001043d8)[lVar3 * 0xe];
    puVar2[9] = (&DAT_001043dc)[lVar3 * 0xe];
    puVar2[10] = (&DAT_001043e0)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_001043c0)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}



====================set_ip_dvfs_table.837@00112474====================
// signature: undefined set_ip_dvfs_table.837(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_837(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_001043c0 = param_2[1];
  _DAT_001043c8 = param_2[2];
  _mtx_dvfs_config_table = *param_2;
  _DAT_001043e0 = param_2[5];
  PTR_DAT_001043e8 = (undefined *)param_2[6];
  _DAT_001043d0 = param_2[3];
  _DAT_001043d8 = param_2[4];
  return 0;
}



====================get_ip_status.838@001124dc====================
// signature: undefined get_ip_status.838(void)

undefined8 get_ip_status_838(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = *(uint *)(g_mmreg_map + 0x38);
  uVar1 = *(uint *)(g_mmreg_map + 0x48) & 7;
  *(uint *)(param_2 + 0xc) = uVar1;
  if (uVar1 == DAT_001043d0) {
    lVar3 = 0;
  }
  else if (uVar1 == DAT_00104408) {
    lVar3 = 1;
  }
  else if (uVar1 == DAT_00104440) {
    lVar3 = 2;
  }
  else if (uVar1 == DAT_00104478) {
    lVar3 = 3;
  }
  else if (uVar1 == DAT_001044b0) {
    lVar3 = 4;
  }
  else if (uVar1 == DAT_001044e8) {
    lVar3 = 5;
  }
  else if (uVar1 == DAT_00104520) {
    lVar3 = 6;
  }
  else {
    if (uVar1 != DAT_00104558) goto LAB_001125e0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_001043cc)[lVar3 * 0xe];
LAB_001125e0:
  *(uint *)(param_2 + 8) = uVar2 >> 3 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================power_on_nb.839@0011260c====================
// signature: undefined power_on_nb.839(void)

void power_on_nb_839(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_831();
  return;
}



====================power_off_nb.840@0011264c====================
// signature: undefined power_off_nb.840(void)

void power_off_nb_840(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_00106598);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.841@0011268c====================
// signature: undefined set_ip_gfree_wait_delay.841(void)

void set_ip_gfree_wait_delay_841(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = (param_1 & 0x3ff) << 0x14;
  return;
}



====================set_ip_freq_upd_en_byp.842@001126dc====================
// signature: undefined set_ip_freq_upd_en_byp.842(void)

void set_ip_freq_upd_en_byp_842(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffd;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 2;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.843@00112774====================
// signature: undefined set_ip_freq_upd_delay_en.843(void)

void set_ip_freq_upd_delay_en_843(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.844@0011280c====================
// signature: undefined set_ip_freq_upd_hdsk_en.844(void)

void set_ip_freq_upd_hdsk_en_844(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffffef;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x10;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en.845@001128a4====================
// signature: undefined set_ip_dvfs_swtrig_en.845(void)

void set_ip_dvfs_swtrig_en_845(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  mutex_unlock(*(undefined4 *)(g_mmreg_map + 0xc),&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.846@00112910====================
// signature: undefined set_ip_dvfs_work_index.846(void)

void set_ip_dvfs_work_index_846(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1d8) = *(uint *)(g_mmreg_map + 0x1d8) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.847@00112970====================
// signature: undefined get_ip_dvfs_work_index.847(void)

void get_ip_dvfs_work_index_847(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1d8);
  return;
}



====================set_ip_dvfs_idle_index.848@001129c0====================
// signature: undefined set_ip_dvfs_idle_index.848(void)

void set_ip_dvfs_idle_index_848(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table.849@00112a38====================
// signature: undefined get_ip_index_from_table.849(void)

void get_ip_index_from_table_849(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_001043cc < param_2) {
    if (DAT_00104404 < param_2) {
      if (DAT_0010443c < param_2) {
        if (DAT_00104474 < param_2) {
          if (DAT_001044ac < param_2) {
            if (DAT_001044e4 < param_2) {
              uVar1 = 6;
              if (DAT_0010451c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.850@00112b20====================
// signature: undefined updata_target_freq.850(void)

undefined8
updata_target_freq_850(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_834(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value.851@00112b88====================
// signature: undefined set_fix_dvfs_value.851(void)

undefined8 set_fix_dvfs_value_851(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 0;
}



====================top_current_volt.852@00112c04====================
// signature: undefined top_current_volt.852(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_852(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.853@00112cd4====================
// signature: undefined mm_current_volt.853(void)

undefined8 mm_current_volt_853(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================ip_dvfs_init.910@00112d48====================
// signature: undefined ip_dvfs_init.910(void)

undefined8 ip_dvfs_init_910(long param_1)

{
  long lVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 *puVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar3 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar3 == (undefined8 *)0x0) {
    printk(&DAT_00105665);
  }
  else {
    lVar2 = 0;
    do {
      lVar1 = lVar2 + 0x38;
      *(uint *)(*(long *)((long)&DAT_00103cc0 + lVar2) + g_mmreg_map) =
           *(uint *)(*(long *)((long)&DAT_00103cc0 + lVar2) + g_mmreg_map) & 0xfffe0000 |
           (*(uint *)((long)&DAT_00103cdc + lVar2) & 7) << 0xe |
           (*(uint *)((long)&DAT_00103cc8 + lVar2) & 7) << 0xb |
           (*(uint *)((long)&DAT_00103cd4 + lVar2) & 0xf) << 7 |
           (*(uint *)((long)&DAT_00103cd8 + lVar2) & 0xf) << 3 |
           *(uint *)((long)&DAT_00103cd0 + lVar2) & 7;
      lVar2 = lVar1;
    } while (lVar1 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103e54;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00103ccc;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffd;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 1;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x134;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) | 7;
    *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | 5;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x20;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar3 = 1;
    puVar3[4] = (ulong)DAT_00103e54;
    printk(&DAT_00105b7f,1,0,7,5,1);
  }
  return 1;
}



====================ip_hw_dvfs_en.911@001130b8====================
// signature: undefined ip_hw_dvfs_en.911(void)

undefined8 ip_hw_dvfs_en_911(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 1;
}



====================ip_auto_tune_en.912@00113158====================
// signature: undefined ip_auto_tune_en.912(void)

undefined8 ip_auto_tune_en_912(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  return 1;
}



====================set_work_freq.913@00113198====================
// signature: undefined set_work_freq.913(void)

undefined8 set_work_freq_913(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103ccc < param_2) {
    if (DAT_00103d04 < param_2) {
      if (DAT_00103d3c < param_2) {
        if (DAT_00103d74 < param_2) {
          if (DAT_00103dac < param_2) {
            if (DAT_00103de4 < param_2) {
              uVar1 = 6;
              if (DAT_00103e1c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) & 0xfffffff8 | uVar1;
  printk(&DAT_00105f64,"set_work_freq",param_2);
  return 1;
}



====================set_idle_freq.914@001132dc====================
// signature: undefined set_idle_freq.914(void)

undefined8 set_idle_freq_914(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103ccc < param_2) {
    if (DAT_00103d04 < param_2) {
      if (DAT_00103d3c < param_2) {
        if (DAT_00103d74 < param_2) {
          if (DAT_00103dac < param_2) {
            if (DAT_00103de4 < param_2) {
              uVar1 = 6;
              if (DAT_00103e1c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 1;
}



====================get_ip_dvfs_table.915@00113420====================
// signature: undefined get_ip_dvfs_table.915(void)

undefined8 get_ip_dvfs_table_915(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3 * 0x38);
    lVar1 = lVar3 + 1;
    *puVar2 = *(undefined4 *)(&dcam_dvfs_config_table + lVar3 * 7);
    puVar2[5] = (&DAT_00103ccc)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00103cd0)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00103ce8)[lVar3 * 7];
    puVar2[4] = (&DAT_00103cc8)[lVar3 * 0xe];
    puVar2[7] = (&DAT_00103cd4)[lVar3 * 0xe];
    puVar2[8] = (&DAT_00103cd8)[lVar3 * 0xe];
    puVar2[9] = (&DAT_00103cdc)[lVar3 * 0xe];
    puVar2[10] = *(undefined4 *)(&DAT_00103ce0 + lVar3 * 7);
    *(undefined8 *)(puVar2 + 2) = (&DAT_00103cc0)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 1;
}



====================set_ip_dvfs_table.916@001134ec====================
// signature: undefined set_ip_dvfs_table.916(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_916(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103cc0 = param_2[1];
  _DAT_00103cc8 = param_2[2];
  dcam_dvfs_config_table = *param_2;
  DAT_00103ce0 = param_2[5];
  PTR_DAT_00103ce8 = (undefined *)param_2[6];
  _DAT_00103cd0 = param_2[3];
  _DAT_00103cd8 = param_2[4];
  return 1;
}



====================get_ip_status.917@00113554====================
// signature: undefined get_ip_status.917(void)

undefined8 get_ip_status_917(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 0xc & 7;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103cd0) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103d08) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103d40) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103d78) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103db0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103de8) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103e20) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103e58) goto LAB_00113658;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_00103ccc)[lVar3 * 0xe];
LAB_00113658:
  *(uint *)(param_2 + 8) = uVar1 >> 0x12 & 7;
  printk(&DAT_001059f0,"get_ip_status",uVar1,uVar1);
  return 1;
}



====================power_on_nb.918@00113688====================
// signature: undefined power_on_nb.918(void)

void power_on_nb_918(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_910();
  printk(&DAT_00104e63,"power_on_nb");
  return;
}



====================power_off_nb.919@001136b8====================
// signature: undefined power_off_nb.919(void)

void power_off_nb_919(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_001065af);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.920@001136f8====================
// signature: undefined set_ip_gfree_wait_delay.920(void)

void set_ip_gfree_wait_delay_920(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = param_1 & 0x3ff;
  return;
}



====================set_ip_freq_upd_en_byp.921@00113748====================
// signature: undefined set_ip_freq_upd_en_byp.921(void)

void set_ip_freq_upd_en_byp_921(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.922@001137e0====================
// signature: undefined set_ip_freq_upd_delay_en.922(void)

void set_ip_freq_upd_delay_en_922(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffd;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 2;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.923@00113878====================
// signature: undefined set_ip_freq_upd_hdsk_en.923(void)

void set_ip_freq_upd_hdsk_en_923(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en.924@00113910====================
// signature: undefined set_ip_dvfs_swtrig_en.924(void)

void set_ip_dvfs_swtrig_en_924(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.925@001139a8====================
// signature: undefined set_ip_dvfs_work_index.925(void)

void set_ip_dvfs_work_index_925(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.926@00113a08====================
// signature: undefined get_ip_dvfs_work_index.926(void)

void get_ip_dvfs_work_index_926(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1e0);
  return;
}



====================set_ip_dvfs_idle_index.927@00113a58====================
// signature: undefined set_ip_dvfs_idle_index.927(void)

void set_ip_dvfs_idle_index_927(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table.928@00113ad0====================
// signature: undefined get_ip_index_from_table.928(void)

void get_ip_index_from_table_928(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_00103ccc < param_2) {
    if (DAT_00103d04 < param_2) {
      if (DAT_00103d3c < param_2) {
        if (DAT_00103d74 < param_2) {
          if (DAT_00103dac < param_2) {
            if (DAT_00103de4 < param_2) {
              uVar1 = 6;
              if (DAT_00103e1c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.929@00113bb8====================
// signature: undefined updata_target_freq.929(void)

undefined8
updata_target_freq_929(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_913(param_1,param_3);
  }
  return 1;
}



====================set_fix_dvfs_value.930@00113c20====================
// signature: undefined set_fix_dvfs_value.930(void)

undefined8 set_fix_dvfs_value_930(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 1;
}



====================top_current_volt.931@00113c9c====================
// signature: undefined top_current_volt.931(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_931(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.932@00113d6c====================
// signature: undefined mm_current_volt.932(void)

undefined8 mm_current_volt_932(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 1;
}



====================ip_dvfs_init.987@00113de0====================
// signature: undefined ip_dvfs_init.987(void)

undefined8 ip_dvfs_init_987(long param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar7 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_00104bcc);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00104208 + lVar6);
      puVar2 = (uint *)((long)&DAT_00104220 + lVar6);
      plVar4 = (long *)((long)&DAT_00104200 + lVar6);
      puVar3 = (uint *)((long)&DAT_00104210 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00104394;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_0010420c;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffef;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffff7f;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x40;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x134;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | 5;
    *(uint *)(g_mmreg_map + 0x1cc) = *(uint *)(g_mmreg_map + 0x1cc) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 8;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_00104324;
    printk(&DAT_00104a19,1,0,5,0,1);
    uVar5 = 0;
  }
  return uVar5;
}



====================ip_hw_dvfs_en.988@0011413c====================
// signature: undefined ip_hw_dvfs_en.988(void)

undefined8 ip_hw_dvfs_en_988(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xfffffff7;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 8;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================ip_auto_tune_en.989@001141dc====================
// signature: undefined ip_auto_tune_en.989(void)

undefined8 ip_auto_tune_en_989(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010480c,"ip_auto_tune_en");
  return 0;
}



====================set_work_freq.990@0011420c====================
// signature: undefined set_work_freq.990(void)

undefined8 set_work_freq_990(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010420c < param_2) {
    if (DAT_00104244 < param_2) {
      if (DAT_0010427c < param_2) {
        if (DAT_001042b4 < param_2) {
          if (DAT_001042ec < param_2) {
            if (DAT_00104324 < param_2) {
              uVar1 = 6;
              if (DAT_0010435c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | uVar1;
  printk(&DAT_00105a0e,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq.991@00114350====================
// signature: undefined set_idle_freq.991(void)

undefined8 set_idle_freq_991(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010420c < param_2) {
    if (DAT_00104244 < param_2) {
      if (DAT_0010427c < param_2) {
        if (DAT_001042b4 < param_2) {
          if (DAT_001042ec < param_2) {
            if (DAT_00104324 < param_2) {
              uVar1 = 6;
              if (DAT_0010435c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1cc) = *(uint *)(g_mmreg_map + 0x1cc) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}



====================get_ip_dvfs_table.992@00114494====================
// signature: undefined get_ip_dvfs_table.992(void)

undefined8 get_ip_dvfs_table_992(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    lVar4 = lVar3 * 0x38;
    puVar2 = (undefined4 *)(param_2 + lVar4);
    lVar1 = lVar3 + 1;
    *puVar2 = *(undefined4 *)(&jpg_dvfs_config_table + lVar3 * 7);
    puVar2[5] = (&DAT_0010420c)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00104210)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00104228)[lVar3 * 7];
    puVar2[4] = (&DAT_00104208)[lVar3 * 0xe];
    puVar2[7] = *(undefined4 *)(&DAT_00104214 + lVar4);
    puVar2[8] = *(undefined4 *)(&DAT_00104218 + lVar3 * 7);
    puVar2[9] = *(undefined4 *)((long)&DAT_00104218 + lVar4 + 4);
    puVar2[10] = (&DAT_00104220)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_00104200)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}



====================set_ip_dvfs_table.993@00114560====================
// signature: undefined set_ip_dvfs_table.993(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_993(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00104200 = param_2[1];
  _DAT_00104208 = param_2[2];
  jpg_dvfs_config_table = *param_2;
  _DAT_00104220 = param_2[5];
  PTR_DAT_00104228 = (undefined *)param_2[6];
  _DAT_00104210 = param_2[3];
  DAT_00104218 = param_2[4];
  return 0;
}



====================get_ip_status.994@001145c8====================
// signature: undefined get_ip_status.994(void)

undefined8 get_ip_status_994(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 8 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00104210) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00104248) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00104280) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001042b8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_001042f0) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00104328) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00104360) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00104398) goto LAB_001146cc;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_0010420c)[lVar3 * 0xe];
LAB_001146cc:
  *(uint *)(param_2 + 8) = uVar1 >> 0xc & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================power_on_nb.995@001146f8====================
// signature: undefined power_on_nb.995(void)

void power_on_nb_995(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_987();
  return;
}



====================power_off_nb.996@00114738====================
// signature: undefined power_off_nb.996(void)

void power_off_nb_996(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_00105c93);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.997@00114778====================
// signature: undefined set_ip_gfree_wait_delay.997(void)

void set_ip_gfree_wait_delay_997(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = param_1 & 0x3ff;
  return;
}



====================set_ip_freq_upd_en_byp.998@001147c8====================
// signature: undefined set_ip_freq_upd_en_byp.998(void)

void set_ip_freq_upd_en_byp_998(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffef;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 0x10;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.999@00114860====================
// signature: undefined set_ip_freq_upd_delay_en.999(void)

void set_ip_freq_upd_delay_en_999(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffff7f;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x80;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.1000@001148f8====================
// signature: undefined set_ip_freq_upd_hdsk_en.1000(void)

void set_ip_freq_upd_hdsk_en_1000(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en.1001@00114990====================
// signature: undefined set_ip_dvfs_swtrig_en.1001(void)

void set_ip_dvfs_swtrig_en_1001(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffff7;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 8;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.1002@00114a28====================
// signature: undefined set_ip_dvfs_work_index.1002(void)

void set_ip_dvfs_work_index_1002(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.1003@00114a8c====================
// signature: undefined get_ip_dvfs_work_index.1003(void)

void get_ip_dvfs_work_index_1003(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1c8);
  return;
}



====================set_ip_dvfs_idle_index.1004@00114adc====================
// signature: undefined set_ip_dvfs_idle_index.1004(void)

void set_ip_dvfs_idle_index_1004(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1cc) = *(uint *)(g_mmreg_map + 0x1cc) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_index_from_table.1005@00114b54====================
// signature: undefined get_ip_index_from_table.1005(void)

void get_ip_index_from_table_1005(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_0010420c < param_2) {
    if (DAT_00104244 < param_2) {
      if (DAT_0010427c < param_2) {
        if (DAT_001042b4 < param_2) {
          if (DAT_001042ec < param_2) {
            if (DAT_00104324 < param_2) {
              uVar1 = 6;
              if (DAT_0010435c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.1006@00114c3c====================
// signature: undefined updata_target_freq.1006(void)

undefined8
updata_target_freq_1006(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_990(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value.1007@00114ca4====================
// signature: undefined set_fix_dvfs_value.1007(void)

undefined8 set_fix_dvfs_value_1007(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 0;
}



====================top_current_volt.1008@00114d20====================
// signature: undefined top_current_volt.1008(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_1008(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.1009@00114df0====================
// signature: undefined mm_current_volt.1009(void)

undefined8 mm_current_volt_1009(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================ip_dvfs_init.1051@00114e64====================
// signature: undefined ip_dvfs_init.1051(void)

undefined8 ip_dvfs_init_1051(long param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar7 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_001066ab);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00103948 + lVar6);
      puVar2 = (uint *)((long)&DAT_00103960 + lVar6);
      plVar4 = (long *)((long)&DAT_00103940 + lVar6);
      puVar3 = (uint *)((long)&DAT_00103950 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103ad4;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_0010394c;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffdff;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x100;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x4d000;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffb;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1d0) = *(uint *)(g_mmreg_map + 0x1d0) & 0xfffffff8 | 2;
    lVar6 = 0;
    *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8;
    do {
      puVar1 = (uint *)((long)&DAT_00103948 + lVar6);
      puVar2 = (uint *)((long)&DAT_00103960 + lVar6);
      plVar4 = (long *)((long)&DAT_00103940 + lVar6);
      puVar3 = (uint *)((long)&DAT_00103950 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 4;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_001039bc;
    printk(&DAT_00105893,1,0,2,0,1);
    uVar5 = 0;
  }
  return uVar5;
}



====================ip_hw_dvfs_en.1052@00115200====================
// signature: undefined ip_hw_dvfs_en.1052(void)

undefined8 ip_hw_dvfs_en_1052(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================ip_auto_tune_en.1053@001152a0====================
// signature: undefined ip_auto_tune_en.1053(void)

undefined8 ip_auto_tune_en_1053(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  return 0;
}



====================set_work_freq.1054@001152e0====================
// signature: undefined set_work_freq.1054(void)

undefined8 set_work_freq_1054(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010394c < param_2) {
    if (DAT_00103984 < param_2) {
      if (DAT_001039bc < param_2) {
        if (DAT_001039f4 < param_2) {
          if (DAT_00103a2c < param_2) {
            if (DAT_00103a64 < param_2) {
              uVar1 = 6;
              if (DAT_00103a9c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1d0) = *(uint *)(g_mmreg_map + 0x1d0) & 0xfffffff8 | uVar1;
  printk(&DAT_00104d49,"set_work_freq",param_2);
  return 0;
}



====================set_idle_freq.1055@00115424====================
// signature: undefined set_idle_freq.1055(void)

undefined8 set_idle_freq_1055(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010394c < param_2) {
    if (DAT_00103984 < param_2) {
      if (DAT_001039bc < param_2) {
        if (DAT_001039f4 < param_2) {
          if (DAT_00103a2c < param_2) {
            if (DAT_00103a64 < param_2) {
              uVar1 = 6;
              if (DAT_00103a9c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}



====================get_ip_dvfs_table.1056@00115568====================
// signature: undefined get_ip_dvfs_table.1056(void)

undefined8 get_ip_dvfs_table_1056(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010480c,"get_ip_dvfs_table");
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3);
    lVar1 = lVar3 + 0x38;
    *puVar2 = *(undefined4 *)((long)&cpp_dvfs_config_table + lVar3);
    puVar2[5] = *(undefined4 *)((long)&DAT_0010394c + lVar3);
    puVar2[6] = *(undefined4 *)((long)&DAT_00103950 + lVar3);
    *(undefined8 *)(puVar2 + 0xc) = *(undefined8 *)((long)&PTR_DAT_00103968 + lVar3);
    puVar2[4] = *(undefined4 *)((long)&DAT_00103948 + lVar3);
    puVar2[7] = *(undefined4 *)(&DAT_00103954 + lVar3);
    puVar2[8] = *(undefined4 *)((long)&DAT_00103958 + lVar3);
    puVar2[9] = *(undefined4 *)((long)&DAT_00103958 + lVar3 + 4);
    puVar2[10] = *(undefined4 *)((long)&DAT_00103960 + lVar3);
    *(undefined8 *)(puVar2 + 2) = *(undefined8 *)((long)&DAT_00103940 + lVar3);
    lVar3 = lVar1;
  } while (lVar1 != 0x1c0);
  return 0;
}



====================set_ip_dvfs_table.1057@00115614====================
// signature: undefined set_ip_dvfs_table.1057(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_1057(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103940 = param_2[1];
  _DAT_00103948 = param_2[2];
  cpp_dvfs_config_table = *param_2;
  _DAT_00103960 = param_2[5];
  PTR_DAT_00103968 = (undefined *)param_2[6];
  _DAT_00103950 = param_2[3];
  DAT_00103958 = param_2[4];
  return 0;
}



====================get_ip_status.1058@0011567c====================
// signature: undefined get_ip_status.1058(void)

undefined8 get_ip_status_1058(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 6 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103950) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103988) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_001039c0) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_001039f8) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103a30) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103a68) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103aa0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00103ad8) goto LAB_00115780;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_0010394c)[lVar3 * 0xe];
LAB_00115780:
  *(uint *)(param_2 + 8) = uVar1 >> 9 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================power_on_nb.1059@001157ac====================
// signature: undefined power_on_nb.1059(void)

void power_on_nb_1059(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init_1051();
  return;
}



====================power_off_nb.1060@001157ec====================
// signature: undefined power_off_nb.1060(void)

void power_off_nb_1060(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_001065d7);
    *puVar1 = 0;
  }
  return;
}



====================set_ip_gfree_wait_delay.1061@0011582c====================
// signature: undefined set_ip_gfree_wait_delay.1061(void)

void set_ip_gfree_wait_delay_1061(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = (param_1 & 0x3ff) << 10;
  return;
}



====================set_ip_freq_upd_en_byp.1062@0011587c====================
// signature: undefined set_ip_freq_upd_en_byp.1062(void)

void set_ip_freq_upd_en_byp_1062(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffff7;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 8;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_delay_en.1063@00115914====================
// signature: undefined set_ip_freq_upd_delay_en.1063(void)

void set_ip_freq_upd_delay_en_1063(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffdff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x200;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_freq_upd_hdsk_en.1064@001159ac====================
// signature: undefined set_ip_freq_upd_hdsk_en.1064(void)

void set_ip_freq_upd_hdsk_en_1064(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffeff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x100;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_swtrig_en.1065@00115a44====================
// signature: undefined set_ip_dvfs_swtrig_en.1065(void)

void set_ip_dvfs_swtrig_en_1065(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================set_ip_dvfs_work_index.1066@00115adc====================
// signature: undefined set_ip_dvfs_work_index.1066(void)

void set_ip_dvfs_work_index_1066(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1d0) = *(uint *)(g_mmreg_map + 0x1d0) & 0xfffffff8 | param_2;
  return;
}



====================get_ip_dvfs_work_index.1067@00115b40====================
// signature: undefined get_ip_dvfs_work_index.1067(void)

void get_ip_dvfs_work_index_1067(undefined8 param_1,undefined4 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_2 = *(undefined4 *)(g_mmreg_map + 0x1d0);
  return;
}



====================set_ip_dvfs_idle_index.1068@00115b90====================
// signature: undefined set_ip_dvfs_idle_index.1068(void)

void set_ip_dvfs_idle_index_1068(undefined8 param_1,uint param_2)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  long lVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = 0;
  *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8 | param_2;
  do {
    puVar1 = (uint *)((long)&DAT_00103948 + lVar5);
    puVar2 = (uint *)((long)&DAT_00103960 + lVar5);
    plVar4 = (long *)((long)&DAT_00103940 + lVar5);
    puVar3 = (uint *)((long)&DAT_00103950 + lVar5);
    lVar5 = lVar5 + 0x38;
    *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
  } while (lVar5 != 0x1c0);
  return;
}



====================get_ip_index_from_table.1069@00115c4c====================
// signature: undefined get_ip_index_from_table.1069(void)

void get_ip_index_from_table_1069(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_0010394c < param_2) {
    if (DAT_00103984 < param_2) {
      if (DAT_001039bc < param_2) {
        if (DAT_001039f4 < param_2) {
          if (DAT_00103a2c < param_2) {
            if (DAT_00103a64 < param_2) {
              uVar1 = 6;
              if (DAT_00103a9c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}



====================updata_target_freq.1070@00115d34====================
// signature: undefined updata_target_freq.1070(void)

undefined8
updata_target_freq_1070(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq_1054(param_1,param_3);
  }
  return 0;
}



====================set_fix_dvfs_value.1071@00115d9c====================
// signature: undefined set_fix_dvfs_value.1071(void)

undefined8 set_fix_dvfs_value_1071(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 1;
}



====================top_current_volt.1072@00115e18====================
// signature: undefined top_current_volt.1072(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 top_current_volt_1072(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&local_2c);
  if (iVar1 == 0) {
    local_2c = local_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = local_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == local_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================mm_current_volt.1073@00115ee8====================
// signature: undefined mm_current_volt.1073(void)

undefined8 mm_current_volt_1073(undefined8 param_1,uint *param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  msleep(1);
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  printk(&DAT_00105401,"mm_dvfs_read_current_volt",uVar1);
  *param_2 = uVar1 >> 0x18 & 7;
  printk(&DAT_0010480c,"mm_current_volt");
  return 0;
}



====================mm_sw_dvfs_onoff@00115f5c====================
// signature: undefined mm_sw_dvfs_onoff(void)

void mm_sw_dvfs_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,(ulong)(param_1 != 0) << 2,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================mmdcdc_sw_dvfs_onoff@00115fec====================
// signature: undefined mmdcdc_sw_dvfs_onoff(void)

void mmdcdc_sw_dvfs_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x18,0x100000,(ulong)(param_1 != 0) << 0x14,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================mmsys_sw_cgb_enable@0011607c====================
// signature: undefined mmsys_sw_cgb_enable(void)

void mmsys_sw_cgb_enable(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *DAT_00100c28 = *DAT_00100c28 & 0xffffff7f;
  }
  else {
    *DAT_00100c28 = *DAT_00100c28 | 0x80;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================mm_dvfs_hold_onoff@0011610c====================
// signature: undefined mm_dvfs_hold_onoff(void)

void mm_dvfs_hold_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"mm_dvfs_hold_onoff",param_1);
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *g_mmreg_map = *g_mmreg_map & 0xfffffffe;
  }
  else {
    *g_mmreg_map = *g_mmreg_map | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}



====================mmsys_init@0011618c====================
// signature: undefined mmsys_init(void)

undefined8 mmsys_init(long param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  printk(&DAT_00104ebe,0,0);
  *(undefined4 *)(lVar1 + 0x40) = 1;
  mutex_lock(&mmsys_glob_reg_lock);
  *DAT_00100c28 = *DAT_00100c28 | 0x80;
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x18,0x100000,0,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,0,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) | 2;
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================get_ip_status.1135@00116324====================
// signature: undefined get_ip_status.1135(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void get_ip_status_1135(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  printk(&DAT_00104eab,mm_power_flag);
  if (mm_power_flag != '\x01') {
    uVar7 = 0xffffffff;
    goto LAB_001170fc;
  }
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48);
  iVar6 = regmap_read(DAT_00100c20,0x20,&local_3c);
  uVar5 = (uint)DAT_00104048;
  if (iVar6 != 0) {
    printk(&DAT_00105e73,"get_ip_status");
    uVar7 = 0;
    goto LAB_001170fc;
  }
  uVar3 = local_3c >> 0x14 & 7;
  if (uVar3 == (uint)DAT_00104048) {
    lVar8 = 0;
LAB_00116490:
    *(undefined **)(param_2 + 0x58) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_00116490;
    }
  }
  uVar3 = uVar1 >> 0x18 & 7;
  if (uVar3 == uVar5) {
    lVar8 = 0;
LAB_00116558:
    *(undefined **)(param_2 + 0x50) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_00116558;
    }
  }
  uVar3 = uVar2 >> 3 & 7;
  uVar4 = uVar1 >> 6 & 7;
  if (uVar3 == DAT_00104050) {
    lVar8 = 0;
LAB_00116630:
    *(undefined4 *)(param_2 + 0x60) = *(undefined4 *)((long)&DAT_00104048 + lVar8 * 0x38 + 4);
  }
  else {
    if (uVar3 == DAT_00104088) {
      lVar8 = 1;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001040c0) {
      lVar8 = 2;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001040f8) {
      lVar8 = 3;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_00104130) {
      lVar8 = 4;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_00104168) {
      lVar8 = 5;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001041a0) {
      lVar8 = 6;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001041d8) {
      lVar8 = 7;
      goto LAB_00116630;
    }
  }
  if (uVar4 == uVar5) {
    lVar8 = 0;
LAB_001166f4:
    *(undefined **)(param_2 + 0x18) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar4 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_001166f4;
    }
  }
  uVar5 = uVar2 >> 6 & 3;
  uVar3 = uVar1 >> 9 & 7;
  if (uVar5 == DAT_00103950) {
    lVar8 = 0;
LAB_001167d0:
    *(undefined4 *)(param_2 + 0x70) = (&DAT_0010394c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103988) {
      lVar8 = 1;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_001039c0) {
      lVar8 = 2;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_001039f8) {
      lVar8 = 3;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103a30) {
      lVar8 = 4;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103a68) {
      lVar8 = 5;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103aa0) {
      lVar8 = 6;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103ad8) {
      lVar8 = 7;
      goto LAB_001167d0;
    }
  }
  if (uVar3 == DAT_00103948) {
    lVar8 = 0;
LAB_0011689c:
    *(undefined **)(param_2 + 0x38) = (&PTR_DAT_00103968)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103980) {
      lVar8 = 1;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_001039b8) {
      lVar8 = 2;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_001039f0) {
      lVar8 = 3;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a28) {
      lVar8 = 4;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a60) {
      lVar8 = 5;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a98) {
      lVar8 = 6;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103ad0) {
      lVar8 = 7;
      goto LAB_0011689c;
    }
  }
  uVar5 = uVar2 >> 0x17 & 3;
  uVar3 = uVar1 >> 0x15 & 7;
  if (uVar5 == DAT_00103e90) {
    lVar8 = 0;
LAB_00116978:
    *(undefined4 *)(param_2 + 0x6c) = (&DAT_00103e8c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103ec8) {
      lVar8 = 1;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f00) {
      lVar8 = 2;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f38) {
      lVar8 = 3;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f70) {
      lVar8 = 4;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103fa8) {
      lVar8 = 5;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103fe0) {
      lVar8 = 6;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00104018) {
      lVar8 = 7;
      goto LAB_00116978;
    }
  }
  if (uVar3 == DAT_00103e88) {
    lVar8 = 0;
LAB_00116a44:
    *(undefined **)(param_2 + 0x30) = (&PTR_DAT_00103ea8)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103ec0) {
      lVar8 = 1;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103ef8) {
      lVar8 = 2;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103f30) {
      lVar8 = 3;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103f68) {
      lVar8 = 4;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103fa0) {
      lVar8 = 5;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103fd8) {
      lVar8 = 6;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00104010) {
      lVar8 = 7;
      goto LAB_00116a44;
    }
  }
  uVar5 = uVar2 >> 8 & 3;
  uVar3 = uVar1 >> 0xc & 7;
  if (uVar5 == DAT_00104210) {
    lVar8 = 0;
LAB_00116b20:
    *(undefined4 *)(param_2 + 0x74) = (&DAT_0010420c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00104248) {
      lVar8 = 1;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104280) {
      lVar8 = 2;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_001042b8) {
      lVar8 = 3;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_001042f0) {
      lVar8 = 4;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104328) {
      lVar8 = 5;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104360) {
      lVar8 = 6;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104398) {
      lVar8 = 7;
      goto LAB_00116b20;
    }
  }
  if (uVar3 == DAT_00104208) {
    lVar8 = 0;
LAB_00116bec:
    *(undefined **)(param_2 + 0x40) = (&PTR_DAT_00104228)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104240) {
      lVar8 = 1;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104278) {
      lVar8 = 2;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_001042b0) {
      lVar8 = 3;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_001042e8) {
      lVar8 = 4;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104320) {
      lVar8 = 5;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104358) {
      lVar8 = 6;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104390) {
      lVar8 = 7;
      goto LAB_00116bec;
    }
  }
  uVar5 = uVar2 >> 10 & 3;
  local_3c = uVar1 >> 0xf & 7;
  if (uVar5 == DAT_00103b10) {
    lVar8 = 0;
LAB_00116ccc:
    *(undefined4 *)(param_2 + 0x68) = *(undefined4 *)((long)&DAT_00103b08 + lVar8 * 0x38 + 4);
  }
  else {
    if (uVar5 == DAT_00103b48) {
      lVar8 = 1;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103b80) {
      lVar8 = 2;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103bb8) {
      lVar8 = 3;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103bf0) {
      lVar8 = 4;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c28) {
      lVar8 = 5;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c60) {
      lVar8 = 6;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c98) {
      lVar8 = 7;
      goto LAB_00116ccc;
    }
  }
  if (local_3c == (uint)DAT_00103b08) {
    lVar8 = 0;
LAB_00116d98:
    *(undefined **)(param_2 + 0x28) = (&PTR_DAT_00103b28)[lVar8 * 7];
  }
  else {
    if (local_3c == DAT_00103b40) {
      lVar8 = 1;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103b78) {
      lVar8 = 2;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103bb0) {
      lVar8 = 3;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103be8) {
      lVar8 = 4;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c20) {
      lVar8 = 5;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c58) {
      lVar8 = 6;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c90) {
      lVar8 = 7;
      goto LAB_00116d98;
    }
  }
  uVar5 = uVar2 >> 0xc & 7;
  uVar3 = uVar1 >> 0x12 & 7;
  if (uVar5 == DAT_00103cd0) {
    lVar8 = 0;
LAB_00116e74:
    *(undefined4 *)(param_2 + 100) = (&DAT_00103ccc)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103d08) {
      lVar8 = 1;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103d40) {
      lVar8 = 2;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103d78) {
      lVar8 = 3;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103db0) {
      lVar8 = 4;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103de8) {
      lVar8 = 5;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103e20) {
      lVar8 = 6;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103e58) {
      lVar8 = 7;
      goto LAB_00116e74;
    }
  }
  if (uVar3 == DAT_00103cc8) {
    lVar8 = 0;
LAB_00116f40:
    *(undefined **)(param_2 + 0x20) = (&PTR_DAT_00103ce8)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103d00) {
      lVar8 = 1;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103d38) {
      lVar8 = 2;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103d70) {
      lVar8 = 3;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103da8) {
      lVar8 = 4;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103de0) {
      lVar8 = 5;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103e18) {
      lVar8 = 6;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103e50) {
      lVar8 = 7;
      goto LAB_00116f40;
    }
  }
  uVar2 = uVar2 & 7;
  uVar1 = uVar1 >> 3 & 7;
  if (uVar2 == DAT_001043d0) {
    lVar8 = 0;
LAB_0011701c:
    *(undefined4 *)(param_2 + 0x78) = (&DAT_001043cc)[lVar8 * 0xe];
  }
  else {
    if (uVar2 == DAT_00104408) {
      lVar8 = 1;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104440) {
      lVar8 = 2;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104478) {
      lVar8 = 3;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_001044b0) {
      lVar8 = 4;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_001044e8) {
      lVar8 = 5;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104520) {
      lVar8 = 6;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104558) {
      lVar8 = 7;
      goto LAB_0011701c;
    }
  }
  if (uVar1 == DAT_001043c8) {
    lVar8 = 0;
LAB_001170e8:
    *(undefined **)(param_2 + 0x48) = (&PTR_DAT_001043e8)[lVar8 * 7];
  }
  else {
    if (uVar1 == DAT_00104400) {
      lVar8 = 1;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104438) {
      lVar8 = 2;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104470) {
      lVar8 = 3;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_001044a8) {
      lVar8 = 4;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_001044e0) {
      lVar8 = 5;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104518) {
      lVar8 = 6;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104550) {
      lVar8 = 7;
      goto LAB_001170e8;
    }
  }
  uVar7 = 1;
LAB_001170fc:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}



====================power_on_nb.1136@0011712c====================
// signature: undefined power_on_nb.1136(void)

void power_on_nb_1136(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mmsys_init();
  mm_power_flag = 1;
  return;
}



====================power_off_nb.1137@0011718c====================
// signature: undefined power_off_nb.1137(void)

void power_off_nb_1137(void)

{
  mm_power_flag = 0;
  return;
}



====================set_mmsys_sw_dvfs_en@00117198====================
// signature: undefined set_mmsys_sw_dvfs_en(void)

undefined8 set_mmsys_sw_dvfs_en(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,(ulong)(param_2 != 0) << 2,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  printk(&DAT_0010480c,"set_mmsys_sw_dvfs_en");
  return 0;
}



====================set_mmsys_dvfs_hold_en@00117240====================
// signature: undefined set_mmsys_dvfs_hold_en(void)

undefined8 set_mmsys_dvfs_hold_en(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"set_mmsys_dvfs_hold_en",param_2);
  if (param_2 == 0) {
    *g_mmreg_map = *g_mmreg_map & 0xfffffffe;
  }
  else {
    *g_mmreg_map = *g_mmreg_map | 1;
  }
  printk(&DAT_0010480c,"set_mmsys_dvfs_hold_en");
  return 0;
}



====================set_mmsys_dvfs_clk_gate_ctrl@001172c0====================
// signature: undefined set_mmsys_dvfs_clk_gate_ctrl(void)

undefined8 set_mmsys_dvfs_clk_gate_ctrl(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"mm_dvfs_auto_gate_sel",param_2);
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  printk(&DAT_0010480c,"set_mmsys_dvfs_clk_gate_ctrl");
  return 0;
}



====================set_mmsys_dvfs_wait_window@00117364====================
// signature: undefined set_mmsys_dvfs_wait_window(void)

undefined8 set_mmsys_dvfs_wait_window(undefined8 param_1,undefined4 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(undefined4 *)(g_mmreg_map + 4) = param_2;
  printk(&DAT_0010480c,"set_mmsys_dvfs_wait_window");
  return 0;
}



====================set_mmsys_dvfs_min_volt@001173a4====================
// signature: undefined set_mmsys_dvfs_min_volt(void)

undefined8 set_mmsys_dvfs_min_volt(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x10) = *(uint *)(g_mmreg_map + 0x10) | param_2 & 7;
  printk(&DAT_0010480c,"set_mmsys_dvfs_min_volt");
  return 0;
}



====================mmsys_power_ctrl@001173f8====================
// signature: undefined mmsys_power_ctrl(void)

void mmsys_power_ctrl(undefined8 param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = DAT_00100c38;
  *DAT_00100c30 = 0x3208004;
  *puVar1 = 0x3c280;
  printk(&DAT_0010480c,"mmsys_power_ctrl");
  uVar2 = 1;
  if (param_2 == 0) {
    uVar2 = 2;
  }
  mmsys_notify_call_chain(uVar2);
  return;
}



====================__typeid__ZTSFiP7devfreqP15ip_dvfs_map_cfgE_global_addr@001175c0====================
// signature: undefined __typeid__ZTSFiP7devfreqP15ip_dvfs_map_cfgE_global_addr(void)

undefined8 __typeid__ZTSFiP7devfreqP15ip_dvfs_map_cfgE_global_addr(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3 * 0x38);
    lVar1 = lVar3 + 1;
    *puVar2 = (&fd_dvfs_config_table)[lVar3 * 0xe];
    puVar2[5] = (&DAT_00103e8c)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00103e90)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00103ea8)[lVar3 * 7];
    puVar2[4] = (&DAT_00103e88)[lVar3 * 0xe];
    puVar2[7] = (&DAT_00103e94)[lVar3 * 0xe];
    puVar2[8] = (&DAT_00103e98)[lVar3 * 0xe];
    puVar2[9] = (&DAT_00103e9c)[lVar3 * 0xe];
    puVar2[10] = (&DAT_00103ea0)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_00103e80)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}



====================__typeid__ZTSFvjE_global_addr@001175f8====================
// signature: undefined __typeid__ZTSFvjE_global_addr(void)

void __typeid__ZTSFvjE_global_addr(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = param_1 & 0x3ff;
  return;
}



====================__typeid__ZTSFiP7devfreqmmjE_global_addr@001176b8====================
// signature: undefined __typeid__ZTSFiP7devfreqmmjE_global_addr(void)

undefined8
__typeid__ZTSFiP7devfreqmmjE_global_addr
          (undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_4 == 1) {
    printk(&DAT_0010480c,"mmsys_adjust_target_freq");
  }
  else {
    set_work_freq(param_1,param_3);
  }
  return 0;
}



====================__typeid__ZTSFiP7devfreqmE_global_addr@001176d4====================
// signature: undefined __typeid__ZTSFiP7devfreqmE_global_addr(void)

undefined8 __typeid__ZTSFiP7devfreqmE_global_addr(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  return 0;
}



====================__typeid__ZTSFiP7devfreqPjE_global_addr@00117744====================
// signature: undefined __typeid__ZTSFiP7devfreqPjE_global_addr(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 __typeid__ZTSFiP7devfreqPjE_global_addr(undefined8 param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint uStack_2c;
  long lStack_28;
  
  *in_x18 = unaff_x30;
  lStack_28 = ___stack_chk_guard;
  uStack_2c = 0;
  msleep(1);
  iVar1 = regmap_read(DAT_00100c20,0x20,&uStack_2c);
  if (iVar1 == 0) {
    uStack_2c = uStack_2c >> 0x14 & 7;
    printk(&DAT_00105401,"top_mm_dvfs_current_volt");
    uVar2 = uStack_2c;
  }
  else {
    printk(&DAT_00105e73,"top_mm_dvfs_current_volt");
    uVar2 = 0;
  }
  *param_2 = uVar2;
  printk(&DAT_0010480c,"top_current_volt");
  if (___stack_chk_guard == lStack_28) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}



====================__typeid__ZTSFiP7devfreqP14ip_dvfs_statusE_global_addr@0011779c====================
// signature: undefined __typeid__ZTSFiP7devfreqP14ip_dvfs_statusE_global_addr(void)

undefined8 __typeid__ZTSFiP7devfreqP14ip_dvfs_statusE_global_addr(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48) >> 0x17 & 3;
  *(uint *)(param_2 + 0xc) = uVar2;
  if (uVar2 == DAT_00103e90) {
    lVar3 = 0;
  }
  else if (uVar2 == DAT_00103ec8) {
    lVar3 = 1;
  }
  else if (uVar2 == DAT_00103f00) {
    lVar3 = 2;
  }
  else if (uVar2 == DAT_00103f38) {
    lVar3 = 3;
  }
  else if (uVar2 == DAT_00103f70) {
    lVar3 = 4;
  }
  else if (uVar2 == DAT_00103fa8) {
    lVar3 = 5;
  }
  else if (uVar2 == DAT_00103fe0) {
    lVar3 = 6;
  }
  else {
    if (uVar2 != DAT_00104018) goto LAB_0010f5c0;
    lVar3 = 7;
  }
  *(undefined4 *)(param_2 + 0xc) = (&DAT_00103e8c)[lVar3 * 0xe];
LAB_0010f5c0:
  *(uint *)(param_2 + 8) = uVar1 >> 0x15 & 7;
  printk(&DAT_0010480c,"get_ip_status");
  return 0;
}



====================__typeid__ZTSFvP7devfreqE_global_addr@001177bc====================
// signature: undefined __typeid__ZTSFvP7devfreqE_global_addr(void)

void __typeid__ZTSFvP7devfreqE_global_addr(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  ip_dvfs_init();
  return;
}



====================__typeid__ZTSFiP7devfreqjE_global_addr@001177fc====================
// signature: undefined __typeid__ZTSFiP7devfreqjE_global_addr(void)

undefined8 __typeid__ZTSFiP7devfreqjE_global_addr(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}



====================__typeid__ZTSFvP7devfreqjE_global_addr@0011782c====================
// signature: undefined __typeid__ZTSFvP7devfreqjE_global_addr(void)

void __typeid__ZTSFvP7devfreqjE_global_addr(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1f0) = *(uint *)(g_mmreg_map + 0x1f0) & 0xfffffff8 | param_2;
  return;
}



====================init_module@00117868====================
// signature: undefined init_module(void)

undefined4 init_module(void)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105e9e);
  mmsys_dvfs_init();
  cpp_dvfs_init();
  fd_dvfs_init();
  isp_dvfs_init();
  jpg_dvfs_init();
  dcam_if_dvfs_init();
  dcam_axi_dvfs_init();
  uVar1 = mtx_dvfs_init();
  printk(&DAT_00105db7);
  return uVar1;
}



====================cleanup_module@001178c8====================
// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105530);
  fd_dvfs_exit();
  cpp_dvfs_exit();
  isp_dvfs_exit();
  jpg_dvfs_exit();
  dcam_if_dvfs_exit();
  dcam_axi_dvfs_exit();
  mtx_dvfs_exit();
  mmsys_dvfs_exit();
  printk(&DAT_00104caa);
  return;
}



====================printk@0011b000====================
// signature: undefined printk(void)

/* WARNING: Control flow encountered bad instruction data */

void printk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devfreq_add_governor@0011b008====================
// signature: undefined devfreq_add_governor(void)

/* WARNING: Control flow encountered bad instruction data */

void devfreq_add_governor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__platform_driver_register@0011b010====================
// signature: undefined __platform_driver_register(void)

/* WARNING: Control flow encountered bad instruction data */

void __platform_driver_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devfreq_remove_governor@0011b018====================
// signature: undefined devfreq_remove_governor(void)

/* WARNING: Control flow encountered bad instruction data */

void devfreq_remove_governor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================platform_driver_unregister@0011b020====================
// signature: undefined platform_driver_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void platform_driver_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sysfs_create_group@0011b028====================
// signature: undefined sysfs_create_group(void)

/* WARNING: Control flow encountered bad instruction data */

void sysfs_create_group(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sysfs_remove_group@0011b030====================
// signature: undefined sysfs_remove_group(void)

/* WARNING: Control flow encountered bad instruction data */

void sysfs_remove_group(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__mutex_init@0011b038====================
// signature: undefined __mutex_init(void)

/* WARNING: Control flow encountered bad instruction data */

void __mutex_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_pm_opp_of_add_table@0011b040====================
// signature: undefined dev_pm_opp_of_add_table(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_pm_opp_of_add_table(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================_dev_err@0011b048====================
// signature: undefined _dev_err(void)

/* WARNING: Control flow encountered bad instruction data */

void _dev_err(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_devfreq_add_device@0011b050====================
// signature: undefined devm_devfreq_add_device(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_devfreq_add_device(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_pm_opp_of_remove_table@0011b058====================
// signature: undefined dev_pm_opp_of_remove_table(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_pm_opp_of_remove_table(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devm_kmalloc@0011b060====================
// signature: undefined devm_kmalloc(void)

/* WARNING: Control flow encountered bad instruction data */

void devm_kmalloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_property_read_variable_u32_array@0011b068====================
// signature: undefined of_property_read_variable_u32_array(void)

/* WARNING: Control flow encountered bad instruction data */

void of_property_read_variable_u32_array(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sprd_mm_pw_notify_register@0011b070====================
// signature: undefined sprd_mm_pw_notify_register(void)

/* WARNING: Control flow encountered bad instruction data */

void sprd_mm_pw_notify_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sprd_mm_pw_notify_unregister@0011b078====================
// signature: undefined sprd_mm_pw_notify_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void sprd_mm_pw_notify_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__ioremap@0011b080====================
// signature: undefined __ioremap(void)

/* WARNING: Control flow encountered bad instruction data */

void __ioremap(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================of_parse_phandle@0011b088====================
// signature: undefined of_parse_phandle(void)

/* WARNING: Control flow encountered bad instruction data */

void of_parse_phandle(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================syscon_node_to_regmap@0011b090====================
// signature: undefined syscon_node_to_regmap(void)

/* WARNING: Control flow encountered bad instruction data */

void syscon_node_to_regmap(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__cfi_slowpath@0011b098====================
// signature: undefined __cfi_slowpath(void)

/* WARNING: Control flow encountered bad instruction data */

void __cfi_slowpath(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_lock@0011b0c0====================
// signature: undefined mutex_lock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_lock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sprintf@0011b0c8====================
// signature: int sprintf(char * __s, char * __format, ...)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sprintf(char *__s,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================mutex_unlock@0011b0d0====================
// signature: undefined mutex_unlock(void)

/* WARNING: Control flow encountered bad instruction data */

void mutex_unlock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================sscanf@0011b0d8====================
// signature: int sscanf(char * __s, char * __format, ...)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sscanf(char *__s,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_update_bits_base@0011b0e0====================
// signature: undefined regmap_update_bits_base(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_update_bits_base(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================msleep@0011b0e8====================
// signature: undefined msleep(void)

/* WARNING: Control flow encountered bad instruction data */

void msleep(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================regmap_read@0011b0f0====================
// signature: undefined regmap_read(void)

/* WARNING: Control flow encountered bad instruction data */

void regmap_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================strncmp@0011b0f8====================
// signature: int strncmp(char * __s1, char * __s2, size_t __n)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncmp(char *__s1,char *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================blocking_notifier_chain_register@0011b100====================
// signature: undefined blocking_notifier_chain_register(void)

/* WARNING: Control flow encountered bad instruction data */

void blocking_notifier_chain_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================blocking_notifier_chain_unregister@0011b108====================
// signature: undefined blocking_notifier_chain_unregister(void)

/* WARNING: Control flow encountered bad instruction data */

void blocking_notifier_chain_unregister(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================blocking_notifier_call_chain@0011b110====================
// signature: undefined blocking_notifier_call_chain(void)

/* WARNING: Control flow encountered bad instruction data */

void blocking_notifier_call_chain(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__list_add_valid@0011b118====================
// signature: undefined __list_add_valid(void)

/* WARNING: Control flow encountered bad instruction data */

void __list_add_valid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__list_del_entry_valid@0011b120====================
// signature: undefined __list_del_entry_valid(void)

/* WARNING: Control flow encountered bad instruction data */

void __list_del_entry_valid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================kfree@0011b128====================
// signature: undefined kfree(void)

/* WARNING: Control flow encountered bad instruction data */

void kfree(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================kmem_cache_alloc_trace@0011b130====================
// signature: undefined kmem_cache_alloc_trace(void)

/* WARNING: Control flow encountered bad instruction data */

void kmem_cache_alloc_trace(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devfreq_recommended_opp@0011b140====================
// signature: undefined devfreq_recommended_opp(void)

/* WARNING: Control flow encountered bad instruction data */

void devfreq_recommended_opp(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_pm_opp_get_freq@0011b148====================
// signature: undefined dev_pm_opp_get_freq(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_pm_opp_get_freq(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_pm_opp_get_voltage@0011b150====================
// signature: undefined dev_pm_opp_get_voltage(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_pm_opp_get_voltage(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================dev_pm_opp_put@0011b158====================
// signature: undefined dev_pm_opp_put(void)

/* WARNING: Control flow encountered bad instruction data */

void dev_pm_opp_put(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================devfreq_event_get_event@0011b160====================
// signature: undefined devfreq_event_get_event(void)

/* WARNING: Control flow encountered bad instruction data */

void devfreq_event_get_event(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================update_devfreq@0011b168====================
// signature: undefined update_devfreq(void)

/* WARNING: Control flow encountered bad instruction data */

void update_devfreq(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__stack_chk_fail@0011b178====================
// signature: noreturn undefined __stack_chk_fail(void)

/* WARNING: Control flow encountered bad instruction data */

void __stack_chk_fail(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================__ubsan_handle_cfi_check_fail_abort@0011b188====================
// signature: undefined __ubsan_handle_cfi_check_fail_abort(void)

/* WARNING: Control flow encountered bad instruction data */

void __ubsan_handle_cfi_check_fail_abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



====================memset@0011b190====================
// signature: void * memset(void * __s, int __c, size_t __n)

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



