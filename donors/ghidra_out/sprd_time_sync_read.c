// sprd_time_sync_read @ 00101574
// signature: undefined sprd_time_sync_read(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_time_sync_read(undefined8 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  code *pcVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  uVar2 = _DAT_001040dc;
  uVar1 = _sys_tz;
  local_38 = ___stack_chk_guard;
  local_70 = 0;
  local_68 = 0;
  local_60 = 0;
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  local_40 = 0;
  if ((tcd_dev == (long *)0x0) || (*tcd_dev == 0)) {
    lVar4 = 0;
  }
  else {
    local_68 = ktime_get_mono_fast_ns();
    local_60 = ktime_get_boot_fast_ns();
    local_58 = ktime_get_real_seconds();
    pcVar3 = _arch_timer_read_counter;
    __cfi_slowpath(0x19e9081ee75ef79b,_arch_timer_read_counter);
    local_50 = (*pcVar3)();
    local_48 = sprd_systimer_read();
    local_40 = CONCAT44(uVar2,uVar1);
    lVar4 = _copy_to_user(param_2,&local_70);
    lVar4 = -(ulong)(lVar4 != 0);
  }
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar4);
}


