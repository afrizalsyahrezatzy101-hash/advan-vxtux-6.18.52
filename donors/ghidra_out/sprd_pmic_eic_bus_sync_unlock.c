// sprd_pmic_eic_bus_sync_unlock @ 00101814
// signature: undefined sprd_pmic_eic_bus_sync_unlock(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_pmic_eic_bus_sync_unlock(long param_1)

{
  ulong uVar1;
  byte bVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  undefined8 *in_x18;
  undefined8 uVar10;
  uint uVar11;
  ulong uVar12;
  undefined8 unaff_x30;
  uint local_4c;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  uVar10 = *(undefined8 *)(param_1 + 0x30);
  lVar4 = gpiochip_get_data(uVar10);
  uVar1 = *(ulong *)(param_1 + 8);
  uVar12 = uVar1 & 0xffffffff;
  uVar11 = (uint)uVar1;
  if ((**(byte **)(param_1 + 0x10) & 3) == 0) {
    if (0xf < uVar12) goto LAB_001019e4;
    bVar2 = *(byte *)(lVar4 + uVar12 * 3 + 0x31c);
    lVar5 = gpiochip_get_data(uVar10);
    uVar6 = *(undefined8 *)(lVar5 + 0x310);
    iVar8 = 1 << (ulong)(uVar11 & 0xf);
    iVar3 = *(int *)(lVar5 + 0x318) + 0x14;
    iVar9 = (uint)bVar2 << (ulong)(uVar11 & 0xf);
  }
  else {
    lVar5 = gpiochip_get_data(uVar10);
    local_4c = 0;
    iVar3 = regmap_read(*(undefined8 *)(lVar5 + 0x310),*(undefined4 *)(lVar5 + 0x318),&local_4c);
    if ((iVar3 == 0) && ((local_4c >> (uVar1 & 0xf) & 1) == 0)) {
      lVar5 = gpiochip_get_data(uVar10);
      uVar6 = *(undefined8 *)(lVar5 + 0x310);
      iVar8 = 1 << (ulong)(uVar11 & 0xf);
      iVar3 = *(int *)(lVar5 + 0x318) + 0x14;
      iVar9 = iVar8;
    }
    else {
      lVar5 = gpiochip_get_data(uVar10);
      uVar6 = *(undefined8 *)(lVar5 + 0x310);
      iVar3 = *(int *)(lVar5 + 0x318) + 0x14;
      iVar8 = 1 << (ulong)(uVar11 & 0xf);
      iVar9 = 0;
    }
  }
  regmap_update_bits_base(uVar6,iVar3,iVar8,iVar9,0,0,0);
  if (uVar12 < 0x10) {
    lVar5 = lVar4 + uVar12 * 3;
    bVar2 = *(byte *)(lVar5 + 0x31d);
    lVar7 = gpiochip_get_data(uVar10);
    uVar11 = uVar11 & 0xf;
    regmap_update_bits_base
              (*(undefined8 *)(lVar7 + 0x310),*(int *)(lVar7 + 0x318) + 0x18,1 << (ulong)uVar11,
               (uint)bVar2 << (ulong)uVar11,0,0,0);
    bVar2 = *(byte *)(lVar5 + 0x31e);
    lVar5 = gpiochip_get_data(uVar10);
    regmap_update_bits_base
              (*(undefined8 *)(lVar5 + 0x310),*(int *)(lVar5 + 0x318) + 0x28,1 << (ulong)uVar11,
               (uint)bVar2 << (ulong)uVar11,0,0,0);
    mutex_unlock(lVar4 + 0x350);
    if (___stack_chk_guard == local_48) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
LAB_001019e4:
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}


