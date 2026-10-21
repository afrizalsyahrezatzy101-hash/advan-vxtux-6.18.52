// trusty_irq_cpu_up @ 00101030
// signature: undefined trusty_irq_cpu_up(void)
// calling convention: unknown


undefined8 trusty_irq_cpu_up(undefined4 param_1,long param_2,undefined8 param_3,undefined8 param_4)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = daif;
  if ((uVar1 & 0x80) == 0) {
    uVar2 = daif;
    daif = uVar2 | 0x80;
  }
  lVar3 = tpidr_el1;
  trusty_irq_enable_irqset
            (param_2 + -0x48,lVar3 + *(long *)(param_2 + -0x20),param_3,param_4,param_1);
  daif = uVar1;
  return 0;
}


