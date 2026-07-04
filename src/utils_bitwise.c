i32 flags_combine(i32 count, const i32 *flags) {
  i32 res = 0;
  for (i32 i = 0; i < count; i++) {
    res |= flags[i];
  }
  return res;
}
