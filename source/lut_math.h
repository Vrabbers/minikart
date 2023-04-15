#pragma once

#include "ftypes.h"

#define DEG(x) (x * 0x10000) / 360

f24_8 lut_sin(u16 theta);

inline f24_8 lut_cos(u16 theta)
{
    return lut_sin(theta + DEG(90));
}