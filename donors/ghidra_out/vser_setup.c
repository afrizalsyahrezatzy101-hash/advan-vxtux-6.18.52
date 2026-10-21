// vser_setup @ 00101da4
// signature: undefined vser_setup(void)
// calling convention: unknown


undefined8 vser_setup(long param_1,byte *param_2)

{
  uint *puVar1;
  undefined4 uVar2;
  uint uVar3;
  short sVar4;
  long lVar5;
  bool bVar6;
  undefined8 *in_x18;
  long *plVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = _vser_dev;
  sVar4 = *(short *)(param_2 + 6);
  plVar7 = *(long **)(*(long *)(param_1 + 0x30) + 0x30);
  if ((*param_2 & 0x60) != 0x40) {
    if (((*param_2 & 0x60) != 0x20) || (param_2[1] != 0x22)) {
      return 0xffffffa1;
    }
    bVar6 = (*(ushort *)(param_2 + 2) & 0xff) == 1;
    goto switchD_00101e3c_caseD_2;
  }
  bVar6 = false;
  if (0x20 < param_2[1]) goto switchD_00101e3c_caseD_2;
  uVar2 = CONCAT22(*(ushort *)(param_2 + 2),*(undefined2 *)(param_2 + 4));
  switch(param_2[1]) {
  case 0:
    puVar1 = (uint *)(_vser_dev + 0x174);
    bVar6 = false;
    *(undefined4 *)(_vser_dev + 0x178) = uVar2;
    *(uint *)(lVar5 + 0x174) = *puVar1 | 1;
    break;
  case 1:
    puVar1 = (uint *)(_vser_dev + 0x174);
    bVar6 = false;
    *(undefined4 *)(_vser_dev + 0x17c) = uVar2;
    *(uint *)(lVar5 + 0x174) = *puVar1 | 2;
    break;
  case 0x10:
    vser_free_all_request(_vser_dev);
    vser_alloc_all_request(lVar5);
    *(undefined4 *)(lVar5 + 0x180) = uVar2;
    *(uint *)(lVar5 + 0x174) = *(uint *)(lVar5 + 0x174) | 0x80;
    queue_work_on(0x20,*(undefined8 *)(lVar5 + 0x1b8),lVar5 + 0x188);
    goto LAB_00101eac;
  case 0x20:
    vser_free_all_request(_vser_dev);
LAB_00101eac:
    bVar6 = false;
  }
switchD_00101e3c_caseD_2:
  uVar3 = *(uint *)(plVar7[1] + 0x28);
  *(uint *)(plVar7[1] + 0x28) = uVar3 & 0xfffc0000 | uVar3 & 0x1ffff | (uint)(sVar4 != 0) << 0x11;
  *(undefined4 *)(plVar7[1] + 8) = 0;
  usb_ep_queue(*(undefined8 *)(*plVar7 + 0x40),plVar7[1],0xa20);
  if (bVar6) {
    *(undefined4 *)(lVar5 + 0xf8) = 1;
    __wake_up(lVar5 + 0x138,3,1,0);
    __wake_up(lVar5 + 0x150,3,1,0);
  }
  return 0;
}


