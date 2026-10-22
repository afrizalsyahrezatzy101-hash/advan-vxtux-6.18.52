// nvt_set_pen_normal_mode @ 0012f188
// signature: undefined nvt_set_pen_normal_mode(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nvt_set_pen_normal_mode(void)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined8 local_38;
  undefined4 local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_38 = 0x4c150;
  local_2c = (uint)CONCAT12((char)(*(uint *)ts[0x1f] >> 7),
                            CONCAT11((char)(*(uint *)ts[0x1f] >> 0xf),0xff));
  CTP_SPI_WRITE(*ts,&local_2c,3);
  CTP_SPI_WRITE(*ts,&local_38,3);
  local_38 = CONCAT62(local_38._2_6_,0xff50);
  CTP_SPI_READ(*ts,&local_38,2);
  if (local_38._1_1_ != '\0') {
    usleep_range(10000,10000);
    local_38 = CONCAT62(local_38._2_6_,0xff50);
    CTP_SPI_READ(*ts,&local_38,2);
    if (local_38._1_1_ != '\0') {
      usleep_range(10000,10000);
      local_38 = CONCAT62(local_38._2_6_,0xff50);
      CTP_SPI_READ(*ts,&local_38,2);
      if (local_38._1_1_ != '\0') {
        usleep_range(10000,10000);
        local_38 = CONCAT62(local_38._2_6_,0xff50);
        CTP_SPI_READ(*ts,&local_38,2);
        if (local_38._1_1_ != '\0') {
          usleep_range(10000,10000);
          local_38 = CONCAT62(local_38._2_6_,0xff50);
          CTP_SPI_READ(*ts,&local_38,2);
          cVar1 = local_38._1_1_;
          if (local_38._1_1_ != '\0') {
            usleep_range(10000,10000);
            printk(&DAT_00121eec,"NVT-ts","nvt_set_pen_normal_mode",0x90,5,cVar1);
            uVar2 = 0xffffffff;
            goto LAB_0012f2e0;
          }
        }
      }
    }
  }
  uVar2 = 0;
LAB_0012f2e0:
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


