// sprd_mbox_phy_outbox_isr @ 00103b34
// signature: undefined sprd_mbox_phy_outbox_isr(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 sprd_mbox_phy_outbox_isr(int param_1,long param_2)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  code *pcVar4;
  int iVar5;
  int iVar6;
  ulong uVar7;
  undefined8 *in_x18;
  int *piVar8;
  undefined8 unaff_x30;
  undefined8 local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  local_70 = 0;
  lVar1 = 200;
  if (*(int *)(param_2 + 0xd0) != param_1) {
    lVar1 = 0x98;
  }
  piVar8 = *(int **)(param_2 + lVar1);
  uVar2 = piVar8[5];
  DataMemoryBarrier(0,1);
  uVar3 = uVar2 >> 0x10 & 0xff;
  iVar5 = uVar3 - (uVar2 >> 0x18);
  if (iVar5 == 0) {
    if ((uVar2 >> 2 & 1) != 0) {
      iVar5 = *(int *)(param_2 + 0xb0);
      goto LAB_00103bc8;
    }
  }
  else {
    if (uVar3 < uVar2 >> 0x18) {
      iVar5 = iVar5 + *(int *)(param_2 + 0xb0);
    }
LAB_00103bc8:
    if (iVar5 != 0) {
      do {
        uVar2 = mbox_rx_fifo_cnt;
        DataMemoryBarrier(0,1);
        DataMemoryBarrier(0,1);
        local_70 = CONCAT44(piVar8[2],piVar8[1]);
        if (*(int *)(param_2 + 0xd0) == param_1) {
          iVar6 = *(int *)(param_2 + 0xd8);
        }
        else {
          iVar6 = *piVar8;
          DataMemoryBarrier(0,1);
        }
        if (*(long *)(*(long *)(param_2 + 0x10) + (long)iVar6 * 0xf8 + 0x10) == 0) {
          if (mbox_rx_fifo_cnt < 0x40) {
            if (0x3f < mbox_rx_fifo_cnt) {
              __ubsan_handle_cfi_check_fail_abort(0x12);
              goto LAB_00103d14;
            }
            uVar7 = (ulong)mbox_rx_fifo_cnt;
            (&mbox_rx_fifo)[uVar7 * 2] = (long)iVar6;
            if (0x400 < (uVar7 << 4 | 8)) {
                    /* WARNING: Does not return */
              pcVar4 = (code *)SoftwareBreakpoint(1,0x103dc4);
              (*pcVar4)();
            }
            mbox_rx_fifo_cnt = uVar2 + 1;
            *(undefined8 *)(&DAT_001001e8 + uVar7 * 0x10) = local_70;
          }
          else {
            _dev_err(*(undefined8 *)(param_2 + 0x88),"mbox_mbox_rx_fifo full, core_id = %d\n",iVar6)
            ;
          }
        }
        else {
          mbox_chan_received_data(*(long *)(param_2 + 0x10) + (long)iVar6 * 0xf8,&local_70);
        }
        DataMemoryBarrier(0,2);
        iVar5 = iVar5 + -1;
        piVar8[3] = 1;
      } while (iVar5 != 0);
      goto LAB_00103d2c;
    }
  }
LAB_00103d14:
  iVar5 = ___ratelimit(&sprd_mbox_phy_outbox_isr__rs,"sprd_mbox_phy_outbox_isr");
  if (iVar5 != 0) {
    _dev_warn(*(undefined8 *)(param_2 + 0x88),"spurious outbox interrupt\n");
  }
LAB_00103d2c:
  pcVar4 = mbox_wakeup_flag_callback_r1;
  if (mbox_wakeup_flag_callback_r1 == (code *)0x0) {
    printk(&DAT_00100953);
  }
  else {
    if (mbox_wakeup_flag_callback_r1 != __typeid__ZTSFvbE_global_addr) {
      __cfi_slowpath(0x301714697a011648,mbox_wakeup_flag_callback_r1);
    }
    (*pcVar4)(0);
  }
  DataMemoryBarrier(0,2);
  piVar8[6] = 1;
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 1;
}


