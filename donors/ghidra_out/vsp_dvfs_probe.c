// vsp_dvfs_probe @ 00109be8
// signature: undefined vsp_dvfs_probe(void)
// calling convention: unknown


undefined4 vsp_dvfs_probe(long param_1)

{
  long lVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  undefined8 *puVar9;
  undefined *puVar10;
  ulong uVar11;
  undefined1 *puVar12;
  undefined8 *in_x18;
  undefined8 uVar13;
  code *pcVar14;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar13 = *(undefined8 *)(param_1 + 0x310);
  lVar1 = param_1 + 0x10;
  lVar7 = devm_kmalloc(lVar1,0x4b0,0xdc0);
  if ((lVar7 == 0) || (lVar8 = find_apsys_device_by_name("apsys-dvfs"), lVar8 == 0)) {
    return 0xfffffff4;
  }
  *(long *)(lVar7 + 0x4a8) = lVar8;
  puVar9 = (undefined8 *)of_device_get_match_data(lVar1);
  if (puVar9 == (undefined8 *)0x0) {
    puVar10 = &DAT_00101ed7;
  }
  else {
    lVar8 = puVar9[2];
    *(long *)(lVar7 + 1000) = lVar8;
    *(undefined4 *)(lVar7 + 0x4a0) = *(undefined4 *)(puVar9 + 1);
    if (lVar8 == 0) {
      printk(&DAT_00101d7e,*puVar9);
      return 0xffffffea;
    }
    uVar3 = of_property_read_variable_u32_array
                      (uVar13,"sprd,dvfs-work-freq",(undefined4 *)(lVar7 + 0x3d0),1,0);
    uVar4 = of_property_read_variable_u32_array
                      (uVar13,"sprd,dvfs-idle-freq",(undefined4 *)(lVar7 + 0x3d4),1,0);
    uVar5 = of_property_read_variable_u32_array
                      (uVar13,"sprd,dvfs-enable-flag",(undefined4 *)(lVar7 + 0x474),1,0);
    if (((uVar4 & (int)uVar4 >> 0x1f) == 0 && (uVar3 & (int)uVar3 >> 0x1f) == 0) &&
        (uVar5 & (int)uVar5 >> 0x1f) == 0) {
      printk(&DAT_001031b2,*(undefined4 *)(lVar7 + 0x3d0),*(undefined4 *)(lVar7 + 0x3d4),
             *(undefined4 *)(lVar7 + 0x474));
      iVar6 = dev_pm_opp_of_add_table(lVar1);
      if (iVar6 != 0) {
        _dev_err(lVar1,"Invalid operating-points in device tree.\n");
        return 0xffffffea;
      }
      *(undefined1 **)(lVar7 + 0x488) = &LAB_00118170;
      iVar6 = strcmp("qogirn6pro-vpuenc",(char *)*puVar9);
      if (iVar6 == 0) {
        puVar12 = &vpuenc_dvfs_chain;
      }
      else {
        puVar12 = &vsp_dvfs_chain;
      }
      blocking_notifier_chain_register(puVar12,lVar7 + 0x488);
      *(long *)(param_1 + 0xa8) = lVar7;
      uVar11 = devm_devfreq_add_device(lVar1,&vsp_dvfs_profile,"vsp_dvfs",0);
      *(ulong *)(lVar7 + 0x3d8) = uVar11;
      if (0xfffffffffffff000 < uVar11) {
        _dev_err(lVar1,"failed to add devfreq dev with vsp-dvfs governor\n");
        uVar2 = *(undefined4 *)(lVar7 + 0x3d8);
        dev_pm_opp_of_remove_table(lVar1);
        iVar6 = strcmp("qogirn6pro-vpuenc",(char *)*puVar9);
        if (iVar6 == 0) {
          puVar12 = &vpuenc_dvfs_chain;
        }
        else {
          puVar12 = &vsp_dvfs_chain;
        }
        blocking_notifier_chain_unregister(puVar12,lVar7 + 0x488);
        return uVar2;
      }
      puVar9 = *(undefined8 **)(lVar7 + 1000);
      if (puVar9 != (undefined8 *)0x0) {
        pcVar14 = (code *)*puVar9;
        if (pcVar14 != (code *)0x0) {
          if (5 < ((ulong)(pcVar14 + -0x117dc4) >> 2 | (long)pcVar14 << 0x3e)) {
            __cfi_slowpath(0x81617324bfd2d865,pcVar14);
          }
          (*pcVar14)(lVar7,uVar13);
          puVar9 = *(undefined8 **)(lVar7 + 1000);
          if (puVar9 == (undefined8 *)0x0) goto LAB_00109e90;
        }
        pcVar14 = (code *)puVar9[1];
        if (pcVar14 != (code *)0x0) {
          if (5 < ((ulong)(pcVar14 + -0x117ddc) >> 2 | (long)pcVar14 << 0x3e)) {
            __cfi_slowpath(0x9fd2f8a3e5e47234,pcVar14);
          }
          (*pcVar14)(lVar7);
        }
      }
LAB_00109e90:
      printk(&DAT_00102c81);
      return 0;
    }
    puVar10 = &DAT_001029a8;
  }
  printk(puVar10);
  return 0xffffffea;
}


