#pragma once

#include <gba_types.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

inline s32 clamp(s32 lower, s32 value, s32 upper)
{
    return MIN(upper, MAX(lower, value));
}

typedef s16 f8_8;
#define F8_8(n) ((f8_8) ((n) * 256.0))

typedef s32 f24_8;
#define F24_8(n) ((f24_8) ((n) * 256.0))

inline f24_8 fmul(f24_8 a, f24_8 b)
{
    return  (a * b) >> 8;
}