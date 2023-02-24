#ifndef NDG_UTILS_H_
#define NDG_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif  // !__cplusplus

#define IS_BETWEEN(a, b, x) (x >= a && x <= b)
#define IS_XBETWEEN(a, b, x) (x > a && x < b)
#define TO_LOWER(x) (IS_BETWEEN('A', 'Z', x)? (x | 0x20): x)
#define IS_ALPHA(x) (IS_BETWEEN('a', 'z', TO_LOWER(x)))
#define IS_NUMERIC(x) (IS_BETWEEN('0', '9', x))

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !NDG_UTILS_H_
