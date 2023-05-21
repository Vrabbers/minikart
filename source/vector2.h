#pragma once

#include "ftypes.h"

typedef struct 
{
    f24_8 x;
    f24_8 y;
} Vector2;

#define V2_8(x, y) ((Vector2) {F24_8(x), F24_8(y)})

inline Vector2 v2add(Vector2 a, Vector2 b)
{
    return (Vector2) {a.x + b.x, a.y + b.y};
}

inline Vector2 v2mul(Vector2 a, f24_8 s)
{
    return (Vector2) {fmul(s, a.x), fmul(s, a.y)};
}

inline Vector2 v2neg(Vector2 a)
{
    return (Vector2) {-a.x, -a.y};
}

inline f24_8 len_squared(Vector2 a)
{
    return fmul(a.x, a.x) + fmul(a.y, a.y);
}

inline f24_8 v2dot(Vector2 a, Vector2 b)
{
    return fmul(a.x, b.x) + fmul(a.y, b.y);
}
