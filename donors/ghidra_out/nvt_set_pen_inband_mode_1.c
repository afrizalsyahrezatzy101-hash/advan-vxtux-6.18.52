// nvt_set_pen_inband_mode_1 @ 0012efac
// signature: undefined nvt_set_pen_inband_mode_1(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nvt_set_pen_inband_mode_1(undefined1 param_1,undefined1 param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_48;
  undefined4 local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_48 = 0x2c150;
  local_3c = (uint)CONCAT12((char)(*(uint *)ts[0x1f] >> 7),
                            CONCAT11((char)(*(uint *)ts[0x1f] >> 0xf),0xff));
  CTP_SPI_WRITE(*ts,&local_3c,3);
  uVar2 = local_48;
  local_48._5_3_ = SUB83(uVar2,5);
  local_48._0_5_ = CONCAT14(param_2,CONCAT13(param_1,(undefined3)local_48));
  CTP_SPI_WRITE(*ts,&local_48,5);
  local_48 = CONCAT62(local_48._2_6_,0xff50);
  CTP_SPI_READ(*ts,&local_48,2);
  if (local_48._1_1_ != '\0') {
    usleep_range(10000,10000);
    local_48 = CONCAT62(local_48._2_6_,0xff50);
    CTP_SPI_READ(*ts,&local_48,2);
    if (local_48._1_1_ != '\0') {
      usleep_range(10000,10000);
      local_48 = CONCAT62(local_48._2_6_,0xff50);
      CTP_SPI_READ(*ts,&local_48,2);
      if (local_48._1_1_ != '\0') {
        usleep_range(10000,10000);
        local_48 = CONCAT62(local_48._2_6_,0xff50);
        CTP_SPI_READ(*ts,&local_48,2);
        if (local_48._1_1_ != '\0') {
          usleep_range(10000,10000);
          local_48 = CONCAT62(local_48._2_6_,0xff50);
          CTP_SPI_READ(*ts,&local_48,2);
          cVar1 = local_48._1_1_;
          if (local_48._1_1_ != '\0') {
            usleep_range(10000,10000);
            printk(&DAT_00121eec,"NVT-ts","nvt_set_pen_inband_mode_1",0x6e,5,cVar1);
            uVar2 = 0xffffffff;
            goto LAB_0012f11c;
          }
        }
      }
    }
  }
  uVar2 = 0;
LAB_0012f11c:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


