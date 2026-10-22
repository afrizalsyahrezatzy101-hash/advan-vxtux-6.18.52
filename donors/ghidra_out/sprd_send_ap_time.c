// sprd_send_ap_time @ 00101030
// signature: undefined sprd_send_ap_time(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_send_ap_time(void)

{
  char *__s;
  char *__s_00;
  char *__s_01;
  char *__s_02;
  char *__s_03;
  char *__s_04;
  long *plVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  code *pcVar4;
  undefined8 uVar5;
  undefined8 *in_x18;
  long lVar6;
  undefined8 unaff_x30;
  char *local_98;
  char *pcStack_90;
  char *local_88;
  char *pcStack_80;
  char *local_78;
  char *pcStack_70;
  char *local_68;
  undefined8 uStack_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  uVar3 = _DAT_001040dc;
  uVar2 = _sys_tz;
  local_58 = ___stack_chk_guard;
  if ((tcd_dev == (long *)0x0) || (*tcd_dev == 0)) {
    printk(&DAT_00100297);
    uVar5 = 0xffffffff;
  }
  else {
    uVar5 = ktime_get_mono_fast_ns();
    *(undefined8 *)(*tcd_dev + 8) = uVar5;
    uVar5 = ktime_get_boot_fast_ns();
    *(undefined8 *)(*tcd_dev + 0x10) = uVar5;
    uVar5 = ktime_get_real_seconds();
    *(undefined8 *)(*tcd_dev + 0x18) = uVar5;
    pcVar4 = _arch_timer_read_counter;
    __cfi_slowpath(0x19e9081ee75ef79b,_arch_timer_read_counter);
    uVar5 = (*pcVar4)();
    *(undefined8 *)(*tcd_dev + 0x20) = uVar5;
    uVar5 = sprd_systimer_read();
    plVar1 = tcd_dev;
    *(undefined8 *)(*tcd_dev + 0x28) = uVar5;
    *(undefined4 *)(*plVar1 + 0x30) = uVar2;
    *(undefined4 *)(*plVar1 + 0x34) = uVar3;
    lVar6 = plVar1[0x13];
    local_68 = (char *)0x0;
    uStack_60 = 0;
    local_78 = (char *)0x0;
    pcStack_70 = (char *)0x0;
    local_88 = (char *)0x0;
    pcStack_80 = (char *)0x0;
    local_98 = (char *)0x0;
    pcStack_90 = (char *)0x0;
    local_98 = (char *)kmem_cache_alloc_trace(_DAT_00104138,0xdc0,800);
    if ((((local_98 != (char *)0x0) && (tcd_dev != (long *)0x0)) && (lVar6 != 0)) && (*tcd_dev != 0)
       ) {
      __s = local_98 + 100;
      __s_00 = local_98 + 200;
      __s_01 = local_98 + 300;
      __s_02 = local_98 + 400;
      __s_03 = local_98 + 500;
      __s_04 = local_98 + 600;
      uStack_60 = 0;
      pcStack_90 = __s;
      local_88 = __s_00;
      pcStack_80 = __s_01;
      local_78 = __s_02;
      pcStack_70 = __s_03;
      local_68 = __s_04;
      snprintf(local_98,100,"monotime_ns=%lld",*(undefined8 *)(*tcd_dev + 8));
      snprintf(__s,100,"boottime_ns=%lld",*(undefined8 *)(*tcd_dev + 0x10));
      snprintf(__s_00,100,"realtime_s=%lld",*(undefined8 *)(*tcd_dev + 0x18));
      snprintf(__s_01,100,"ts_cnt=%lld",*(undefined8 *)(*tcd_dev + 0x20));
      snprintf(__s_02,100,"sysfrt_cnt=%lld",*(undefined8 *)(*tcd_dev + 0x28));
      snprintf(__s_03,100,"minuteswest=%d",(ulong)*(uint *)(*tcd_dev + 0x30));
      snprintf(__s_04,100,"dsttime=%d",(ulong)*(uint *)(*tcd_dev + 0x34));
      kobject_uevent_env(lVar6,2,&local_98);
      _dev_info(tcd_dev[0x13],"send %s %s %s %s %s %s %s to userspace\n",local_98,pcStack_90,
                local_88,pcStack_80,local_78,pcStack_70,local_68);
      kfree(local_98);
    }
    uVar5 = 0;
  }
  if (___stack_chk_guard == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}


