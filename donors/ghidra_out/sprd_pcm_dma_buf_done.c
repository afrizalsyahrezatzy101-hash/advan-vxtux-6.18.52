// sprd_pcm_dma_buf_done @ 00104ca8
// signature: undefined sprd_pcm_dma_buf_done(void)
// calling convention: unknown


void sprd_pcm_dma_buf_done(long *param_1)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(*param_1 + 0x140) + 0x150);
  if (*(int *)(lVar3 + 0x90) == 0) {
    lVar2 = *(long *)(*param_1 + 0x10);
    *(undefined4 *)(lVar3 + 0x90) = 1;
    printk(&DAT_0010132a,*(undefined4 *)(*(long *)(lVar2 + 0x5f8) + 8));
  }
  if (*(int *)(lVar3 + 0x78) == 1) {
LAB_00104dd0:
    snd_pcm_period_elapsed(*param_1);
  }
  else {
    iVar1 = *(int *)(*(long *)(*(long *)(*param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar1 == 0xb) || (iVar1 == 0)) && (*(int *)(*param_1 + 0x3c) == 0)) {
      _raw_spin_lock(pm_dma + 0x28);
    }
    if (param_1[1] == *(long *)(lVar3 + 0x10)) {
      *(undefined4 *)(lVar3 + 0x6c) = 1;
      if ((*(long *)(lVar3 + 0x18) == 0) || (*(int *)(lVar3 + 0x70) != 0)) {
LAB_00104d9c:
        iVar1 = *(int *)(*(long *)(*(long *)(*param_1 + 0x10) + 0x5f8) + 8);
        if (((iVar1 == 0xb) || (iVar1 == 0)) && (*(int *)(*param_1 + 0x3c) == 0)) {
          _raw_spin_unlock(pm_dma + 0x28);
        }
        *(undefined8 *)(lVar3 + 0x6c) = 0;
        goto LAB_00104dd0;
      }
      if (param_1[1] == *(long *)(lVar3 + 0x18)) goto LAB_00104d88;
    }
    else if (param_1[1] == *(long *)(lVar3 + 0x18)) {
LAB_00104d88:
      *(undefined4 *)(lVar3 + 0x70) = 1;
      if ((*(long *)(lVar3 + 0x10) == 0) || (*(int *)(lVar3 + 0x6c) != 0)) goto LAB_00104d9c;
    }
    iVar1 = *(int *)(*(long *)(*(long *)(*param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar1 == 0) || (iVar1 == 0xb)) && (*(int *)(*param_1 + 0x3c) == 0)) {
      _raw_spin_unlock(pm_dma + 0x28);
    }
  }
  return;
}


