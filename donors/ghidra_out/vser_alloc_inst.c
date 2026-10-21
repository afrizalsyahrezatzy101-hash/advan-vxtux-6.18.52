// vser_alloc_inst @ 001018a8
// signature: undefined vser_alloc_inst(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long vser_alloc_inst(void)

{
  undefined8 uVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = kmem_cache_alloc_trace(_memcpy,0xdc0,200);
  uVar1 = _memset;
  if (lVar3 == 0) {
    lVar3 = -0xc;
  }
  else {
    *(undefined1 **)(lVar3 + 0xa8) = &LAB_001053f0;
    *(undefined1 **)(lVar3 + 0xb0) = &LAB_001053f4;
    lVar4 = kmem_cache_alloc_trace(uVar1,0xdc0,0x1c0);
    if (lVar4 == 0) {
      iVar2 = -0xc;
    }
    else {
      *(long *)(lVar3 + 0xc0) = lVar4;
      *(undefined4 *)(lVar4 + 0xe0) = 0;
      _vser_dev = lVar4;
      __init_waitqueue_head(lVar4 + 0x138,"&dev->read_wq",&vserusb_func);
      __init_waitqueue_head(lVar4 + 0x150,"&dev->write_wq",&vserusb_func);
      *(undefined4 *)(lVar4 + 0x10c) = 0;
      *(undefined4 *)(lVar4 + 0x104) = 0;
      *(undefined4 *)(lVar4 + 0x108) = 0;
      *(undefined8 *)(lVar4 + 0x178) = 0x400000004000;
      *(undefined4 *)(lVar4 + 0x110) = 0;
      tx_req_count = 0;
      *(long *)(lVar4 + 0x118) = lVar4 + 0x118;
      *(long *)(lVar4 + 0x120) = lVar4 + 0x118;
      *(long *)(lVar4 + 0x128) = lVar4 + 0x128;
      *(undefined8 *)(lVar4 + 0x188) = 0xfffffffe0;
      *(long *)(lVar4 + 0x130) = lVar4 + 0x128;
      *(long *)(lVar4 + 400) = lVar4 + 400;
      *(long *)(lVar4 + 0x198) = lVar4 + 400;
      *(undefined1 **)(lVar4 + 0x1a0) = &LAB_001053f8;
      lVar5 = alloc_workqueue(&DAT_001007cf,0xe000a,1,"VserTestWq");
      *(long *)(lVar4 + 0x1b8) = lVar5;
      if (lVar5 == 0) {
        iVar2 = -0xc;
      }
      else {
        iVar2 = misc_register(&vser_device);
        if (iVar2 == 0) {
          config_group_init_type_name(lVar3,&DAT_00100b15,&vser_func_type);
          return lVar3;
        }
        destroy_workqueue(*(undefined8 *)(lVar4 + 0x1b8));
      }
      kfree(lVar4);
    }
    kfree(lVar3);
    lVar3 = (long)iVar2;
  }
  return lVar3;
}


