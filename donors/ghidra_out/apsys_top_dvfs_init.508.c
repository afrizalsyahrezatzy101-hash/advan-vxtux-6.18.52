// apsys_top_dvfs_init.508 @ 0010f978
// signature: undefined apsys_top_dvfs_init.508(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void apsys_top_dvfs_init_508(long param_1)

{
  ulong uVar1;
  undefined8 *in_x18;
  undefined8 uVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = 0x68000000000f07;
  printk(&DAT_001034f0,"apsys_top_dvfs_init");
  if ((((uint)_cpu_hwcaps >> 0x17 & 1) == 0) &&
     (uVar2 = 0x68000000000707, _kimage_vaddr != -0x3ff0000000)) {
    uVar2 = 0x68000000000f07;
  }
  uVar1 = __ioremap(0x322a0000,0x1cc,uVar2);
  if (0xfffffffffffff000 < uVar1) {
    printk(&DAT_00102494);
  }
  *(ulong *)(param_1 + 0x3e0) = uVar1;
  return;
}


