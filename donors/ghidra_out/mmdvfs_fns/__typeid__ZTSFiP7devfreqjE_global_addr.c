====================__typeid__ZTSFiP7devfreqjE_global_addr@001177fc====================
// signature: undefined __typeid__ZTSFiP7devfreqjE_global_addr(void)

undefined8 __typeid__ZTSFiP7devfreqjE_global_addr(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffffbf;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x40;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}


