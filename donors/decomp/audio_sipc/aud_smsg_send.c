// signature: undefined aud_smsg_send(void)

undefined4 aud_smsg_send(byte param_1,ushort *param_2)

{
  undefined8 uVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  undefined1 *puVar5;
  ushort *__src;
  size_t __n;
  ulong uVar7;
  ushort *puVar8;
  undefined8 *in_x18;
  undefined4 uVar9;
  undefined1 *__dest;
  long lVar10;
  undefined8 unaff_x30;
  ushort *puVar6;
  
  *in_x18 = unaff_x30;
  if (1 < param_1) {
LAB_00105910:
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  lVar10 = (&aud_smsg_ipcs)[param_1];
  printk(&DAT_00101cdc,"aud_smsg_send",param_1,param_2[1]);
  if (lVar10 == 0) {
    uVar9 = 0xffffffed;
  }
  else {
    uVar7 = (ulong)param_2[1];
    if (0x10 < uVar7) goto LAB_00105910;
    if (*(long *)(lVar10 + uVar7 * 8 + 0x298) == 0) {
      printk(&DAT_00102298,"aud_smsg_send",uVar7);
      uVar9 = 0xffffffed;
    }
    else if ((*(char *)(lVar10 + uVar7 + 0x3a8) == '\x02') && (*(int *)(lVar10 + 0x3cc) != 0)) {
      uVar4 = _raw_spin_lock_irqsave(lVar10 + 0x68);
      if ((uint)(**(int **)(lVar10 + 0x28) - **(int **)(lVar10 + 0x20)) < *(uint *)(lVar10 + 0x18))
      {
        if (*(int *)(lVar10 + 0x3cc) == 0) {
          printk(&DAT_00101faf,"aud_smsg_send");
          uVar9 = 0;
          uVar2 = msg_index;
        }
        else {
          __dest = (undefined1 *)
                   (*(long *)(lVar10 + 0x10) +
                   (ulong)(*(int *)(lVar10 + 0x18) - 1U & **(uint **)(lVar10 + 0x28)) * 0x14);
          if ((((uint)__dest ^ (uint)param_2) & 7) == 0) {
            if (((ulong)param_2 & 7) == 0) {
              __n = 0x14;
              __src = param_2;
            }
            else {
              puVar5 = __dest;
              puVar6 = param_2;
              puVar8 = param_2;
              do {
                puVar8 = (ushort *)((long)puVar8 + 1);
                __src = (ushort *)((long)puVar6 + 1);
                __dest = puVar5 + 1;
                *puVar5 = (char)*puVar6;
                puVar5 = __dest;
                puVar6 = __src;
              } while (((ulong)puVar8 & 7) != 0);
              __n = ((ulong)param_2 | 0xfffffffffffffff8) + 0x14;
            }
            memcpy(__dest,__src,__n);
          }
          else {
            *__dest = (char)*param_2;
            __dest[1] = *(undefined1 *)((long)param_2 + 1);
            __dest[2] = (char)param_2[1];
            __dest[3] = *(undefined1 *)((long)param_2 + 3);
            __dest[4] = (char)param_2[2];
            __dest[5] = *(undefined1 *)((long)param_2 + 5);
            __dest[6] = (char)param_2[3];
            __dest[7] = *(undefined1 *)((long)param_2 + 7);
            __dest[8] = (char)param_2[4];
            __dest[9] = *(undefined1 *)((long)param_2 + 9);
            __dest[10] = (char)param_2[5];
            __dest[0xb] = *(undefined1 *)((long)param_2 + 0xb);
            __dest[0xc] = (char)param_2[6];
            __dest[0xd] = *(undefined1 *)((long)param_2 + 0xd);
            __dest[0xe] = (char)param_2[7];
            __dest[0xf] = *(undefined1 *)((long)param_2 + 0xf);
            __dest[0x10] = (char)param_2[8];
            __dest[0x11] = *(undefined1 *)((long)param_2 + 0x11);
            __dest[0x12] = (char)param_2[9];
            __dest[0x13] = *(undefined1 *)((long)param_2 + 0x13);
          }
          **(int **)(lVar10 + 0x28) = **(int **)(lVar10 + 0x28) + 1;
          uVar1 = aud_smsg_mboxchan;
          if (0x13 < msg_index) goto LAB_00105910;
          *(ulong *)(&aud_smsg_send_msg_val + (ulong)msg_index * 8) =
               (ulong)*param_2 << 0x30 | (ulong)param_2[1] << 0x20 | (ulong)*(uint *)(param_2 + 2);
          iVar3 = mbox_send_message(uVar1);
          if (iVar3 < 0) {
            printk(&DAT_00101b95,"aud_smsg_send",iVar3);
          }
          mbox_chan_txdone(aud_smsg_mboxchan,0);
          uVar9 = 0;
          uVar2 = 0;
          if ((int)(msg_index + 1) < 0x14) {
            uVar2 = msg_index + 1;
          }
        }
      }
      else {
        printk(&DAT_00102117,"aud_smsg_send");
        uVar9 = 0xfffffff0;
        uVar2 = msg_index;
      }
      msg_index = uVar2;
      _raw_spin_unlock_irqrestore(lVar10 + 0x68,uVar4);
    }
    else {
      printk(&DAT_0010230a,"aud_smsg_send",uVar7);
      uVar9 = 0xffffffea;
    }
  }
  return uVar9;
}



