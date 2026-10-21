// trusty_log_remove @ 00101370
// signature: undefined trusty_log_remove(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 trusty_log_remove(long param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  undefined8 *in_x18;
  long lVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *(long *)(param_1 + 0xa8);
  atomic_notifier_chain_unregister(&panic_notifier_list,lVar4 + 0x58);
  trusty_call_notifier_unregister(*(undefined8 *)(lVar4 + 8),lVar4 + 0x40);
  uVar5 = *(ulong *)(lVar4 + 0x28);
  if ((uVar5 ^ 0xffffff8000000000) < (-1L << (_vabits_actual - 1U & 0x3f)) + 0x8000000000U) {
    uVar3 = _memstart_addr + (uVar5 & 0x7fffffffff);
  }
  else {
    uVar3 = uVar5 - _kimage_voffset;
  }
  iVar2 = trusty_std_call32(*(undefined8 *)(lVar4 + 8),0x33000002,uVar3,uVar3 >> 0x20,0);
  if (iVar2 == 0) {
    free_pages(uVar5);
  }
  else {
    printk(&DAT_001004ec,iVar2);
  }
  lVar6 = *(long *)(lVar4 + 0x30);
  iVar2 = *(int *)(lVar4 + 0x38);
  lVar7 = (long)iVar2;
  if (*(long *)(lVar4 + 0x18) != 0) {
    vm_unmap_ram(*(long *)(lVar4 + 0x18),iVar2);
  }
  if (iVar2 != 0) {
    do {
      lVar1 = lVar7 * 8;
      lVar7 = lVar7 + -1;
      __free_pages(*(undefined8 *)(lVar6 + -8 + lVar1),0);
    } while (lVar7 != 0);
  }
  kfree(lVar6);
  kfree(lVar4);
  sysfs_remove_group(param_1 + 0x10,&trusty_log_attr_group);
  return 0;
}


