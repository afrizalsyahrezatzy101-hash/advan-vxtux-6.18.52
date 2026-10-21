// signature: undefined aud_smsg_irq_handler(void)

undefined8 aud_smsg_irq_handler(undefined8 param_1,long param_2)

{
  short sVar1;
  undefined8 uVar2;
  undefined8 *in_x18;
  short *psVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = _raw_spin_lock_irqsave(param_2 + 0x6c);
  if (**(int **)(param_2 + 0x48) != **(int **)(param_2 + 0x40)) {
    do {
      psVar3 = (short *)(*(long *)(param_2 + 0x30) +
                        (ulong)(*(int *)(param_2 + 0x38) - 1U & **(uint **)(param_2 + 0x40)) * 0x14)
      ;
      if (((*(int *)(param_2 + 0x3cc) == 0) && (*psVar3 == -0x4112)) &&
         (*(int *)(psVar3 + 2) == 0xbeee)) {
        sVar1 = psVar3[1];
        if (sVar1 != -0x4112) goto joined_r0x00105b54;
        printk(&DAT_00100e6e);
        *(undefined4 *)(param_2 + 0x3cc) = 1;
        aud_smsg_all_ch_notify(param_2,psVar3);
        **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
      }
      else {
        sVar1 = psVar3[1];
joined_r0x00105b54:
        if ((sVar1 == 2) && (*psVar3 == 0x25)) {
          *(undefined4 *)(param_2 + 0x3cc) = 0;
          printk(&DAT_001014d8,*(undefined4 *)(psVar3 + 2),*(undefined4 *)(psVar3 + 4),
                 *(undefined4 *)(psVar3 + 6),*(undefined4 *)(psVar3 + 8));
          aud_smsg_all_ch_notify(param_2,psVar3);
          **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
          agdsp_access_dumpreg();
          aud_smsg_dump_func(0);
          **(undefined4 **)(param_2 + 0x28) = 0;
          **(undefined4 **)(param_2 + 0x20) = 0;
          **(undefined4 **)(param_2 + 0x48) = 0;
          **(undefined4 **)(param_2 + 0x40) = 0;
        }
        else {
          if (*(int *)(param_2 + 0x3cc) != 0) {
            aud_smsg_ch_send(param_2,psVar3);
          }
          **(int **)(param_2 + 0x40) = **(int **)(param_2 + 0x40) + 1;
        }
      }
    } while (**(int **)(param_2 + 0x48) != **(int **)(param_2 + 0x40));
  }
  _raw_spin_unlock_irqrestore(param_2 + 0x6c,uVar2);
  return 1;
}



