// kernel_vser_set_pass_mode @ 00101884
// signature: undefined kernel_vser_set_pass_mode(void)
// calling convention: unknown


void kernel_vser_set_pass_mode(byte param_1)

{
  s_in_bypass_mode = param_1 & 1;
  return;
}


