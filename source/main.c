
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

f24_8 x;
f24_8 y;

u16 theta;
f24_8 vel;

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
    if (keys & KEY_A)
        vel += 0x0100;
    
    vel = (vel * 0x0040 * 3) >> 8;
    
    if (keys & KEY_LEFT)
        theta += 0x0280;
    if (keys & KEY_RIGHT)
        theta -= 0x0280;

    x -= (vel * lut_sin(theta)) >> 8;
    y -= (vel * lut_cos(theta)) >> 8;

    if ((x >> 8) > SCREEN_WIDTH + 16)
        x = (-15) << 8;
    if ((y >> 8) > SCREEN_HEIGHT + 16)
        y = (-15) << 8;
    OBJATTR* obj = give_shoam_slot();
    obj->attr0 = OBJ_Y(y >> 8) | ATTR0_ROTSCALE | ATTR0_SQUARE;
    obj->attr1 = OBJ_X(x >> 8) | ATTR1_SIZE_16;

    ShoamAffinePtr affine;
    give_shoam_affine_slot(&affine);
    ObjAffineSource aff[1] = {{0x100, 0x100, theta}};
    ObjAffineSet(aff, affine, 1, 8);
}

void vblank(void)
{
    frame_counter++;
    copy_shoam_to_oam();
}





