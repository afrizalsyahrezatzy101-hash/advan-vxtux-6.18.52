// signature: undefined aud_smsg_ch_send(void)

void aud_smsg_ch_send(long param_1,undefined1 *param_2)

{
  int *piVar1;
  uint *puVar2;
  char cVar3;
  bool bVar4;
  undefined1 *puVar5;
  undefined1 *__dest;
  undefined1 *__src;
  size_t __n;
  ulong uVar7;
  long lVar8;
  undefined1 *puVar9;
  long lVar10;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  undefined1 *puVar6;
  
  *in_x18 = unaff_x30;
  if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
    piVar1 = (int *)(param_1 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar4) {
        *piVar1 = *piVar1 + 1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    uVar7 = (ulong)*(ushort *)(param_2 + 2);
    if (0x11 < uVar7) goto LAB_001064e0;
    if (*(int *)(param_1 + uVar7 * 4 + 0x364) != 0) {
      if (0x10 < *(ushort *)(param_2 + 2)) goto LAB_001064e0;
      if (*(char *)(param_1 + uVar7 + 0x3a8) == '\x02') {
        lVar8 = *(long *)(param_1 + uVar7 * 8 + 0x298);
        puVar2 = (uint *)(lVar8 + 0x38);
        if ((int)(*puVar2 - *(int *)(lVar8 + 0x3c)) < 0x100) {
          lVar10 = lVar8 + (ulong)(*puVar2 & 0xff) * 0x14;
          __dest = (undefined1 *)(lVar10 + 0x40);
          if ((((uint)__dest ^ (uint)param_2) & 7) == 0) {
            if (((ulong)param_2 & 7) == 0) {
              __n = 0x14;
              __src = param_2;
            }
            else {
              puVar5 = __dest;
              puVar6 = param_2;
              puVar9 = param_2;
              do {
                puVar9 = puVar9 + 1;
                __src = puVar6 + 1;
                __dest = puVar5 + 1;
                *puVar5 = *puVar6;
                puVar5 = __dest;
                puVar6 = __src;
              } while (((ulong)puVar9 & 7) != 0);
              __n = ((ulong)param_2 | 0xfffffffffffffff8) + 0x14;
            }
            memcpy(__dest,__src,__n);
          }
          else {
            lVar8 = lVar8 + (ulong)(*puVar2 & 0xff) * 0x14;
            *__dest = *param_2;
            *(undefined1 *)(lVar10 + 0x41) = param_2[1];
            *(undefined1 *)(lVar8 + 0x42) = param_2[2];
            *(undefined1 *)(lVar8 + 0x43) = param_2[3];
            *(undefined1 *)(lVar8 + 0x44) = param_2[4];
            *(undefined1 *)(lVar8 + 0x45) = param_2[5];
            *(undefined1 *)(lVar8 + 0x46) = param_2[6];
            *(undefined1 *)(lVar8 + 0x47) = param_2[7];
            *(undefined1 *)(lVar8 + 0x48) = param_2[8];
            *(undefined1 *)(lVar8 + 0x49) = param_2[9];
            *(undefined1 *)(lVar8 + 0x4a) = param_2[10];
            *(undefined1 *)(lVar8 + 0x4b) = param_2[0xb];
            *(undefined1 *)(lVar8 + 0x4c) = param_2[0xc];
            *(undefined1 *)(lVar8 + 0x4d) = param_2[0xd];
            *(undefined1 *)(lVar8 + 0x4e) = param_2[0xe];
            *(undefined1 *)(lVar8 + 0x4f) = param_2[0xf];
            *(undefined1 *)(lVar8 + 0x50) = param_2[0x10];
            *(undefined1 *)(lVar8 + 0x51) = param_2[0x11];
            *(undefined1 *)(lVar8 + 0x52) = param_2[0x12];
            *(undefined1 *)(lVar8 + 0x53) = param_2[0x13];
          }
          *puVar2 = *puVar2 + 1;
        }
        else {
          printk(&DAT_0010238e);
        }
        if (0x10 < (ulong)*(ushort *)(param_2 + 2)) {
LAB_001064e0:
                    /* WARNING: Subroutine does not return */
          __ubsan_handle_cfi_check_fail_abort(0x12);
        }
        __wake_up(*(undefined8 *)(param_1 + (ulong)*(ushort *)(param_2 + 2) * 8 + 0x298),1,0,0);
        uVar7 = (ulong)*(ushort *)(param_2 + 2);
        if (0x11 < uVar7) goto LAB_001064e0;
      }
    }
    piVar1 = (int *)(param_1 + uVar7 * 4 + 800);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar4) {
        *piVar1 = *piVar1 + -1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
  }
  return;
}



