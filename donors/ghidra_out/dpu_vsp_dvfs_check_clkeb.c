// dpu_vsp_dvfs_check_clkeb @ 0010eea8
// signature: undefined dpu_vsp_dvfs_check_clkeb(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int dpu_vsp_dvfs_check_clkeb(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_1c;
  long local_18;
  
  *in_x18 = unaff_x30;
  local_18 = ___stack_chk_guard;
  local_1c = 0;
  regmap_read(regmap_aon_base,0,&local_1c);
  if (___stack_chk_guard == local_18) {
    return (local_1c >> 0x15 & 1) - 1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


