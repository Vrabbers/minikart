#include <string.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include "kart.h"
#include "kart_pal_bin.h"
#include "kart_img_bin.h"
#include "sprites.h"

u32 shoam_index = 0;
u32 affine_index = 0;

OBJATTR shadow_oam[128];

inline u16 RGB5_mul(u16 c1, u16 c2)
{
    int r = ((c1 & 0x1f) * (c2 & 0x1f)) >> 5;
    int g = (((c1 >> 5) & 0x1f) * ((c2 >> 5) & 0x1f)) >> 5;
    int b = ((c1 >> 10)) * ((c2 >> 10)) >> 5;
    return RGB5(r, g, b);
}

void copy_kart_pals(void)   
{
    const u16* pal_base = (u16*) kart_pal_bin;
    u16 buffer[16];
    for (int c = 0; c < KART_COLORS; c++)
    {
        memcpy(buffer, pal_base, sizeof(buffer));
        buffer[1] = RGB5_mul(buffer[1], kart_RGB5s[c]);
        buffer[2] = RGB5_mul(buffer[2], kart_RGB5s[c]);
        memcpy(&OBJ_COLORS[c * 16], buffer, sizeof(buffer));
    }
}

OBJATTR* give_shoam_slot(void)
{
    OBJATTR* addr = &shadow_oam[shoam_index];
    shoam_index++;
    return addr;
}

u32 give_shoam_affine_slot(ShoamAffinePtr* ptr)
{
    u32 ix = affine_index;
    affine_index++;
    *ptr = (ShoamAffinePtr) &shadow_oam[ix << 2].dummy;
    return ix;
}

void clear_shoam(void)
{
    OBJATTR cleared = { OBJ_DISABLE, 0, 0, 0 };
    for (int i = 0; i < 128; i++) 
        shadow_oam[i] = cleared;
    shoam_index = 0;
    affine_index = 0;
}

void copy_shoam_to_oam(void)
{
    CpuFastSet(shadow_oam, OAM, (128 * sizeof(OBJATTR)) / 4);
}