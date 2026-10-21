// trusty_remove @ 001021f0
// signature: undefined trusty_remove(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 trusty_remove(long param_1)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0xa8);
  device_for_each_child(param_1 + 0x10,0,&LAB_001034b8);
  uVar1 = cpumask_next(0xffffffff,&__cpu_possible_mask);
  if (uVar1 < _nr_cpu_ids) {
    do {
      if (0x1f < uVar1) {
                    /* WARNING: Subroutine does not return */
        __ubsan_handle_cfi_check_fail_abort(0x12);
      }
      flush_work(*(long *)(&__per_cpu_offset + (ulong)uVar1 * 8) + *(long *)(lVar2 + 0x70) + 8);
      uVar1 = cpumask_next((ulong)uVar1,&__cpu_possible_mask);
    } while (uVar1 < _nr_cpu_ids);
  }
  free_percpu(*(undefined8 *)(lVar2 + 0x70));
  destroy_workqueue(*(undefined8 *)(lVar2 + 0x68));
  if (*(long *)(lVar2 + 0x50) != 0) {
    device_remove_file(param_1 + 0x10,&dev_attr_trusty_version);
    kfree(*(undefined8 *)(lVar2 + 0x50));
  }
  kfree(lVar2);
  return 0;
}


