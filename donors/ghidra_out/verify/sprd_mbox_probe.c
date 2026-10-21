// sprd_mbox_probe @ 001021cc
// signature: undefined sprd_mbox_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sprd_mbox_probe(long param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long *plVar4;
  ulong uVar5;
  long lVar6;
  char *pcVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  undefined8 *in_x18;
  undefined8 uVar11;
  long *plVar12;
  long *plVar13;
  undefined8 unaff_x30;
  undefined8 local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_68 = ___stack_chk_guard;
  uVar11 = *(undefined8 *)(param_1 + 0x310);
  local_70 = 0;
  plVar4 = (long *)devm_kmalloc(lVar1,0xf0,0xdc0);
  if (plVar4 == (long *)0x0) {
LAB_00102450:
    iVar3 = -0xc;
    goto LAB_00102454;
  }
  plVar4[0x11] = lVar1;
  uVar5 = syscon_regmap_lookup_by_phandle_args(uVar11,"sprd,mailbox_clk",2,&local_70);
  if (0xfffffffffffff000 < uVar5) {
    _dev_err(lVar1,"failed to get mailbox clock\n");
    iVar3 = -0x16;
    goto LAB_00102454;
  }
  regmap_update_bits_base(uVar5,local_70 & 0xffffffff,local_70._4_4_,local_70._4_4_,0,0,0);
  reg_ctrl_1 = (undefined4)local_70;
  reg_ctrl_2 = local_70._4_4_;
  reg_ctrl_0 = uVar5;
  iVar3 = sprd_mbox_phy_r1_init(plVar4);
  if (iVar3 != 0) goto LAB_00102454;
  mbox_phy_ops = plVar4[0x1c];
  chan_num = (int)plVar4[3];
  *plVar4 = lVar1;
  plVar4[1] = (long)&sprd_mbox_ops;
  *(undefined1 *)((long)plVar4 + 0x1c) = 1;
  plVar4[5] = (long)&LAB_00104334;
  lVar6 = devm_kmalloc(lVar1,(long)chan_num * 0xf8,0xdc0);
  plVar4[2] = lVar6;
  if (lVar6 == 0) goto LAB_00102450;
  lVar8 = (long)chan_num;
  if ((chan_num != 0) && (bVar2 = chan_num != 1, *(undefined8 *)(lVar6 + 0xf0) = 0, bVar2)) {
    lVar10 = 0x1e8;
    for (lVar9 = 2; *(long *)(lVar6 + lVar10) = lVar9 + -1, lVar8 != lVar9; lVar9 = lVar9 + 1) {
      lVar6 = plVar4[2];
      lVar10 = lVar10 + 0xf8;
    }
  }
  outbox_lock = 0;
  mbox_lock = 0;
  __init_waitqueue_head(&deliver_thread_wait,"&deliver_thread_wait",&sprd_mbox_probe___key);
  plVar12 = (long *)plVar4[0x11];
  plVar13 = plVar12 + -2;
  iVar3 = platform_get_irq_byname(plVar13,"inbox");
  *(int *)((long)plVar4 + 0xbc) = iVar3;
  if (iVar3 < 0) {
LAB_001024f8:
    pcVar7 = "failed to request IRQ\n";
  }
  else {
    lVar6 = plVar12[0xe];
    if (lVar6 == 0) {
      lVar6 = *plVar12;
    }
    iVar3 = devm_request_threaded_irq
                      (plVar12,iVar3,__typeid__ZTSF9irqreturniPvE_global_addr,0,0x4000,lVar6,plVar4)
    ;
    if (iVar3 != 0) {
      pcVar7 = "failed to request inbox IRQ: %d\n";
LAB_001024ec:
      _dev_err(plVar12,pcVar7,iVar3);
      goto LAB_001024f8;
    }
    iVar3 = platform_get_irq_byname(plVar13,"outbox");
    *(int *)(plVar4 + 0x18) = iVar3;
    if (iVar3 < 0) goto LAB_001024f8;
    lVar6 = plVar12[0xe];
    if (lVar6 == 0) {
      lVar6 = *plVar12;
    }
    iVar3 = devm_request_threaded_irq(plVar12,iVar3,&LAB_00104388,0,0x4000,lVar6,plVar4);
    if (iVar3 != 0) {
      pcVar7 = "failed to request outbox IRQ: %d\n";
      goto LAB_001024ec;
    }
    iVar3 = platform_get_irq_byname(plVar13,"oob-outbox");
    *(int *)(plVar4 + 0x1a) = iVar3;
    if (iVar3 < 0) {
      _dev_warn(plVar12,"failed to get oob_irq\n");
    }
    else {
      lVar6 = plVar12[0xe];
      if (lVar6 == 0) {
        lVar6 = *plVar12;
      }
      iVar3 = devm_request_threaded_irq(plVar12,iVar3,&LAB_00104388,0,0,lVar6,plVar4);
      if (iVar3 != 0) {
        _dev_err(plVar12,"failed to request oob_irq: %d\n",iVar3);
      }
    }
    iVar3 = devm_add_action(lVar1,__typeid__ZTSFvPvE_global_addr,plVar4);
    if (iVar3 == 0) {
      iVar3 = devm_mbox_controller_register(lVar1,plVar4);
      if (iVar3 != 0) {
        _dev_err(lVar1,"failed to register mailbox: %d\n",iVar3);
        goto LAB_00102454;
      }
      tx_data = devm_kmalloc(lVar1,(long)chan_num * 0x208,0xdc0);
      if (tx_data == 0) goto LAB_00102450;
      iVar3 = cpu_pm_register_notifier(&mbox_pm_notifier_block);
      if (iVar3 == 0) {
        mbox_deliver_thread =
             kthread_create_on_node(&LAB_0010433c,plVar4,0xffffffff,"mbox-deliver-thread");
        wake_up_process();
        uVar5 = debugfs_initialized();
        if ((uVar5 & 1) != 0) {
          lVar6 = debugfs_create_dir("mailbox",0);
          plVar4[0x1d] = lVar6;
          if (lVar6 == 0) {
            _dev_err(lVar1,"failed to create debugfs! ret = %d\n",0xffffffea);
          }
          else {
            debugfs_create_file(&DAT_00100cd8,0x124,lVar6,plVar4,&sprd_mbox_debugfs_ops);
          }
        }
        _dev_info(plVar4[0x11],"probe done, version=0x%x\n",(int)plVar4[0x17]);
        iVar3 = 0;
        goto LAB_00102454;
      }
      pcVar7 = "failed to register cpu pm notifier\n";
    }
    else {
      if (reg_ctrl_0 != 0) {
        regmap_update_bits_base(reg_ctrl_0,reg_ctrl_1,reg_ctrl_2,0,0,0,0);
        _dev_info(plVar4[0x11],"mailbox disabled.\n");
      }
      pcVar7 = "failed to add mailbox disable action\n";
    }
  }
  _dev_err(lVar1,pcVar7);
LAB_00102454:
  if (___stack_chk_guard == local_68) {
    return iVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


