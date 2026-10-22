// trusty_irq_create_irq_mapping @ 00101cb4
// signature: undefined trusty_irq_create_irq_mapping(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void trusty_irq_create_irq_mapping(long *param_1,uint param_2)

{
  long lVar1;
  uint uVar2;
  code *pcVar3;
  uint uVar4;
  long lVar5;
  ulong uVar6;
  undefined8 *in_x18;
  int iVar7;
  undefined8 unaff_x30;
  uint local_94;
  undefined8 local_90;
  long local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_90 = 0;
  local_88 = 0;
  local_94 = 0;
  local_48 = 0;
  uStack_40 = 0;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_80 = 0;
  lVar5 = of_find_property(*(undefined8 *)(*param_1 + 0x300),"interrupt-ranges",0);
  if (lVar5 != 0) {
    uVar6 = of_property_read_u32_index
                      (*(undefined8 *)(*param_1 + 0x300),"interrupt-ranges",0,&local_90);
    if ((int)uVar6 == 0) {
      iVar7 = 3;
      while( true ) {
        uVar6 = of_property_read_u32_index
                          (*(undefined8 *)(*param_1 + 0x300),"interrupt-ranges",iVar7 + -2,&local_94
                          );
        if ((int)uVar6 != 0) break;
        if (((uint)local_90 <= param_2) && (param_2 <= local_94)) {
          uVar6 = of_property_read_u32_index
                            (*(undefined8 *)(*param_1 + 0x300),"interrupt-ranges",iVar7 + -1,
                             (long)&local_90 + 4);
          if (((int)uVar6 == 0) &&
             (uVar6 = of_parse_phandle_with_args
                                (*(undefined8 *)(*param_1 + 0x300),"interrupt-templates",
                                 "#interrupt-cells",local_90._4_4_,&local_88), (int)uVar6 == 0)) {
            if (local_88 == 0) {
                    /* WARNING: Does not return */
              pcVar3 = (code *)SoftwareBreakpoint(0x800,0x101e94);
              (*pcVar3)();
            }
            if ((uint)local_80 == 0) {
                    /* WARNING: Does not return */
              pcVar3 = (code *)SoftwareBreakpoint(0x800,0x101ea0);
              (*pcVar3)();
            }
            if (local_90._4_4_ == 0) {
              uVar6 = 0;
              local_80 = 1;
              goto LAB_00101eb4;
            }
            uVar6 = (ulong)local_80._4_4_;
            if (local_80._4_4_ < (uint)local_80) {
              if (local_80._4_4_ == 0) goto LAB_00101eb4;
              lVar5 = 0;
              goto LAB_00101e18;
            }
            _dev_err(*param_1,"irq pos is out of range: %d\n");
            uVar6 = 0xffffffea;
          }
          break;
        }
        uVar6 = of_property_read_u32_index
                          (*(undefined8 *)(*param_1 + 0x300),"interrupt-ranges",iVar7,&local_90);
        iVar7 = iVar7 + 3;
        if ((int)uVar6 != 0) break;
      }
    }
    goto LAB_00101e4c;
  }
  goto LAB_00101e48;
  while( true ) {
    lVar1 = lVar5 + 4;
    *(undefined4 *)((long)&local_80 + lVar5 + 4) = *(undefined4 *)((long)&local_78 + lVar5);
    lVar5 = lVar1;
    if (uVar6 * 4 - lVar1 == 0) break;
LAB_00101e18:
    if (lVar5 == 0x3c) goto LAB_00101e40;
  }
  if (local_80._4_4_ < 0x10) {
LAB_00101eb4:
    *(uint *)((long)&local_80 + uVar6 * 4 + 4) = param_2 - (uint)local_90;
    uVar4 = irq_create_of_mapping(&local_88);
    uVar2 = 0xffffffea;
    if (uVar4 != 0) {
      uVar2 = uVar4;
    }
    uVar6 = (ulong)uVar2;
    goto LAB_00101e4c;
  }
LAB_00101e40:
  __ubsan_handle_cfi_check_fail_abort(0x12);
LAB_00101e48:
  uVar6 = (ulong)param_2;
LAB_00101e4c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}


