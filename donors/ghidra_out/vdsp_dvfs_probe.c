// vdsp_dvfs_probe @ 0010ae90
// signature: undefined vdsp_dvfs_probe(void)
// calling convention: unknown


uint vdsp_dvfs_probe(long param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  long lVar6;
  undefined8 *puVar7;
  ulong uVar8;
  undefined8 *in_x18;
  undefined8 uVar9;
  code *pcVar10;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar9 = *(undefined8 *)(param_1 + 0x310);
  lVar1 = param_1 + 0x10;
  puVar5 = (undefined4 *)devm_kmalloc(lVar1,0x490,0xdc0);
  if ((puVar5 == (undefined4 *)0x0) || (lVar6 = find_apsys_device_by_name("apsys-dvfs"), lVar6 == 0)
     ) {
    return 0xfffffff4;
  }
  *(long *)(puVar5 + 0x122) = lVar6;
  puVar7 = (undefined8 *)of_device_get_match_data(lVar1);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_00101ed7);
    return 0xffffffea;
  }
  *(undefined8 *)(puVar5 + 0x120) = *puVar7;
  printk(&DAT_001034f0,"vdsp_dvfs_parse_dt");
  uVar2 = of_property_read_variable_u32_array(uVar9,"sprd,hw-dfs-en",puVar5 + 0x100,1,0);
  uVar3 = of_property_read_variable_u32_array(uVar9,"sprd,work-index-def",puVar5 + 0x101,1,0);
  uVar4 = of_property_read_variable_u32_array(uVar9,"sprd,idle-index-def",puVar5 + 0x102,1,0);
  uVar2 = uVar3 & (int)uVar3 >> 0x1f | uVar2 & (int)uVar2 >> 0x1f | uVar4 & (int)uVar4 >> 0x1f;
  if (uVar2 != 0) {
    printk(&DAT_00102b96);
    return uVar2;
  }
  uVar2 = dev_pm_opp_of_add_table(lVar1);
  if (uVar2 != 0) {
    _dev_err(lVar1,"invalid operating-points in device tree.\n");
    return uVar2;
  }
  *(undefined1 **)(puVar5 + 0xf2) = &LAB_00118174;
  uVar2 = blocking_notifier_chain_register(&vdsp_dvfs_chain,puVar5 + 0xf2);
  if (uVar2 == 0) {
    *(undefined4 **)(param_1 + 0xa8) = puVar5;
    uVar8 = devm_devfreq_add_device(lVar1,&vdsp_dvfs_profile,"vdsp_dvfs",0);
    *(ulong *)(puVar5 + 0xec) = uVar8;
    if (uVar8 < 0xfffffffffffff001) {
      device_rename(uVar8 + 0x30,&DAT_00101e97);
      puVar7 = *(undefined8 **)(puVar5 + 0x120);
      *puVar5 = 1;
      if (puVar7 != (undefined8 *)0x0) {
        pcVar10 = (code *)*puVar7;
        if (pcVar10 != (code *)0x0) {
          if (1 < ((ulong)(pcVar10 + -0x117f78) >> 2 | (long)pcVar10 << 0x3e)) {
            __cfi_slowpath(0x3c4797a62497b400,pcVar10);
          }
          (*pcVar10)(puVar5,uVar9);
          puVar7 = *(undefined8 **)(puVar5 + 0x120);
          if (puVar7 == (undefined8 *)0x0) goto LAB_0010b120;
        }
        pcVar10 = (code *)puVar7[1];
        if (pcVar10 != (code *)0x0) {
          if (pcVar10 != __typeid__ZTSFiP9vdsp_dvfsP6deviceE_global_addr) {
            __cfi_slowpath(0xfbecd39c1944443a,pcVar10);
          }
          (*pcVar10)(puVar5,lVar1);
          puVar7 = *(undefined8 **)(puVar5 + 0x120);
          if (puVar7 == (undefined8 *)0x0) goto LAB_0010b120;
        }
        pcVar10 = (code *)puVar7[2];
        if (pcVar10 != (code *)0x0) {
          if (5 < ((ulong)(pcVar10 + -0x117f90) >> 2 | (long)pcVar10 << 0x3e)) {
            __cfi_slowpath(0x9974caa7c4cff8b7,pcVar10);
          }
          (*pcVar10)(puVar5);
        }
      }
LAB_0010b120:
      printk(&DAT_00102015);
      return 0;
    }
    _dev_err(lVar1,"failed to add devfreq dev with vdsp-dvfs governor\n");
    uVar2 = puVar5[0xec];
  }
  else {
    _dev_err(lVar1,"failed to register vdsp layer change notifier\n");
  }
  dev_pm_opp_of_remove_table(lVar1);
  blocking_notifier_chain_unregister(&vdsp_dvfs_chain,puVar5 + 0xf2);
  return uVar2;
}


