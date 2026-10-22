// mmc_get_wp_current_status_and_type @ 00108ce4
// signature: undefined mmc_get_wp_current_status_and_type(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void mmc_get_wp_current_status_and_type(long *param_1)

{
  ulong uVar1;
  ulong uVar2;
  uint *puVar3;
  undefined *puVar4;
  byte *pbVar5;
  long lVar6;
  int iVar7;
  undefined8 *in_x18;
  long lVar8;
  ulong uVar9;
  undefined8 unaff_x30;
  undefined8 local_180;
  undefined8 local_178;
  undefined8 local_170;
  undefined8 local_168;
  undefined8 local_160;
  undefined8 local_158;
  undefined8 local_150;
  undefined8 local_148;
  undefined8 local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined1 *local_128;
  undefined8 local_120;
  undefined8 local_118;
  undefined8 *local_110;
  undefined8 *local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_110 = &local_88;
  local_48 = ___stack_chk_guard;
  lVar8 = *param_1;
  local_108 = &local_160;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  local_78 = 0;
  local_70 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  local_c8 = 0;
  uStack_c0 = 0;
  local_d8 = 0;
  uStack_d0 = 0;
  local_e8 = 0;
  uStack_e0 = 0;
  local_f8 = 0;
  uStack_f0 = 0;
  local_120 = 0;
  local_118 = 0;
  local_130 = 0;
  local_128 = (undefined1 *)0x0;
  local_140 = 0;
  local_138 = 0;
  local_150 = 0;
  local_148 = 0;
  local_160 = 0;
  local_158 = 0;
  uStack_100 = 0;
  local_170 = 0;
  local_168 = 0;
  local_180 = 0;
  local_178 = 0;
  puVar3 = (uint *)kmem_cache_alloc_trace(_memcpy,0xdc0,4);
  if (puVar3 == (uint *)0x0) {
    lVar6 = *(long *)(lVar8 + 0x78);
    if (lVar6 == 0) {
      lVar6 = *(long *)(lVar8 + 8);
    }
    printk(&DAT_00100e04,lVar6);
LAB_00108e80:
    lVar8 = *param_1;
    local_110 = &local_88;
    local_118 = 0;
    local_108 = &local_160;
    local_58 = 0;
    uStack_50 = 0;
    local_68 = 0;
    uStack_60 = 0;
    local_78 = 0;
    local_70 = 0;
    local_88 = 0;
    uStack_80 = 0;
    local_98 = 0;
    uStack_90 = 0;
    local_a8 = 0;
    uStack_a0 = 0;
    local_b8 = 0;
    uStack_b0 = 0;
    local_c8 = 0;
    uStack_c0 = 0;
    local_d8 = 0;
    uStack_d0 = 0;
    local_e8 = 0;
    uStack_e0 = 0;
    local_f8 = 0;
    uStack_f0 = 0;
    local_128 = (undefined1 *)0x0;
    local_120 = 0;
    local_138 = 0;
    local_130 = 0;
    local_148 = 0;
    local_140 = 0;
    local_158 = 0;
    local_150 = 0;
    local_168 = 0;
    local_160 = 0;
    local_178 = 0;
    local_170 = 0;
    local_180 = 0;
    uStack_100 = 0;
    pbVar5 = (byte *)kmem_cache_alloc_trace(_memcpy,0xdc0,8);
    if (pbVar5 == (byte *)0x0) {
      lVar6 = *(long *)(lVar8 + 0x78);
      if (lVar6 == 0) {
        lVar6 = *(long *)(lVar8 + 8);
      }
      printk(&DAT_00101d8d,lVar6);
    }
    else {
      local_88 = CONCAT44(local_88._4_4_,0x1f);
      local_70 = CONCAT44(local_70._4_4_,0xb5);
      if (*(int *)(lVar8 + 0x3b0) != 0) goto LAB_001090e4;
      local_158 = 0x100000008;
      local_148 = CONCAT44(local_148._4_4_,0x200);
      local_150 = local_150 & 0xffffffff00000000;
      local_88 = CONCAT44((int)_DAT_00100660,0x1f);
      local_130 = CONCAT44(local_130._4_4_,1);
      local_128 = (undefined1 *)&local_180;
      sg_init_one(&local_180,pbVar5,8);
      mmc_wait_for_req(*param_1,&local_118);
      if ((int)local_68 == 0) {
        if (local_150._4_4_ == 0) {
          lVar6 = *(long *)(lVar8 + 0x78);
          uVar1 = (ulong)pbVar5[7] |
                  ((ulong)pbVar5[5] << 8 |
                   ((ulong)((uint)pbVar5[1] << 8 | (uint)*pbVar5 << 0x10 | (uint)pbVar5[2]) << 0x10
                    | (ulong)pbVar5[3] << 8 | (ulong)pbVar5[4]) << 0x10 | (ulong)pbVar5[6]) << 8;
          if (lVar6 == 0) {
            lVar6 = *(long *)(lVar8 + 8);
          }
          printk(&DAT_0010188f,lVar6,uVar1);
          if (*(int *)(lVar8 + 0x3b0) != 0) goto LAB_001090e4;
          if (DAT_00100654 == 0) {
            uVar9 = 0;
          }
          else {
            uVar9 = 0;
            iVar7 = DAT_00100654;
            do {
              uVar2 = uVar9 << 2;
              uVar9 = uVar9 << 2;
              if (DAT_00100650 != 0) {
                uVar9 = uVar2 | 2;
              }
              iVar7 = iVar7 + -1;
            } while (iVar7 != 0);
          }
          lVar6 = *(long *)(lVar8 + 0x78);
          if (lVar6 == 0) {
            lVar6 = *(long *)(lVar8 + 8);
          }
          printk(&DAT_00101c12,lVar6,uVar9);
          if (uVar1 != uVar9) {
            lVar6 = *(long *)(lVar8 + 0x78);
            if (lVar6 == 0) {
              lVar6 = *(long *)(lVar8 + 8);
            }
            printk(&DAT_001018f6,lVar6);
          }
        }
        else {
          lVar6 = *(long *)(lVar8 + 0x78);
          if (lVar6 == 0) {
            lVar6 = *(long *)(lVar8 + 8);
          }
          printk(&DAT_00101b97,lVar6);
        }
      }
      else {
        lVar6 = *(long *)(lVar8 + 0x78);
        if (lVar6 == 0) {
          lVar6 = *(long *)(lVar8 + 8);
        }
        printk(&DAT_00101dcc,lVar6);
      }
      kfree(pbVar5);
    }
    if (___stack_chk_guard == local_48) {
      return;
    }
  }
  else {
    local_88 = CONCAT44(local_88._4_4_,0x1e);
    local_70 = CONCAT44(local_70._4_4_,0xb5);
    if (*(int *)(lVar8 + 0x3b0) == 0) {
      local_158 = 0x100000004;
      local_148 = CONCAT44(local_148._4_4_,0x200);
      local_150 = local_150 & 0xffffffff00000000;
      local_88 = CONCAT44((int)_DAT_00100660,0x1e);
      local_130 = CONCAT44(local_130._4_4_,1);
      local_128 = (undefined1 *)&local_180;
      sg_init_one(&local_180,puVar3,4);
      mmc_wait_for_req(lVar8,&local_118);
      if ((int)local_68 == 0) {
        if (local_150._4_4_ != 0) {
          lVar6 = *(long *)(lVar8 + 0x78);
          if (lVar6 == 0) {
            lVar6 = *(long *)(lVar8 + 8);
          }
          puVar4 = &DAT_00101b97;
          goto LAB_00108e74;
        }
        lVar6 = *(long *)(lVar8 + 0x78);
        uVar1 = ((ulong)*puVar3 << 0x20 & 0xff00ff00ff00ff00) >> 8 |
                ((ulong)*puVar3 << 0x20 & 0xff00ff00ff00ff) << 8;
        if (lVar6 == 0) {
          lVar6 = *(long *)(lVar8 + 8);
        }
        printk(&DAT_001012b6,lVar6,((uVar1 & 0xffff0000ffff0000) >> 0x10 | uVar1 << 0x10) >> 0x20);
      }
      else {
        lVar6 = *(long *)(lVar8 + 0x78);
        if (lVar6 == 0) {
          lVar6 = *(long *)(lVar8 + 8);
        }
        puVar4 = &DAT_00101dcc;
LAB_00108e74:
        printk(puVar4,lVar6);
      }
      kfree(puVar3);
      goto LAB_00108e80;
    }
LAB_001090e4:
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


