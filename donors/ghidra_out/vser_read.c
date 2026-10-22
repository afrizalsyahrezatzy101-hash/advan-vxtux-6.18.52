// vser_read @ 001031d8
// signature: undefined vser_read(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void vser_read(long param_1,undefined8 param_2,ulong param_3)

{
  int *piVar1;
  long lVar2;
  char cVar3;
  bool bVar4;
  code *pcVar5;
  int iVar6;
  long lVar7;
  uint uVar8;
  undefined8 *in_x18;
  undefined8 uVar9;
  undefined8 *puVar10;
  ulong uVar11;
  long lVar12;
  undefined8 unaff_x30;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  lVar12 = *(long *)(param_1 + 200);
  if ((*(int *)(lVar12 + 0x174) == 0) && (param_3 <= (ulong)(long)*(int *)(lVar12 + 0x17c))) {
    piVar1 = (int *)(lVar12 + 0x104);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      iVar6 = *piVar1;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar4) {
        *piVar1 = iVar6 + 1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    DataMemoryBarrier(2,3);
    if (iVar6 + 1 == 1) {
      lVar2 = lVar12 + 0x138;
      do {
        if (*(int *)(lVar12 + 0xf8) != 0) {
          if (*(int *)(lVar12 + 0xfc) != 0) goto LAB_00103354;
          goto LAB_001033a4;
        }
        if (*(int *)(lVar12 + 0xfc) != 0) goto LAB_00103354;
        if (*(int *)(lVar12 + 0xfc) == 0) {
          local_78 = 0;
          uStack_70 = 0;
          local_88 = 0;
          uStack_80 = 0;
          local_90 = 0;
          init_wait_entry(&local_90,0);
          while ((lVar7 = prepare_to_wait_event(lVar2,&local_90,1), *(int *)(lVar12 + 0xf8) == 0 &&
                 (*(int *)(lVar12 + 0xfc) == 0))) {
            if (lVar7 != 0) goto LAB_001032cc;
            schedule();
          }
          finish_wait(lVar2,&local_90);
          lVar7 = 0;
        }
        else {
          lVar7 = 0;
        }
LAB_001032cc:
      } while (-1 < (int)lVar7);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
      lVar12 = (long)(int)lVar7;
    }
    else {
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
      lVar12 = -0x10;
    }
  }
  else {
    lVar12 = -0x16;
  }
  goto LAB_00103234;
code_r0x001034b4:
  uVar11 = (ulong)uVar8;
  if (param_3 <= uVar8) {
    uVar11 = param_3;
  }
  uVar8 = (uint)uVar11;
  uVar11 = (ulong)(int)uVar8;
  if (uVar11 >> 0x1f != 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(0x800,0x103520);
    (*pcVar5)();
  }
  uVar9 = *puVar10;
  __check_object_size(uVar9,uVar11,1);
  lVar12 = _copy_to_user(param_2,uVar9,uVar11);
  if (lVar12 != 0) {
    uVar8 = 0xfffffff2;
  }
  uVar11 = (ulong)uVar8;
  goto LAB_00103358;
LAB_001033a4:
  uVar8 = (uint)param_3;
  if ((*(byte *)(**(long **)(lVar12 + 0xd8) + 0x431) & 1) != 0) {
    uVar8 = (uint)*(ushort *)(*(long *)(lVar12 + 0xf0) + 0x2e);
    uVar8 = ((uint)param_3 - 1) + uVar8 & -uVar8;
  }
  puVar10 = *(undefined8 **)(lVar12 + 0x168);
  *(uint *)(puVar10 + 1) = uVar8;
  *(undefined4 *)(lVar12 + 0x170) = 0;
  iVar6 = usb_ep_queue(*(undefined8 *)(lVar12 + 0xf0),puVar10,0xa20);
  if (-1 < iVar6) {
    if (*(int *)(lVar12 + 0x170) == 0) {
      local_78 = 0;
      uStack_70 = 0;
      local_88 = 0;
      uStack_80 = 0;
      local_90 = 0;
      init_wait_entry(&local_90,0);
      uVar11 = prepare_to_wait_event(lVar2,&local_90,1);
      iVar6 = *(int *)(lVar12 + 0x170);
      while (iVar6 == 0) {
        if (uVar11 != 0) {
          if (-1 < (int)uVar11) goto LAB_0010345c;
          usb_ep_dequeue(*(undefined8 *)(lVar12 + 0xf0),puVar10);
          *(undefined4 *)(lVar12 + 0xfc) = 1;
          goto LAB_00103358;
        }
        schedule();
        uVar11 = prepare_to_wait_event(lVar2,&local_90,1);
        iVar6 = *(int *)(lVar12 + 0x170);
      }
      finish_wait(lVar2,&local_90);
    }
LAB_0010345c:
    if (*(int *)(lVar12 + 0xfc) != 0) goto LAB_00103354;
    uVar8 = *(uint *)(puVar10 + 0xb);
    if (uVar8 != 0) goto code_r0x001034b4;
    goto LAB_001033a4;
  }
  *(undefined4 *)(lVar12 + 0xfc) = 1;
LAB_00103354:
  uVar11 = 0xfffffffb;
LAB_00103358:
  Hint_Prefetch(piVar1,2,0,1);
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
    if (bVar4) {
      *piVar1 = *piVar1 + -1;
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  lVar12 = (long)(int)uVar11;
LAB_00103234:
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar12);
  }
  return;
}


