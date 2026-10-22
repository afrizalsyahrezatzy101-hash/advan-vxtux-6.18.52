// sprd_time_sync_probe @ 001012d4
// signature: undefined sprd_time_sync_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong sprd_time_sync_probe(long param_1)

{
  long lVar1;
  undefined8 *puVar2;
  uint uVar3;
  ulong uVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  long lVar7;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_5c;
  long local_58;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_58 = ___stack_chk_guard;
  local_5c = 0;
  _dev_info(lVar1,"start parse sprd_time_sync\n");
  uVar4 = __class_create(&__this_module,"sprd_time_sync",&sprd_time_sync_probe___key);
  sprd_time_sync_class = uVar4;
  if (uVar4 < 0xfffffffffffff001) {
    puVar5 = (undefined8 *)devm_kmalloc(lVar1,0xa0,0xdc0);
    if (puVar5 == (undefined8 *)0x0) {
      uVar4 = 0xfffffff4;
    }
    else {
      tcd_dev = puVar5;
      puVar6 = (undefined8 *)devm_kmalloc(lVar1,0x38,0xdc0);
      if (puVar6 == (undefined8 *)0x0) {
        uVar4 = 0xfffffff4;
        _dev_err(lVar1,"Failed to parse sprd_time_sync device tree, ret=%d\n",0xfffffff4);
      }
      else {
        *puVar6 = "sprd_time_sync";
        uVar3 = alloc_chrdev_region(&local_5c,0,1,"sprd_time_sync");
        if (uVar3 == 0) {
          puVar2 = puVar5 + 2;
          cdev_init(puVar2,&sprd_time_sync_fops);
          uVar3 = cdev_add(puVar2,local_5c,1);
          uVar4 = sprd_time_sync_class;
          if (uVar3 == 0) {
            *(uint *)(puVar5 + 1) = local_5c >> 0x14;
            *(uint *)((long)puVar5 + 0xc) = local_5c & 0xfffff;
            lVar7 = device_create(uVar4,0,local_5c,0,&DAT_001001b7,"sprd_time_sync");
            puVar5[0x13] = lVar7;
            if (lVar7 != 0) {
              *puVar5 = puVar6;
              *(undefined8 **)(param_1 + 0xa8) = puVar5;
              _dev_info(param_1 + 0x10,"success parse sprd_time_sync\n");
              uVar4 = 0;
              goto LAB_0010136c;
            }
            _dev_err(lVar1,"create dev failed\n");
            cdev_del(puVar2);
            uVar4 = 0xffffffed;
          }
          else {
            uVar4 = (ulong)uVar3;
            _dev_err(lVar1,"Failed to add sprd_time_sync cdev\n");
          }
          unregister_chrdev_region(local_5c,1);
        }
        else {
          uVar4 = (ulong)uVar3;
          _dev_err(lVar1,"Failed to alloc sprd_time_sync chrdev\n");
        }
      }
    }
  }
LAB_0010136c:
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar4 & 0xffffffff;
}


