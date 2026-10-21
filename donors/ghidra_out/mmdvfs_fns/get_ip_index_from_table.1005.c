====================get_ip_index_from_table.1005@00114b54====================
// signature: undefined get_ip_index_from_table.1005(void)

void get_ip_index_from_table_1005(undefined8 param_1,ulong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *param_3 = 0;
  uVar1 = 0;
  if (DAT_0010420c < param_2) {
    if (DAT_00104244 < param_2) {
      if (DAT_0010427c < param_2) {
        if (DAT_001042b4 < param_2) {
          if (DAT_001042ec < param_2) {
            if (DAT_00104324 < param_2) {
              uVar1 = 6;
              if (DAT_0010435c < param_2) {
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


