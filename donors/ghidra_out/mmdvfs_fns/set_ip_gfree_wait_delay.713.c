====================set_ip_gfree_wait_delay.713@00110590====================
// signature: undefined set_ip_gfree_wait_delay.713(void)

void set_ip_gfree_wait_delay_713(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = (param_1 & 0x3ff) << 0x14;
  return;
}


