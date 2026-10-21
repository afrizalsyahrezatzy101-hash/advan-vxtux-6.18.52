====================get_ip_status_show@0010ddac====================
// signature: undefined get_ip_status_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_ip_status_show(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  long lVar4;
  undefined8 *in_x18;
  long lVar5;
  long *plVar6;
  undefined8 unaff_x30;
  undefined8 local_d0;
  undefined8 local_c8;
  ulong local_c0;
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
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  *in_x18 = unaff_x30;
  local_48 = ___stack_chk_guard;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  local_c8 = 0;
  local_c0 = 0;
  local_d0 = 0;
  plVar6 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar6 == 0) {
    printk(&DAT_00105763);
    lVar5 = -0x16;
    goto LAB_0010df18;
  }
  lVar5 = param_1 + -0x30;
  mutex_lock(param_1 + -0x20);
  if ((plVar6[0xc] == 0) || (pcVar3 = *(code **)(plVar6[0xc] + 0x80), pcVar3 == (code *)0x0)) {
    printk(&DAT_00105b1c,"get_ip_status_show");
  }
  else {
    if (7 < ((ulong)(pcVar3 + -0x11779c) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x67ed40f9fdf2c861,pcVar3);
    }
    (*pcVar3)(lVar5,&local_c8);
  }
  if ((plVar6[0xc] == 0) || (pcVar3 = *(code **)(plVar6[0xc] + 0x130), pcVar3 == (code *)0x0)) {
    printk(&DAT_001047c6,"get_ip_status_show");
    lVar4 = plVar6[0xc];
    if (lVar4 != 0) goto LAB_0010de8c;
LAB_0010df80:
    printk(&DAT_001047c6,"get_ip_status_show");
  }
  else {
    if (0xd < ((ulong)(pcVar3 + -0x117744) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x2e12bb038fa0cb35,pcVar3);
    }
    (*pcVar3)(lVar5,(long)&local_d0 + 4);
    lVar4 = plVar6[0xc];
    if (lVar4 == 0) goto LAB_0010df80;
LAB_0010de8c:
    pcVar3 = *(code **)(lVar4 + 0x138);
    if (pcVar3 == (code *)0x0) goto LAB_0010df80;
    if (0xd < ((ulong)(pcVar3 + -0x117744) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x2e12bb038fa0cb35,pcVar3);
    }
    (*pcVar3)(lVar5,&local_d0);
  }
  iVar1 = sprintf(param_3,"module_dvfs_read_clk=%d\n",local_c0 >> 0x20);
  iVar2 = sprintf(param_3 + iVar1,"module_dvfs_read_volt=%d\n",local_c0 & 0xffffffff);
  lVar5 = (long)iVar1 + (long)iVar2;
  iVar1 = sprintf(param_3 + lVar5,"module_dvfs_read_top_volt=%d\n",local_d0 >> 0x20);
  lVar5 = lVar5 + iVar1;
  iVar1 = sprintf(param_3 + lVar5,"module_dvfs_read_mm_volt=%d\n",local_d0 & 0xffffffff);
  lVar5 = lVar5 + iVar1;
  mutex_unlock(param_1 + -0x20);
LAB_0010df18:
  if (___stack_chk_guard == local_48) {
    return lVar5;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


