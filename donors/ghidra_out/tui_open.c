// tui_open @ 00101340
// signature: undefined tui_open(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 tui_open(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = kmem_cache_alloc_trace(_DAT_001030c0,0xdc0,0x28);
  if (lVar1 == 0) {
    uVar2 = 0xfffffff4;
  }
  else {
    __init_waitqueue_head(lVar1 + 0x10,"&ts->tui_wq",&trusty_tui_driver);
    uVar2 = 0;
    *(long *)(param_2 + 200) = lVar1;
  }
  s_ts = lVar1;
  return uVar2;
}


