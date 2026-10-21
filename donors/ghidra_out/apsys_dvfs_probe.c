// apsys_dvfs_probe @ 00106220
// signature: undefined apsys_dvfs_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int apsys_dvfs_probe(long param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined8 *puVar4;
  ulong uVar5;
  undefined *puVar6;
  undefined8 uVar7;
  undefined8 *in_x18;
  undefined8 uVar8;
  code *pcVar9;
  undefined8 unaff_x30;
  long local_b0;
  long lStack_a8;
  int local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_48 = ___stack_chk_guard;
  uVar8 = *(undefined8 *)(param_1 + 0x310);
  memset(&local_b0,0,0x60);
  lVar3 = devm_kmalloc(lVar1,0x410,0xdc0);
  if (lVar3 == 0) {
    iVar2 = -0xc;
    goto LAB_0010627c;
  }
  puVar4 = (undefined8 *)of_device_get_match_data(lVar1);
  if (puVar4 == (undefined8 *)0x0) {
    printk(&DAT_00101ed7);
    iVar2 = -0x16;
    goto LAB_0010627c;
  }
  *(undefined8 *)(lVar3 + 0x3d0) = *puVar4;
  iVar2 = strcmp("qogirn6pro",(char *)puVar4[1]);
  if (iVar2 == 0) {
    local_4c = 0;
    iVar2 = sprd_get_soc_id(4,&local_4c,1);
    if (iVar2 == 0) {
      if (local_4c == 0) {
        puVar6 = &DAT_001023ef;
      }
      else {
        if (local_4c != 1) {
          puVar6 = &DAT_00102820;
          goto LAB_00106524;
        }
        puVar6 = &DAT_00102a7e;
      }
      printk(puVar6);
      if (local_4c != 0) goto LAB_00106348;
    }
    else {
      puVar6 = &DAT_00102af4;
LAB_00106524:
      printk(puVar6);
    }
    printk(&DAT_00103698,puVar4[1]);
    n6pro_AA_flag = 1;
    iVar2 = -0x16;
    goto LAB_0010627c;
  }
LAB_00106348:
  iVar2 = of_address_to_resource(uVar8,0,&local_b0);
  if (iVar2 != 0) {
    printk(&DAT_0010283d);
    iVar2 = -0x13;
    goto LAB_0010627c;
  }
  uVar7 = 0x68000000000f07;
  if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
     (uVar7 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
    uVar7 = 0x68000000000f07;
  }
  uVar5 = __ioremap(local_b0,(lStack_a8 - local_b0) + 1,uVar7);
  if (0xfffffffffffff000 < uVar5) {
    printk(&DAT_00103504);
    iVar2 = -0xe;
    goto LAB_0010627c;
  }
  *(ulong *)(lVar3 + 0x3d8) = uVar5;
  printk(&DAT_00102ede);
  dvfs_class = __class_create(&__this_module,&DAT_00102732,&apsys_dvfs_class_init___key);
  if (0xfffffffffffff000 < dvfs_class) {
    printk(&DAT_00101cc7);
  }
  *(ulong *)(lVar3 + 0x330) = dvfs_class;
  *(long *)(lVar3 + 0x60) = lVar1;
  *(undefined8 *)(lVar3 + 0x300) = *(undefined8 *)(param_1 + 0x310);
  dev_set_name(lVar3,"apsys");
  *(long *)(lVar3 + 0x98) = lVar3;
  iVar2 = device_register(lVar3);
  if (iVar2 != 0) {
    printk(&DAT_00102bd5);
  }
  iVar2 = sysfs_create_group(lVar3,&apsys_group);
  if (iVar2 != 0) {
    _dev_err(lVar1,"apsys create sysfs class failed, ret=%d\n",iVar2);
    goto LAB_0010627c;
  }
  *(long *)(param_1 + 0xa8) = lVar3;
  puVar4 = *(undefined8 **)(lVar3 + 0x3d0);
  if (puVar4 != (undefined8 *)0x0) {
    pcVar9 = (code *)*puVar4;
    if (pcVar9 != (code *)0x0) {
      if (4 < ((ulong)(pcVar9 + -0x117d24) >> 2 | (long)pcVar9 << 0x3e)) {
        __cfi_slowpath(0xc617a05b6541bdb6,pcVar9);
      }
      (*pcVar9)(lVar3,uVar8);
      puVar4 = *(undefined8 **)(lVar3 + 0x3d0);
      if (puVar4 == (undefined8 *)0x0) goto LAB_00106508;
    }
    pcVar9 = (code *)puVar4[1];
    if (pcVar9 != (code *)0x0) {
      if (9 < ((ulong)(pcVar9 + -0x117d88) >> 2 | (long)pcVar9 << 0x3e)) {
        __cfi_slowpath(0x7c6c8abd0b51b8c0,pcVar9);
      }
      (*pcVar9)(lVar3);
      puVar4 = *(undefined8 **)(lVar3 + 0x3d0);
      if (puVar4 == (undefined8 *)0x0) goto LAB_00106508;
    }
    pcVar9 = (code *)puVar4[7];
    if (pcVar9 != (code *)0x0) {
      if (9 < ((ulong)(pcVar9 + -0x117d88) >> 2 | (long)pcVar9 << 0x3e)) {
        __cfi_slowpath(0x7c6c8abd0b51b8c0,pcVar9);
      }
      (*pcVar9)(lVar3);
    }
  }
LAB_00106508:
  printk(&DAT_00103535);
  iVar2 = 0;
LAB_0010627c:
  if (___stack_chk_guard != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar2;
}


