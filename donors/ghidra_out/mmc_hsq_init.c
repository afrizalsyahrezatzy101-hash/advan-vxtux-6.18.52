// mmc_hsq_init @ 00101234
// signature: undefined mmc_hsq_init(void)
// calling convention: unknown


undefined8 mmc_hsq_init(undefined8 *param_1,undefined8 *param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  param_1[0xd] = 0x4000000040;
  *(undefined4 *)((long)param_1 + 0x74) = 0x40;
  lVar1 = devm_kmalloc(*param_2,0x200,0xdc0);
  param_1[5] = lVar1;
  if (lVar1 == 0) {
    uVar2 = 0xfffffff4;
  }
  else {
    lVar1 = 0;
    *param_1 = param_2;
    param_2[0xca] = &mmc_hsq_ops;
    param_2[0xcb] = param_1;
    do {
      *(undefined4 *)((long)param_1 + lVar1 + 0x78) = 0x40;
      lVar1 = lVar1 + 4;
    } while (lVar1 != 0x100);
    param_1[7] = 0xfffffffe0;
    param_1[8] = param_1 + 8;
    param_1[9] = param_1 + 8;
    param_1[10] = &LAB_001021a4;
    *(undefined4 *)(param_1 + 6) = 0;
    __init_waitqueue_head(param_1 + 2,"&hsq->wait_queue",0x100000);
    uVar2 = 0;
  }
  return uVar2;
}


