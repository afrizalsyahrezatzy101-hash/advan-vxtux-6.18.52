// sprd_mbox_deliver_thread @ 00102868
// signature: undefined sprd_mbox_deliver_thread(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 sprd_mbox_deliver_thread(long param_1)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  undefined8 *in_x18;
  uint uVar7;
  ulong uVar8;
  code *pcVar9;
  undefined8 unaff_x30;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  local_70 = 0;
  uVar2 = kthread_should_stop();
joined_r0x001028a4:
  do {
    if ((uVar2 & 1) != 0) {
      if (___stack_chk_guard == local_68) {
        return 0;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    if (mbox_tx_mask == 0) {
      local_80 = 0;
      uStack_78 = 0;
      local_90 = 0;
      uStack_88 = 0;
      local_98 = 0;
      init_wait_entry(&local_98,0);
      while (lVar3 = prepare_to_wait_event(&deliver_thread_wait,&local_98,1), mbox_tx_mask == 0) {
        if (lVar3 != 0) {
          if ((int)lVar3 == 0) goto LAB_0010295c;
          _dev_warn(*(undefined8 *)(param_1 + 0x88),"mbox_deliver_thread is interrupted\n");
          uVar2 = kthread_should_stop();
          goto joined_r0x001028a4;
        }
        schedule();
      }
      finish_wait(&deliver_thread_wait,&local_98);
    }
LAB_0010295c:
    uVar4 = _raw_spin_lock_irqsave(&mbox_lock);
    uVar2 = (ulong)chan_num;
    if (chan_num != 0) {
      uVar8 = 0;
      do {
        if (((mbox_tx_mask >> (ulong)((uint)uVar8 & 0x1f) & 1) != 0) &&
           (lVar3 = tx_data + uVar8 * 0x208, uVar7 = *(uint *)(lVar3 + 0x204),
           *(uint *)(lVar3 + 0x200) != uVar7)) {
          lVar5 = *(long *)(param_1 + 0x10);
          lVar3 = tx_data;
          do {
            local_70 = *(undefined8 *)(lVar3 + uVar8 * 0x208 + (ulong)(uVar7 & 0x3f) * 8);
            pcVar9 = *(code **)(*(long *)(param_1 + 0xe0) + 0x18);
            if (1 < ((ulong)(pcVar9 + -0x10437c) >> 2 | (long)pcVar9 << 0x3e)) {
              __cfi_slowpath(0xc19c8378abe429fc,pcVar9);
            }
            iVar1 = (*pcVar9)(lVar5 + uVar8 * 0xf8,&local_70);
            lVar3 = tx_data;
            if (iVar1 != 0) break;
            uVar7 = uVar7 + 1;
            lVar6 = tx_data + uVar8 * 0x208;
            *(uint *)(lVar6 + 0x204) = uVar7;
          } while (uVar7 != *(uint *)(lVar6 + 0x200));
          uVar2 = (ulong)chan_num;
        }
        uVar8 = uVar8 + 1;
      } while (uVar8 < uVar2);
    }
    mbox_tx_mask = 0;
    _raw_spin_unlock_irqrestore(&mbox_lock,uVar4);
    uVar2 = kthread_should_stop();
  } while( true );
}


