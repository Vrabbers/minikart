
#include <gba.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "kart_img_bin.h"
#include "sprites.h"
#include "lut_math.h"
#include "ftypes.h"

vu32 frame_counter;

vu32 keys;

struct kart 
{
    f24_8 x;
    f24_8 y;
    u16 theta;
    f8_8 theta_vel;
    f24_8 vel;
    u32 timer;
};

struct kart karts[8];

void vblank(void);
void game(void);

int main(void) 
{
    memcpy(OBJ_BASE_ADR, kart_img_bin, kart_img_bin_size);
    copy_kart_pals();

    BG_COLORS[0] = 0x764C;
    irqInit();
    irqEnable(IRQ_VBLANK);
    clear_shoam();
    VBlankIntrWait();

    REG_DISPCNT = BG_ALL_ENABLE | OBJ_1D_MAP | OBJ_ON;

    for (int i = 0; i < 8; i++)
    {
        struct kart k = 
            {
                i << 12,
                i << 12,
                rand() & 0xFFFF,
                0,
                F24_8(0.0),
                rand() & 0x1A
            };
        karts[i] = k;
    }

    while(true)
    {
        game();
        VBlankIntrWait();
        vblank();
    }
}

void game(void)
{
    clear_shoam();

    keys = ~REG_KEYINPUT;

    if (keys & KEY_LEFT)
        karts[0].theta_vel = F8_8(5.0);
    else if (keys & KEY_RIGHT)
        karts[0].theta_vel = F8_8(-5.0);
    else 
        karts[0].theta_vel = 0;

    if (keys & KEY_A)
        karts[0].vel = F24_8(5.0);
    
    for (int i = 1; i < 8; i++)
    {
        struct kart* k = &karts[i];

        if (k->timer == 0)
        {
            k->vel = rand() & 0x3ff;
            u16 theta_vel = rand() & 0x7ff;
            k->theta_vel = (rand() & 1) ? theta_vel : -theta_vel;
            k->timer = rand() & 0x1F;
        }
        else 
        {
            k->timer--;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        struct kart* k = &karts[i];
        f24_8 dx = (lut_cos(k->theta) * k->vel) >> 8;
        f24_8 dy = (lut_sin(k->theta) * k->vel) >> 8;
        k->x = clamp(F24_8(0.0), k->x + dx, F24_8(SCREEN_WIDTH));
        k->y = clamp(F24_8(0.0), k->y - dy, F24_8(SCREEN_HEIGHT));
        k->theta += k->theta_vel;
        k->vel = (k->vel * F24_8(95/100.0)) >> 8;

        OBJATTR* obj = give_shoam_slot();
        ShoamAffinePtr affine_ptr;
        u32 affine_ix = give_shoam_affine_slot(&affine_ptr);
        obj->attr0 = OBJ_Y((k->y >> 8) - 8) | ATTR0_ROTSCALE | ATTR0_SQUARE;
        obj->attr1 = OBJ_X((k->x >> 8) - 8) | ATTR1_SIZE_16 | ATTR1_ROTDATA(affine_ix);
        obj->attr2 = ATTR2_PALETTE(i);
        ObjAffineSource affine_source = { F8_8(1.0), F8_8(1.0), k->theta };
        ObjAffineSet(&affine_source, affine_ptr, 1, 8);
    }
}

void vblank(void)
{
    frame_counter++;
    copy_shoam_to_oam();
}





