// signature: undefined audio_sipc_probe(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void audio_sipc_probe(long param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  long lVar5;
  ulong uVar6;
  undefined8 uVar7;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  int local_5c;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  printk(&DAT_00101e91,"audio_sipc_probe");
  printk(&DAT_00100e22,"aud_ipc_init");
  __mutex_init(&DAT_00100530,"&aud_ipc->lock_block_param",&audio_sipc_create_s_sipc_inst);
  __mutex_init(&DAT_00100510,"&aud_ipc->lock",&audio_sipc_create_s_sipc_inst);
  local_5c = 0;
  uVar3 = audio_mem_alloc(7,&local_5c);
  if (uVar3 != 0) {
    lVar5 = audio_mem_vmap((ulong)uVar3,local_5c,0);
    if (lVar5 != 0) {
      DAT_00100560 = local_5c;
      DAT_00100550 = lVar5;
      DAT_00100558 = (ulong)uVar3;
      __memset_io(lVar5,0);
      uVar2 = local_5c - 0x10;
      uVar1 = uVar2 >> 1;
      uVar6 = (ulong)uVar1;
      printk(&DAT_00101d50,"audio_sipc_create",lVar5,uVar3,uVar6);
      DAT_001005b0 = 1;
      audio_sipc_create_s_sipc_inst = "ap-agdsp-ipc";
      DAT_001005c8 = uVar6 + lVar5 + uVar6;
      DAT_00100984 = uVar1 + uVar3;
      DAT_001005d0 = DAT_001005c8 + 4;
      DAT_0010097c = DAT_00100984 + uVar1;
      DAT_001005e8 = DAT_001005c8 + 8;
      DAT_001005f0 = DAT_001005c8 + 0xc;
      DAT_00100980 = DAT_0010097c + 4;
      DAT_00100988 = DAT_0010097c + 8;
      DAT_0010098c = DAT_0010097c + 0xc;
      DAT_001005c0 = uVar2 / 0x28;
      DAT_001005f8 = 0;
      DAT_001005b8 = lVar5;
      DAT_001005d8 = uVar6 + lVar5;
      DAT_001005e0 = DAT_001005c0;
      DAT_00100978 = uVar3;
      aud_smsg_ipc_create(1);
      printk(&DAT_00100c71,"audio_sipc_create");
      uVar3 = audio_mem_alloc(6,&DAT_00100448);
      _DAT_00100440 = (ulong)uVar3;
      if (uVar3 == 0) {
        printk(&DAT_0010112f,"aud_ipc_init",0x1d7);
        DAT_001009a0 = 0;
      }
      else {
        g_aud_ipc = audio_mem_vmap(_DAT_00100440,DAT_00100448,0);
        if (g_aud_ipc == 0) {
          printk(&DAT_00101a06,"aud_ipc_command_param_init");
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(0,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100450 = 0;
          DAT_00100460 = local_5c;
          DAT_00100454 = iVar4;
          DAT_00100458 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(1,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100468 = 1;
          DAT_00100478 = local_5c;
          DAT_0010046c = iVar4;
          DAT_00100470 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(2,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100480 = 2;
          DAT_00100490 = local_5c;
          DAT_00100484 = iVar4;
          DAT_00100488 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(3,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_00100498 = 3;
          DAT_001004a8 = local_5c;
          DAT_0010049c = iVar4;
          DAT_001004a0 = uVar7;
        }
        local_5c = 0;
        iVar4 = audio_mem_alloc(4,&local_5c);
        if (iVar4 != 0) {
          uVar7 = audio_mem_vmap(iVar4,local_5c,0);
          __memset_io(uVar7,0,local_5c);
          DAT_001004c0 = local_5c;
          DAT_001004b0 = 4;
          DAT_001004b4 = iVar4;
          DAT_001004b8 = uVar7;
        }
      }
      goto LAB_0010457c;
    }
    printk(&DAT_00101a5f,"audio_sipc_create");
  }
  printk(&DAT_0010163e,"aud_ipc_init",0xffffffff);
LAB_0010457c:
  DAT_00100588 = &LAB_00107680;
  DAT_00100598 = 0;
  DAT_00100570 = 0;
  DAT_00100580 = 0;
  DAT_00100578 = 500;
  DAT_00100568 = param_1 + 0x10;
  DAT_001005a0 = mbox_request_channel(&DAT_00100568,0);
  if (DAT_001005a0 < 0xfffffffffffff001) {
    printk(&DAT_001011dc);
    aud_smsg_mboxchan = DAT_001005a0;
    agdsp_set_mboxchan();
    uVar6 = 0;
  }
  else {
    _dev_err(param_1 + 0x10,"mailbox request chan fail !!!");
    uVar6 = DAT_001005a0 & 0xffffffff;
  }
  if (___stack_chk_guard == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}



