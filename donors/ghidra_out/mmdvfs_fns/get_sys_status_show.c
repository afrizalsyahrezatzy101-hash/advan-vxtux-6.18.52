====================get_sys_status_show@0010acb8====================
// signature: undefined get_sys_status_show(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_sys_status_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  int iVar10;
  code *pcVar11;
  undefined8 *in_x18;
  long lVar12;
  undefined8 unaff_x30;
  undefined8 uVar13;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  ulong local_58;
  ulong local_50;
  ulong local_48;
  ulong uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_48 = 0;
  uStack_40 = 0;
  local_58 = 0;
  local_50 = 0;
  local_68 = 0;
  local_60 = 0;
  local_78 = 0;
  local_70 = 0;
  local_88 = 0;
  local_80 = 0;
  local_98 = 0;
  local_90 = 0;
  local_a8 = 0;
  local_a0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  lVar12 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  if (((lVar12 != 0) && (lVar12 = *(long *)(lVar12 + 0x20), lVar12 != 0)) &&
     (*(long *)(lVar12 + 0x128) != 0)) {
    pcVar11 = *(code **)(lVar12 + 0x80);
    if (7 < ((ulong)(pcVar11 + -0x11779c) >> 2 | (long)pcVar11 << 0x3e)) {
      __cfi_slowpath(0x67ed40f9fdf2c861,pcVar11);
    }
    iVar10 = (*pcVar11)(param_1 + -0x30,&local_b8);
    if (0 < iVar10) {
      param_3[0x58] = '\n';
      uVar9 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._80_8_;
      uVar8 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._72_8_;
      uVar7 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._64_8_;
      uVar13 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._56_8_;
      uVar6 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._48_8_;
      uVar5 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._40_8_;
      uVar4 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._32_8_;
      uVar3 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._24_8_;
      uVar2 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._16_8_;
      uVar1 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._8_8_;
      param_3[0x59] = '\0';
      *(undefined8 *)param_3 = s_ip_volt_V___mtx_vote_isp_vote_fd_00104f38._0_8_;
      *(undefined8 *)(param_3 + 8) = uVar1;
      *(undefined8 *)(param_3 + 0x20) = uVar4;
      *(undefined8 *)(param_3 + 0x28) = uVar5;
      *(undefined8 *)(param_3 + 0x10) = uVar2;
      *(undefined8 *)(param_3 + 0x18) = uVar3;
      *(undefined8 *)(param_3 + 0x48) = uVar8;
      *(undefined8 *)(param_3 + 0x50) = uVar9;
      *(undefined8 *)(param_3 + 0x38) = uVar13;
      *(undefined8 *)(param_3 + 0x40) = uVar7;
      *(undefined8 *)(param_3 + 0x30) = uVar6;
      uVar13 = local_90;
      iVar10 = sprintf(param_3 + 0x59,"%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
                       local_70,local_a0,local_88,local_80,local_78,local_98,local_90,local_68,
                       local_60);
      uVar6 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._40_8_;
      uVar5 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._32_8_;
      uVar4 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._24_8_;
      uVar3 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._16_8_;
      uVar2 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._8_8_;
      uVar1 = s_ip_clock_Hz___mtx_isp_fd_cpp_jpg_00104d81._0_8_;
      lVar12 = (long)iVar10;
      param_3[lVar12 + 0x89] = '\0';
      *(undefined8 *)(param_3 + lVar12 + 0x59) = uVar1;
      *(undefined8 *)(param_3 + lVar12 + 0x61) = uVar2;
      *(undefined8 *)(param_3 + lVar12 + 0x69) = uVar3;
      *(undefined8 *)(param_3 + lVar12 + 0x81) = uVar6;
      *(undefined8 *)(param_3 + lVar12 + 0x79) = uVar5;
      *(undefined8 *)(param_3 + lVar12 + 0x71) = uVar4;
      iVar10 = sprintf(param_3 + lVar12 + 0x89,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",uStack_40 & 0xffffffff
                       ,local_58 & 0xffffffff,local_50 >> 0x20,local_48 & 0xffffffff,
                       local_48 >> 0x20,local_58 >> 0x20,
                       CONCAT44((int)((ulong)uVar13 >> 0x20),(undefined4)local_50));
      lVar12 = lVar12 + 0x89 + (long)iVar10;
      goto LAB_0010ae68;
    }
  }
  uVar6 = s_Error_Get_ip_status_at_wrong_tim_001060f5._56_8_;
  uVar5 = s_Error_Get_ip_status_at_wrong_tim_001060f5._48_8_;
  uVar3 = s_Error_Get_ip_status_at_wrong_tim_001060f5._24_8_;
  uVar2 = s_Error_Get_ip_status_at_wrong_tim_001060f5._8_8_;
  uVar1 = s_Error_Get_ip_status_at_wrong_tim_001060f5._0_8_;
  *(undefined8 *)(param_3 + 0x10) = s_Error_Get_ip_status_at_wrong_tim_001060f5._16_8_;
  *(undefined8 *)(param_3 + 0x18) = uVar3;
  uVar13 = s_Error_Get_ip_status_at_wrong_tim_001060f5._64_8_;
  uVar4 = s_Error_Get_ip_status_at_wrong_tim_001060f5._40_8_;
  uVar3 = s_Error_Get_ip_status_at_wrong_tim_001060f5._32_8_;
  *(undefined8 *)param_3 = uVar1;
  *(undefined8 *)(param_3 + 8) = uVar2;
  *(undefined8 *)(param_3 + 0x38) = uVar6;
  *(undefined8 *)(param_3 + 0x40) = uVar13;
  *(undefined8 *)(param_3 + 0x28) = uVar4;
  *(undefined8 *)(param_3 + 0x30) = uVar5;
  *(undefined8 *)(param_3 + 0x20) = uVar3;
  lVar12 = 0x47;
LAB_0010ae68:
  mutex_unlock(param_1 + -0x20);
  if (___stack_chk_guard == local_38) {
    return lVar12;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


