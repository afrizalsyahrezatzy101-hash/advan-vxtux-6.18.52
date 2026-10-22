====================set_ip_gfree_wait_delay.920@001136f8====================
// signature: undefined set_ip_gfree_wait_delay.920(void)

void set_ip_gfree_wait_delay_920(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x214) = param_1 & 0x3ff;
  return;
}


