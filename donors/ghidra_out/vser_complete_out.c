// vser_complete_out @ 001023dc
// signature: undefined vser_complete_out(void)
// calling convention: unknown


void vser_complete_out(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = _vser_dev;
  *(undefined4 *)(_vser_dev + 0x170) = 1;
  if (*(int *)(param_2 + 0x54) != 0) {
    *(undefined4 *)(lVar1 + 0xfc) = 1;
  }
  __wake_up(lVar1 + 0x138,3,1,0);
  return;
}


