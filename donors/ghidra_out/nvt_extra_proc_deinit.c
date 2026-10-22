// nvt_extra_proc_deinit @ 00130920
// signature: undefined nvt_extra_proc_deinit(void)
// calling convention: unknown


void nvt_extra_proc_deinit(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (NVT_proc_fw_version_entry != 0) {
    remove_proc_entry("nvt_fw_version",0);
    NVT_proc_fw_version_entry = 0;
    printk(&DAT_00123a26,"NVT-ts","nvt_extra_proc_deinit",0x352,"nvt_fw_version");
  }
  if (NVT_proc_baseline_entry != 0) {
    remove_proc_entry("nvt_baseline",0);
    NVT_proc_baseline_entry = 0;
    printk(&DAT_00123a26,"NVT-ts","nvt_extra_proc_deinit",0x358,"nvt_baseline");
  }
  if (NVT_proc_raw_entry != 0) {
    remove_proc_entry("nvt_raw",0);
    NVT_proc_raw_entry = 0;
    printk(&DAT_00123a26,"NVT-ts","nvt_extra_proc_deinit",0x35e,"nvt_raw");
  }
  if (NVT_proc_diff_entry != 0) {
    remove_proc_entry("nvt_diff",0);
    NVT_proc_diff_entry = 0;
    printk(&DAT_00123a26,"NVT-ts","nvt_extra_proc_deinit",0x364,"nvt_diff");
  }
  if ((*(char *)(ts + 0x13a) != '\0') && (NVT_proc_pen_diff_entry != 0)) {
    remove_proc_entry("nvt_pen_diff",0);
    NVT_proc_pen_diff_entry = 0;
    printk(&DAT_00123a26,"NVT-ts","nvt_extra_proc_deinit",0x36b,"nvt_pen_diff");
  }
  return;
}


