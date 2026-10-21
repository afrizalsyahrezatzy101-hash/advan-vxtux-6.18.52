====================get_ip_index_from_table.849@00112a38====================
// signature: undefined get_ip_index_from_table.849(void)

void get_ip_index_from_table_849(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_001043cc < param_2) {
    if (DAT_00104404 < param_2) {
      if (DAT_0010443c < param_2) {
        if (DAT_00104474 < param_2) {
          if (DAT_001044ac < param_2) {
            if (DAT_001044e4 < param_2) {
              uVar1 = 6;
              if (DAT_0010451c < param_2) {
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


