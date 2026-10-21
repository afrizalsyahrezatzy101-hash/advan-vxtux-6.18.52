// sprd_pmic_eic_irq_handler @ 00101244
// signature: undefined sprd_pmic_eic_irq_handler(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 sprd_pmic_eic_irq_handler(undefined8 param_1,long param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  long lVar9;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined4 local_6c;
  ulong local_68;
  uint local_5c;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  local_68 = 0;
  local_6c = 0;
  iVar4 = regmap_read(*(undefined8 *)(param_2 + 0x310),*(int *)(param_2 + 0x318) + 0x20,&local_6c);
  if (iVar4 == 0) {
    local_68 = (ulong)(ushort)local_6c;
    uVar5 = find_next_bit(&local_68,*(undefined2 *)(param_2 + 0x8c),0);
    if (uVar5 < *(ushort *)(param_2 + 0x8c)) {
      do {
        lVar9 = gpiochip_get_data(param_2);
        uVar1 = uVar5 & 0xf;
        iVar4 = 1 << uVar1;
        regmap_update_bits_base
                  (*(undefined8 *)(lVar9 + 0x310),*(int *)(lVar9 + 0x318) + 0x24,iVar4,iVar4,0,0,0);
        uVar6 = irq_find_mapping(*(undefined8 *)(param_2 + 0xa8),uVar5);
        handle_nested_irq();
        lVar9 = irq_get_irq_data(uVar6);
        if ((lVar9 != 0) && ((**(byte **)(lVar9 + 0x10) & 3) != 0)) {
          lVar9 = gpiochip_get_data(param_2);
          local_5c = 0;
          uVar7 = regmap_read(*(undefined8 *)(lVar9 + 0x310),*(undefined4 *)(lVar9 + 0x318),
                              &local_5c);
          if (uVar7 == 0) {
            uVar7 = (uint)((1L << uVar1 & (ulong)local_5c) != 0);
          }
          while( true ) {
            lVar9 = gpiochip_get_data(param_2);
            iVar2 = iVar4;
            if (uVar7 != 0) {
              iVar2 = 0;
            }
            regmap_update_bits_base
                      (*(undefined8 *)(lVar9 + 0x310),*(int *)(lVar9 + 0x318) + 0x14,iVar4,iVar2,0,0
                       ,0);
            lVar9 = gpiochip_get_data(param_2);
            local_5c = 0;
            uVar8 = regmap_read(*(undefined8 *)(lVar9 + 0x310),*(undefined4 *)(lVar9 + 0x318),
                                &local_5c);
            uVar3 = (uint)((1L << uVar1 & (ulong)local_5c) != 0);
            if (uVar8 != 0) {
              uVar3 = uVar8;
            }
            if (uVar7 == uVar3) break;
            _dev_warn(*(undefined8 *)(param_2 + 0x10),"PMIC EIC level was changed.\n");
            uVar7 = uVar3;
          }
          lVar9 = gpiochip_get_data(param_2);
          regmap_update_bits_base
                    (*(undefined8 *)(lVar9 + 0x310),*(int *)(lVar9 + 0x318) + 0x18,iVar4,iVar4,0,0,0
                    );
          lVar9 = gpiochip_get_data(param_2);
          regmap_update_bits_base
                    (*(undefined8 *)(lVar9 + 0x310),*(int *)(lVar9 + 0x318) + 0x28,iVar4,iVar4,0,0,0
                    );
        }
        uVar5 = find_next_bit(&local_68,*(undefined2 *)(param_2 + 0x8c),uVar5 + 1);
      } while (uVar5 < *(ushort *)(param_2 + 0x8c));
    }
  }
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 1;
}


