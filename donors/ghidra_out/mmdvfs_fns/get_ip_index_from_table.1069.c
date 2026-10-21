====================get_ip_index_from_table.1069@00115c4c====================
// signature: undefined get_ip_index_from_table.1069(void)

void get_ip_index_from_table_1069(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_0010394c < param_2) {
    if (DAT_00103984 < param_2) {
      if (DAT_001039bc < param_2) {
        if (DAT_001039f4 < param_2) {
          if (DAT_00103a2c < param_2) {
            if (DAT_00103a64 < param_2) {
              uVar1 = 6;
              if (DAT_00103a9c < param_2) {
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


