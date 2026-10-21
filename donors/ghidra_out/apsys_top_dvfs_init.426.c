// apsys_top_dvfs_init.426 @ 0010f188
// signature: undefined apsys_top_dvfs_init.426(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void apsys_top_dvfs_init_426(long param_1)

{
  ulong uVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_34;
  undefined8 local_30;
  long local_28;
  
  *in_x18 = unaff_x30;
  uVar3 = 0x68000000000f07;
  local_28 = ___stack_chk_guard;
  local_30 = 0;
  local_34 = 0;
  if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
     (uVar3 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
    uVar3 = 0x68000000000f07;
  }
  uVar1 = __ioremap(0x64940000,0x600,uVar3);
  if (0xfffffffffffff000 < uVar1) {
    printk(&DAT_00101b2f);
  }
  *(ulong *)(param_1 + 0x3e0) = uVar1;
  local_34 = *(uint *)(uVar1 + 0xd84) & 0xfffffffe;
  *(uint *)(uVar1 + 0xd84) = local_34;
  lVar2 = of_find_matching_node_and_match(0,&sprd_dvfs_of_match,&local_30);
  if (lVar2 == 0) {
    printk(&DAT_00102f70);
  }
  else {
    lVar2 = of_find_device_by_node();
    if (lVar2 == 0) {
      printk(&DAT_0010332a);
    }
    else {
      uVar3 = dev_get_regmap(*(undefined8 *)(lVar2 + 0x70),0);
      regmap_read(uVar3,0xa158,&local_34);
      local_34 = local_34 | 4;
      regmap_write(uVar3,0xa158);
      *(undefined4 *)(uVar1 + 0x3d0) = 0xd000d0;
      *(undefined4 *)(uVar1 + 0x39c) = 0;
    }
  }
  if (___stack_chk_guard != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


