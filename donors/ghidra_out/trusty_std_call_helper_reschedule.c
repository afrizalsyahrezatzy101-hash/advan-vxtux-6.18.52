// trusty_std_call_helper_reschedule @ 001014d8
// signature: undefined trusty_std_call_helper_reschedule(void)
// calling convention: unknown


long trusty_std_call_helper_reschedule(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  undefined8 *in_x18;
  long lVar8;
  int iVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *param_1;
  lVar4 = param_1[1];
  lVar3 = param_1[2];
  lVar5 = param_1[3];
  lVar8 = param_1[4];
  uVar6 = daif;
  daif = uVar6 | 0x80;
  lVar1 = *(long *)(lVar2 + 0x98) + 0x20;
  atomic_notifier_call_chain(lVar1,0,0);
  lVar7 = trusty_std_call_inner(lVar2,lVar4,lVar3,lVar5,lVar8);
  if (lVar7 == -0xb) {
    iVar9 = 1;
  }
  else {
    iVar9 = 1;
    do {
      atomic_notifier_call_chain(lVar1,1,0);
      uVar6 = daif;
      daif = uVar6 & 0xffffffffffffff7f;
      if ((int)lVar7 != -5) goto LAB_0010156c;
      if (iVar9 == 0x100) {
        _dev_warn(lVar2,"%s(0x%lx 0x%lx 0x%lx 0x%lx) returned busy\n",
                  "trusty_std_call_helper_reschedule",lVar4,lVar3,lVar5,lVar8);
      }
      msleep(iVar9);
      uVar6 = daif;
      daif = uVar6 | 0x80;
      iVar9 = iVar9 << (iVar9 < 1000);
      atomic_notifier_call_chain(lVar1,0,0);
      lVar7 = trusty_std_call_inner(lVar2,lVar4,lVar3,lVar5,lVar8);
    } while (lVar7 != -0xb);
  }
  atomic_notifier_call_chain(lVar1,2,0);
  uVar6 = daif;
  daif = uVar6 & 0xffffffffffffff7f;
  lVar7 = -0xb;
LAB_0010156c:
  if (0x100 < iVar9) {
    _dev_warn(lVar2,"%s(0x%lx 0x%lx 0x%lx 0x%lx) busy cleared\n","trusty_std_call_helper_reschedule"
              ,lVar4,lVar3,lVar5,lVar8);
  }
  return lVar7;
}


