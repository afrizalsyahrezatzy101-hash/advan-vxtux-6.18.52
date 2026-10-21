// vsp_pd_probe @ 00101030
// signature: undefined vsp_pd_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void vsp_pd_probe(long param_1)

{
  long lVar1;
  int iVar2;
  long *plVar3;
  undefined8 uVar4;
  long lVar5;
  ulong uVar6;
  char *pcVar7;
  char *pcVar8;
  undefined *puVar9;
  undefined8 *in_x18;
  undefined8 *puVar10;
  undefined8 unaff_x30;
  undefined8 local_58;
  char *local_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_48 = ___stack_chk_guard;
  puVar10 = *(undefined8 **)(param_1 + 0x310);
  local_58 = 0;
  _dev_info(lVar1,"%s, %d\n","vsp_pd_probe",0xc5);
  plVar3 = (long *)devm_kmalloc(lVar1,0x638,0xdc0);
  if (plVar3 == (long *)0x0) {
    uVar4 = 0xfffffff4;
    goto LAB_00101098;
  }
  *plVar3 = lVar1;
  lVar5 = kstrdup(*puVar10,0xcc0);
  plVar3[0xa4] = lVar5;
  plVar3[0xa9] = (long)&LAB_001020d8;
  plVar3[0xaa] = (long)&LAB_001020dc;
  uVar6 = syscon_regmap_lookup_by_phandle_args(puVar10,"pmu-vsp-auto-shutdown-syscon",2,&local_58);
  if (uVar6 < 0xfffffffffffff001) {
    plVar3[0xbf] = uVar6;
    *(undefined4 *)(plVar3 + 0xc3) = (undefined4)local_58;
    *(undefined4 *)(plVar3 + 0xc5) = local_58._4_4_;
    _dev_info(lVar1,"VSP syscon[%s]%p, offset 0x%x, mask 0x%x\n","pmu-vsp-auto-shutdown-syscon",
              uVar6);
  }
  else {
    _dev_err(lVar1,"Read Vsp Dts %s regmap fail\n","pmu-vsp-auto-shutdown-syscon");
    plVar3[0xbf] = 0;
    *(undefined4 *)(plVar3 + 0xc3) = 0;
    *(undefined4 *)(plVar3 + 0xc5) = 0;
  }
  uVar6 = syscon_regmap_lookup_by_phandle_args(puVar10,"pmu-vsp-force-shutdown-syscon",2,&local_58);
  if (uVar6 < 0xfffffffffffff001) {
    plVar3[0xc0] = uVar6;
    *(undefined4 *)((long)plVar3 + 0x61c) = (undefined4)local_58;
    *(undefined4 *)((long)plVar3 + 0x62c) = local_58._4_4_;
    _dev_info(lVar1,"VSP syscon[%s]%p, offset 0x%x, mask 0x%x\n","pmu-vsp-force-shutdown-syscon",
              uVar6);
  }
  else {
    _dev_err(lVar1,"Read Vsp Dts %s regmap fail\n","pmu-vsp-force-shutdown-syscon");
    plVar3[0xc0] = 0;
    *(undefined4 *)((long)plVar3 + 0x61c) = 0;
    *(undefined4 *)((long)plVar3 + 0x62c) = 0;
  }
  uVar6 = syscon_regmap_lookup_by_phandle_args(puVar10,"pmu-pwr-status-syscon",2,&local_58);
  if (uVar6 < 0xfffffffffffff001) {
    plVar3[0xc1] = uVar6;
    *(undefined4 *)(plVar3 + 0xc4) = (undefined4)local_58;
    *(undefined4 *)(plVar3 + 0xc6) = local_58._4_4_;
    _dev_info(lVar1,"VSP syscon[%s]%p, offset 0x%x, mask 0x%x\n","pmu-pwr-status-syscon",uVar6);
  }
  else {
    _dev_err(lVar1,"Read Vsp Dts %s regmap fail\n","pmu-pwr-status-syscon");
    plVar3[0xc1] = 0;
    *(undefined4 *)(plVar3 + 0xc4) = 0;
    *(undefined4 *)(plVar3 + 0xc6) = 0;
  }
  uVar6 = syscon_regmap_lookup_by_phandle_args(puVar10,"vsp-domain-eb-syscon",2,&local_58);
  if (uVar6 < 0xfffffffffffff001) {
    plVar3[0xc2] = uVar6;
    *(undefined4 *)((long)plVar3 + 0x624) = (undefined4)local_58;
    *(undefined4 *)((long)plVar3 + 0x634) = local_58._4_4_;
    _dev_info(lVar1,"VSP syscon[%s]%p, offset 0x%x, mask 0x%x\n","vsp-domain-eb-syscon",uVar6);
  }
  else {
    _dev_err(lVar1,"Read Vsp Dts %s regmap fail\n","vsp-domain-eb-syscon");
    plVar3[0xc2] = 0;
    *(undefined4 *)((long)plVar3 + 0x624) = 0;
    *(undefined4 *)((long)plVar3 + 0x634) = 0;
  }
  local_50 = (char *)0x0;
  lVar5 = of_find_node_opts_by_path("/chosen",0);
  if ((lVar5 == 0) ||
     (iVar2 = of_property_read_string(lVar5,"bootargs",&local_50), pcVar8 = local_50, iVar2 < 0)) {
LAB_001012f0:
    pm_genpd_init(plVar3 + 1,0,1);
    of_genpd_add_provider_simple(puVar10,plVar3 + 1);
    __pm_runtime_set_status(lVar1,0);
    __pm_runtime_use_autosuspend(lVar1,1);
    pm_runtime_enable(lVar1);
  }
  else {
    pcVar7 = strstr(local_50,"androidboot.mode=cali");
    if (pcVar7 != (char *)0x0) {
      iVar2 = 1;
    }
    pcVar8 = strstr(pcVar8,"androidboot.mode=autotest");
    if (pcVar8 != (char *)0x0) {
      iVar2 = 2;
    }
    printk(&DAT_0010024f,"boot_mode_check",iVar2);
    if (iVar2 == 0) goto LAB_001012f0;
    if (plVar3[0xc2] == 0) {
      iVar2 = regmap_update_bits_base
                        (plVar3[0xc0],*(undefined4 *)((long)plVar3 + 0x61c),
                         *(undefined4 *)((long)plVar3 + 0x62c),*(undefined4 *)((long)plVar3 + 0x62c)
                         ,0,0,0);
      if (iVar2 != 0) {
        uVar4 = 0x62;
        goto LAB_00101404;
      }
LAB_001013f4:
      puVar9 = &DAT_001003ee;
    }
    else {
      printk(&DAT_0010017e);
      iVar2 = regmap_update_bits_base
                        (plVar3[0xc2],*(undefined4 *)((long)plVar3 + 0x624),
                         *(undefined4 *)((long)plVar3 + 0x634),0,0,0,0);
      if (iVar2 == 0) goto LAB_001013f4;
      uVar4 = 0x57;
LAB_00101404:
      printk(&DAT_001001b1,"vsp_shutdown",uVar4);
      puVar9 = &DAT_00100111;
    }
    printk(puVar9,"vsp_pd_probe");
  }
  uVar4 = 0;
LAB_00101098:
  if (___stack_chk_guard == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}


