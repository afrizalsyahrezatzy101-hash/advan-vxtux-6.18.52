====================get_ip_index_from_table.928@00113ad0====================
// signature: undefined get_ip_index_from_table.928(void)

void get_ip_index_from_table_928(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_00103ccc < param_2) {
    if (DAT_00103d04 < param_2) {
      if (DAT_00103d3c < param_2) {
        if (DAT_00103d74 < param_2) {
          if (DAT_00103dac < param_2) {
            if (DAT_00103de4 < param_2) {
              uVar1 = 6;
              if (DAT_00103e1c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  *param_3 = uVar1;
  return;
}


