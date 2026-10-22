// is_in_tui @ 0010106c
// signature: undefined is_in_tui(void)
// calling convention: unknown


bool is_in_tui(void)

{
  if (s_ts != 0) {
    return 0 < *(int *)(s_ts + 4);
  }
  return false;
}


