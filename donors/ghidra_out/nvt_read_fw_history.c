// nvt_read_fw_history @ 00126138
// signature: undefined nvt_read_fw_history(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nvt_read_fw_history(uint param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  undefined4 local_124;
  char local_120 [128];
  undefined8 local_a0;
  byte local_98 [64];
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  local_98[0x38] = 0;
  local_98[0x28] = 0;
  local_98[0x29] = 0;
  local_98[0x2a] = 0;
  local_98[0x2b] = 0;
  local_98[0x2c] = 0;
  local_98[0x2d] = 0;
  local_98[0x2e] = 0;
  local_98[0x2f] = 0;
  local_98[0x30] = 0;
  local_98[0x31] = 0;
  local_98[0x32] = 0;
  local_98[0x33] = 0;
  local_98[0x34] = 0;
  local_98[0x35] = 0;
  local_98[0x36] = 0;
  local_98[0x37] = 0;
  local_98[0x18] = 0;
  local_98[0x19] = 0;
  local_98[0x1a] = 0;
  local_98[0x1b] = 0;
  local_98[0x1c] = 0;
  local_98[0x1d] = 0;
  local_98[0x1e] = 0;
  local_98[0x1f] = 0;
  local_98[0x20] = 0;
  local_98[0x21] = 0;
  local_98[0x22] = 0;
  local_98[0x23] = 0;
  local_98[0x24] = 0;
  local_98[0x25] = 0;
  local_98[0x26] = 0;
  local_98[0x27] = 0;
  local_98[8] = 0;
  local_98[9] = 0;
  local_98[10] = 0;
  local_98[0xb] = 0;
  local_98[0xc] = 0;
  local_98[0xd] = 0;
  local_98[0xe] = 0;
  local_98[0xf] = 0;
  local_98[0x10] = 0;
  local_98[0x11] = 0;
  local_98[0x12] = 0;
  local_98[0x13] = 0;
  local_98[0x14] = 0;
  local_98[0x15] = 0;
  local_98[0x16] = 0;
  local_98[0x17] = 0;
  local_a0 = 0;
  local_98[0] = 0;
  local_98[1] = 0;
  local_98[2] = 0;
  local_98[3] = 0;
  local_98[4] = 0;
  local_98[5] = 0;
  local_98[6] = 0;
  local_98[7] = 0;
  local_120[0x70] = '\0';
  local_120[0x71] = '\0';
  local_120[0x72] = '\0';
  local_120[0x73] = '\0';
  local_120[0x74] = '\0';
  local_120[0x75] = '\0';
  local_120[0x76] = '\0';
  local_120[0x77] = '\0';
  local_120[0x78] = '\0';
  local_120[0x79] = '\0';
  local_120[0x7a] = '\0';
  local_120[0x7b] = '\0';
  local_120[0x7c] = '\0';
  local_120[0x7d] = '\0';
  local_120[0x7e] = '\0';
  local_120[0x7f] = '\0';
  local_120[0x60] = '\0';
  local_120[0x61] = '\0';
  local_120[0x62] = '\0';
  local_120[99] = '\0';
  local_120[100] = '\0';
  local_120[0x65] = '\0';
  local_120[0x66] = '\0';
  local_120[0x67] = '\0';
  local_120[0x68] = '\0';
  local_120[0x69] = '\0';
  local_120[0x6a] = '\0';
  local_120[0x6b] = '\0';
  local_120[0x6c] = '\0';
  local_120[0x6d] = '\0';
  local_120[0x6e] = '\0';
  local_120[0x6f] = '\0';
  local_120[0x50] = '\0';
  local_120[0x51] = '\0';
  local_120[0x52] = '\0';
  local_120[0x53] = '\0';
  local_120[0x54] = '\0';
  local_120[0x55] = '\0';
  local_120[0x56] = '\0';
  local_120[0x57] = '\0';
  local_120[0x58] = '\0';
  local_120[0x59] = '\0';
  local_120[0x5a] = '\0';
  local_120[0x5b] = '\0';
  local_120[0x5c] = '\0';
  local_120[0x5d] = '\0';
  local_120[0x5e] = '\0';
  local_120[0x5f] = '\0';
  local_120[0x40] = '\0';
  local_120[0x41] = '\0';
  local_120[0x42] = '\0';
  local_120[0x43] = '\0';
  local_120[0x44] = '\0';
  local_120[0x45] = '\0';
  local_120[0x46] = '\0';
  local_120[0x47] = '\0';
  local_120[0x48] = '\0';
  local_120[0x49] = '\0';
  local_120[0x4a] = '\0';
  local_120[0x4b] = '\0';
  local_120[0x4c] = '\0';
  local_120[0x4d] = '\0';
  local_120[0x4e] = '\0';
  local_120[0x4f] = '\0';
  local_120[0x30] = '\0';
  local_120[0x31] = '\0';
  local_120[0x32] = '\0';
  local_120[0x33] = '\0';
  local_120[0x34] = '\0';
  local_120[0x35] = '\0';
  local_120[0x36] = '\0';
  local_120[0x37] = '\0';
  local_120[0x38] = '\0';
  local_120[0x39] = '\0';
  local_120[0x3a] = '\0';
  local_120[0x3b] = '\0';
  local_120[0x3c] = '\0';
  local_120[0x3d] = '\0';
  local_120[0x3e] = '\0';
  local_120[0x3f] = '\0';
  local_120[0x20] = '\0';
  local_120[0x21] = '\0';
  local_120[0x22] = '\0';
  local_120[0x23] = '\0';
  local_120[0x24] = '\0';
  local_120[0x25] = '\0';
  local_120[0x26] = '\0';
  local_120[0x27] = '\0';
  local_120[0x28] = '\0';
  local_120[0x29] = '\0';
  local_120[0x2a] = '\0';
  local_120[0x2b] = '\0';
  local_120[0x2c] = '\0';
  local_120[0x2d] = '\0';
  local_120[0x2e] = '\0';
  local_120[0x2f] = '\0';
  local_120[0x10] = '\0';
  local_120[0x11] = '\0';
  local_120[0x12] = '\0';
  local_120[0x13] = '\0';
  local_120[0x14] = '\0';
  local_120[0x15] = '\0';
  local_120[0x16] = '\0';
  local_120[0x17] = '\0';
  local_120[0x18] = '\0';
  local_120[0x19] = '\0';
  local_120[0x1a] = '\0';
  local_120[0x1b] = '\0';
  local_120[0x1c] = '\0';
  local_120[0x1d] = '\0';
  local_120[0x1e] = '\0';
  local_120[0x1f] = '\0';
  local_120[0] = '\0';
  local_120[1] = '\0';
  local_120[2] = '\0';
  local_120[3] = '\0';
  local_120[4] = '\0';
  local_120[5] = '\0';
  local_120[6] = '\0';
  local_120[7] = '\0';
  local_120[8] = '\0';
  local_120[9] = '\0';
  local_120[10] = '\0';
  local_120[0xb] = '\0';
  local_120[0xc] = '\0';
  local_120[0xd] = '\0';
  local_120[0xe] = '\0';
  local_120[0xf] = '\0';
  if (param_1 != 0) {
    local_124._0_2_ = CONCAT11((char)(param_1 >> 0xf),0xff);
    local_124._0_3_ = CONCAT12((char)(param_1 >> 7),(undefined2)local_124);
    local_124 = (uint)(uint3)local_124;
    CTP_SPI_WRITE(*ts,&local_124,3);
    local_a0 = CONCAT71(local_a0._1_7_,(char)param_1) & 0xffffffffffffff7f;
    CTP_SPI_READ(*ts,&local_a0,0x41);
    printk(&DAT_001230d9,"NVT-ts","nvt_read_fw_history",0x523,param_1);
    lVar1 = 0;
    do {
      snprintf(local_120,0x80,
               "%02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X\n",
               (ulong)*(byte *)((long)&local_a0 + lVar1 + 1),
               (ulong)*(byte *)((long)&local_a0 + lVar1 + 2),
               (ulong)*(byte *)((long)&local_a0 + lVar1 + 3),
               (ulong)*(byte *)((long)&local_a0 + lVar1 + 4),
               (ulong)*(byte *)((long)&local_a0 + lVar1 + 5),
               (uint)*(byte *)((long)&local_a0 + lVar1 + 6),
               (uint)*(byte *)((long)&local_a0 + lVar1 + 7),(uint)local_98[lVar1],
               (uint)local_98[lVar1 + 1],(uint)local_98[lVar1 + 2],(uint)local_98[lVar1 + 3],
               (uint)local_98[lVar1 + 4],(uint)local_98[lVar1 + 5],(uint)local_98[lVar1 + 6],
               (uint)local_98[lVar1 + 7],(uint)local_98[lVar1 + 8]);
      printk(&DAT_00121389,"NVT-ts","nvt_read_fw_history",0x52c,local_120);
      lVar1 = lVar1 + 0x10;
    } while (lVar1 != 0x40);
    local_124._0_2_ = CONCAT11((char)(*(uint *)ts[0x1f] >> 0xf),0xff);
    local_124._0_3_ = CONCAT12((char)(*(uint *)ts[0x1f] >> 7),(undefined2)local_124);
    local_124 = (uint)(uint3)local_124;
    CTP_SPI_WRITE(*ts,&local_124,3);
  }
  if (___stack_chk_guard == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


