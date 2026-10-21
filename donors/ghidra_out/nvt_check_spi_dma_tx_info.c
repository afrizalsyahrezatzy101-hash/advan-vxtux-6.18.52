// nvt_check_spi_dma_tx_info @ 0012578c
// signature: undefined nvt_check_spi_dma_tx_info(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nvt_check_spi_dma_tx_info(void)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  int iVar3;
  undefined8 unaff_x30;
  undefined8 local_48;
  undefined4 local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  iVar3 = 200;
  local_38 = ___stack_chk_guard;
  local_48 = 0;
  do {
    local_3c = (uint)CONCAT12((char)(*(uint *)(ts[0x1f] + 0x88) >> 7),
                              CONCAT11((char)(*(uint *)(ts[0x1f] + 0x88) >> 0xf),0xff));
    CTP_SPI_WRITE(*ts,&local_3c,3);
    local_48 = CONCAT62(local_48._2_6_,0xff00);
    local_48 = CONCAT71(local_48._1_7_,*(undefined1 *)(ts[0x1f] + 0x88)) & 0xffffffffffffff7f;
    CTP_SPI_READ(*ts,&local_48,2);
    cVar1 = local_48._1_1_;
    if (local_48._1_1_ == '\0') {
      uVar2 = 0;
      goto LAB_00125868;
    }
    usleep_range(1000,1000);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  printk(&DAT_00121eec,"NVT-ts","nvt_check_spi_dma_tx_info",0x1f9,200,cVar1);
  uVar2 = 0xffffffff;
LAB_00125868:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


