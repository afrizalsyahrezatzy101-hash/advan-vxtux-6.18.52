// get_dvfs_status_show.93 @ 00109180
// signature: undefined get_dvfs_status_show.93(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void get_dvfs_status_show_93(long param_1,undefined8 param_2,void *param_3)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  code *pcVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  memset(&local_98,0,0x70);
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_0010291a);
    lVar5 = -0x16;
  }
  else {
    lVar3 = *(long *)(lVar5 + 1000);
    if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x40), pcVar4 == (code *)0x0)) {
      memcpy(param_3,"undefined\n",0xb);
      lVar5 = 10;
    }
    else {
      if (5 < ((ulong)(pcVar4 + -0x117df4) >> 2 | (long)pcVar4 << 0x3e)) {
        __cfi_slowpath(0xffe6b08aaf98193a,pcVar4);
      }
      (*pcVar4)(lVar5,&local_98);
      memcpy(param_3,
             "apsys_cur_volt\tvsp_vote_volt\tvpuenc_vote_volt\tdpu_vote_volt\tvdsp_vote_volt\n",0x4c
            );
      iVar1 = sprintf((char *)((long)param_3 + 0x4b),"%s\t\t%s\t\t%s\t\t%s\t\t%s\n",local_98,
                      local_78,uStack_70,uStack_90,local_68);
      memcpy((void *)((long)param_3 + (long)iVar1 + 0x4b),
             "\t\tvsp_cur_freq\tvpuenc_cur_freq\tdpu_cur_freq\tvdsp_cur_freq\n",0x3b);
      lVar5 = (long)iVar1 + 0x85;
      iVar1 = sprintf((char *)((long)param_3 + lVar5),"\t\t%s\t\t%s\t\t%s\t\t%s\n",local_48,
                      uStack_40,local_60,local_38);
      lVar5 = lVar5 + iVar1;
    }
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar5);
}


