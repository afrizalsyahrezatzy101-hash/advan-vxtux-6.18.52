// sprd_pcm_platform_proc_write @ 00104e7c
// signature: undefined sprd_pcm_platform_proc_write(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_pcm_platform_proc_write(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined4 local_134;
  undefined3 local_130;
  undefined2 uStack_12d;
  undefined3 uStack_12b;
  undefined3 uStack_128;
  undefined2 uStack_125;
  undefined3 uStack_123;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  long local_70;
  
  *in_x18 = unaff_x30;
  local_70 = ___stack_chk_guard;
  local_80 = 0;
  uStack_78 = 0;
  local_90 = 0;
  uStack_88 = 0;
  local_a0 = 0;
  uStack_98 = 0;
  local_b0 = 0;
  uStack_a8 = 0;
  local_c0 = 0;
  uStack_b8 = 0;
  local_d0 = 0;
  uStack_c8 = 0;
  local_e0 = 0;
  uStack_d8 = 0;
  local_f0 = 0;
  uStack_e8 = 0;
  local_100 = 0;
  uStack_f8 = 0;
  local_110 = 0;
  uStack_108 = 0;
  local_120 = 0;
  uStack_118 = 0;
  local_130 = 0;
  uStack_12d = 0;
  uStack_12b = 0;
  uStack_128 = 0;
  uStack_125 = 0;
  uStack_123 = 0;
  local_134 = 0;
  iVar1 = snd_info_get_line(param_2,&local_b0,0x40);
  while (iVar1 == 0) {
    iVar1 = sscanf((char *)&local_b0,"%s %x",&local_130,&local_134);
    if (iVar1 == 2) {
      if (CONCAT35(uStack_12b,CONCAT23(uStack_12d,local_130)) == 0x69735f7264645f74 &&
          CONCAT35(uStack_128,CONCAT32(uStack_12b,uStack_12d)) == 0x657a69735f7264) {
        test_ddr_buffer_size = local_134;
        printk(&DAT_001007a1);
      }
      if (CONCAT35(uStack_12b,CONCAT23(uStack_12d,local_130)) == 0x72695f6573755f74 &&
          CONCAT35(uStack_128,CONCAT32(uStack_12b,uStack_12d)) == 0x6d6172695f6573) {
        test_normal_cap_use_iram = local_134;
        printk(&DAT_0010125c);
      }
      if (CONCAT35(uStack_12b,CONCAT23(uStack_12d,local_130)) == 0x5f6563726f665f74 &&
          CONCAT26(uStack_125,CONCAT33(uStack_128,uStack_12b)) == 0x706f74735f6563) {
        test_force_stop = local_134;
        printk(&DAT_00100375);
      }
      if ((CONCAT35(uStack_12b,CONCAT23(uStack_12d,local_130)) == 0x6f705f6775626564 &&
          CONCAT35(uStack_123,CONCAT23(uStack_125,uStack_128)) == 0x6f6c5f7265746e69) &&
          (short)local_120 == 0x67) {
        init_pm_dma___key_3 = local_134;
        printk(&DAT_001007cb);
      }
    }
    iVar1 = snd_info_get_line(param_2,&local_b0,0x40);
  }
  if (___stack_chk_guard == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


