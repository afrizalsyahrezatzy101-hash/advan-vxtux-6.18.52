// sprd_pcm_dma_step_config @ 0010467c
// signature: undefined sprd_pcm_dma_step_config(void)
// calling convention: unknown


void sprd_pcm_dma_step_config(long param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  get_sp_audio_debug_flag();
  if (param_3 == 0) {
    uVar4 = 4;
    if (*(int *)(param_1 + 0x98) != 1) {
      uVar4 = 2;
    }
    uVar1 = uVar4;
    uVar2 = 0;
    if (param_2 != 0) {
      uVar1 = 0;
      uVar2 = uVar4;
    }
    *(undefined4 *)(param_1 + 0x78) = uVar2;
    *(undefined4 *)(param_1 + 0x7c) = uVar1;
    printk(&DAT_001003e4);
  }
  else {
    iVar3 = *(int *)(param_1 + 0x74);
    if (iVar3 == 2) {
      uVar4 = 4;
    }
    else if (iVar3 == 4) {
      uVar4 = 8;
    }
    else {
      printk(&DAT_001004cb,iVar3);
      iVar3 = *(int *)(param_1 + 0x74);
      uVar4 = 0;
    }
    uVar1 = uVar4;
    uVar2 = 0;
    if (param_2 != 0) {
      uVar1 = 0;
      uVar2 = uVar4;
    }
    *(undefined4 *)(param_1 + 0x78) = uVar2;
    *(undefined4 *)(param_1 + 0x7c) = uVar1;
    printk(&DAT_00101525,uVar2,uVar1,iVar3);
  }
  return;
}


