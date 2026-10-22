// mmc_send_tuning_read @ 00109990
// signature: undefined mmc_send_tuning_read(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int mmc_send_tuning_read(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  int iVar2;
  undefined8 unaff_x30;
  undefined8 local_170;
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 local_150;
  undefined8 local_148;
  undefined8 local_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 local_120;
  undefined1 *local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 *local_100;
  undefined8 *local_f8;
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
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
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
  local_f8 = (undefined8 *)0x0;
  uStack_f0 = 0;
  local_108 = 0;
  local_100 = (undefined8 *)0x0;
  local_48 = 0;
  uStack_40 = 0;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  local_60 = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_118 = (undefined1 *)0x0;
  uStack_110 = 0;
  local_128 = 0;
  local_120 = 0;
  local_138 = 0;
  uStack_130 = 0;
  local_148 = 0;
  local_140 = 0;
  local_158 = 0;
  local_150 = 0;
  local_168 = 0;
  uStack_160 = 0;
  local_170 = 0;
  lVar1 = kmem_cache_alloc_trace(_DAT_00112410,0xdc0,0x200);
  if (lVar1 == 0) {
    lVar1 = *(long *)(param_1 + 0x78);
    if (lVar1 == 0) {
      lVar1 = *(long *)(param_1 + 8);
    }
    printk(&DAT_001019b1,lVar1);
    iVar2 = -0xc;
  }
  else {
    local_100 = &local_78;
    local_f8 = &local_150;
    local_150 = 100000000;
    local_148 = 0x100000200;
    local_138 = CONCAT44(local_138._4_4_,0x200);
    local_140 = local_140 & 0xffffffff00000000;
    local_78 = CONCAT44(local_78._4_4_,0x11);
    local_60 = CONCAT44(local_60._4_4_,0x3d);
    local_120 = CONCAT44(local_120._4_4_,1);
    local_118 = (undefined1 *)&local_170;
    sg_init_one(&local_170,lVar1,0x200);
    mmc_wait_for_req(param_1,&local_108);
    iVar2 = local_140._4_4_;
    if ((int)local_58 != 0) {
      iVar2 = (int)local_58;
    }
    kfree(lVar1);
  }
  if (___stack_chk_guard == local_38) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


