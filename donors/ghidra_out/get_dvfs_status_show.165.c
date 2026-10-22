// get_dvfs_status_show.165 @ 0010a48c
// signature: undefined get_dvfs_status_show.165(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void get_dvfs_status_show_165(long param_1,undefined8 param_2,void *param_3)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_78;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_48;
  undefined8 local_38;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  lVar4 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  memset(&local_98,0,0x70);
  lVar2 = *(long *)(lVar4 + 0x480);
  if ((lVar2 == 0) || (pcVar3 = *(code **)(lVar2 + 0x80), pcVar3 == (code *)0x0)) {
    memcpy(param_3,"undefined\n",0xb);
    lVar2 = 10;
  }
  else {
    if (1 < ((ulong)(pcVar3 + -0x117fc0) >> 2 | (long)pcVar3 << 0x3e)) {
      __cfi_slowpath(0x5b6c89d1ccfd4a41,pcVar3);
    }
    (*pcVar3)(lVar4,&local_98);
    memcpy(param_3,"apsys_cur_volt\tvsp_vote_volt\tdpu_vote_volt\tvdsp_vote_volt\n",0x3b);
    iVar1 = sprintf((char *)((long)param_3 + 0x3a),"%s\t\t%s\t\t%s\t\t%s\n",local_98,local_78,
                    uStack_90,local_68);
    memcpy((void *)((long)param_3 + (long)iVar1 + 0x3a),
           "\t\tvsp_cur_freq\tdpu_cur_freq\tvdsp_cur_freq\n",0x2b);
    lVar2 = (long)iVar1 + 100;
    iVar1 = sprintf((char *)((long)param_3 + lVar2),"\t\t%s\t\t%s\t\t%s\n",local_48,local_60,
                    local_38);
    lVar2 = lVar2 + iVar1;
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar2);
}


