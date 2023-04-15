#include "lut_math.h"
#include "sin_lut_bin.h"

const f8_8* sin_lut = (f8_8*) &sin_lut_bin[0];

f24_8 lut_sin(u16 theta)
{
    if (theta == DEG(90))
        return 0x0100;
    if (theta == DEG(270))
        return -0x0100;
    u32 theta_bits = (theta >> 7) & 0x7f;

    u32 ix = (theta & 0x4000) ? (127 - theta_bits) : theta_bits;
    f24_8 val = sin_lut[ix];
    if (theta & 0x8000)
        val = -val;
    return val;
}