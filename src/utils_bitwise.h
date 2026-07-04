#ifndef UTILS_BITWISE_H
#define UTILS_BITWISE_H

i32 flags_combine(i32 count, const i32 *flags);

#define FLAGS_COMBINE(...)                                                     \
  flags_combine(sizeof((i32[]){__VA_ARGS__}) / sizeof(i32),                    \
                (i32[]){__VA_ARGS__})

#define FLAG_ADD(current, flag) ((current) | (flag))
#define FLAG_REMOVE(current, flag) ((current) & ~(flag))
#define FLAG_CONTAINS(current, flag) (((current) & (flag)) == (flag))

#endif
