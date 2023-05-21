
#include <gba.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "kart_img_bin.h"
#include "sprites.h"
#include "lut_math.h"
#include "ftypes.h"
#include "vector2.h"


vu32 frame_counter;

vu32 keys;

struct kart 
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    u16 theta;
    f8_8 theta_vel;
};

struct kart karts[8];

void vblank(void);
void game(void);

int main(void) 
{
    memcpy(OBJ_BASE_ADR, kart_img_bin, kart_img_bin_size);
    copy_kart_pals();

    //consoleDemoInit();

    BG_COLORS[0] = 0x764C;
    irqInit();
    irqEnable(IRQ_VBLANK);
    clear_shoam();
    VBlankIntrWait();

    REG_DISPCNT = OBJ_1D_MAP | OBJ_ON;

    for (int i = 0; i < 8; i++)
    {
        struct kart k = 
            {
                v2mul(V2_8(1.0, 1.0), i << 14),
                V2_8(0, 0),
                V2_8(0, 0),
                0,
                0
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

void random_karts(void)
{
    return;
}

void update_karts(void)
{
    for (int i = 0; i < 8; i++)
    {
        struct kart* k = &karts[i];

        Vector2 dir_vec = {lut_cos(k->theta), -lut_sin(k->theta)};
        Vector2 ortho_vec = {-dir_vec.y, dir_vec.x};

        f24_8 para_friction = fmul(v2dot(k->velocity, dir_vec), F24_8(-0.05));
        f24_8 ortho_friction = fmul(v2dot(k->velocity, ortho_vec), F24_8(-0.1));

        k->acceleration = v2add(k->acceleration, v2mul(dir_vec, para_friction));
        k->acceleration = v2add(k->acceleration, v2mul(ortho_vec, ortho_friction));


        k->velocity = v2add(k->velocity, k->acceleration);
        k->position = v2add(k->position, k->velocity);

        k->theta += k->theta_vel;

        k->acceleration = V2_8(0.0, 0.0);
    }
}

void render_karts(void)
{
    struct kart me_kart = karts[0];
    //render kart #0 (the player kart)
    OBJATTR* me = give_shoam_slot();
    ShoamAffinePtr affine_ptr;
    u32 affine_ix = give_shoam_affine_slot(&affine_ptr);
    ObjAffineSource affine_source = { F8_8(1.0), F8_8(1.0), me_kart.theta };
    ObjAffineSet(&affine_source, affine_ptr, 1, 8);

    me->attr0 = OBJ_Y(72) | ATTR0_ROTSCALE | ATTR0_SQUARE;
    me->attr1 = OBJ_X(112) | ATTR1_SIZE_16 | ATTR1_ROTDATA(affine_ix);
    for (int i = 1; i < 8; i++)
    {
        struct kart* k = &karts[i];
        s32 x, y;
        x = ((k->position.x - me_kart.position.x) >> 8) + 118;
        y = ((k->position.y - me_kart.position.y) >> 8) + 72;

        if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT)
            continue;
        
        OBJATTR* obj = give_shoam_slot();
        affine_ix = give_shoam_affine_slot(&affine_ptr);
        obj->attr0 = OBJ_Y(y - 8) | ATTR0_ROTSCALE | ATTR0_SQUARE;
        obj->attr1 = OBJ_X(x - 8) | ATTR1_SIZE_16 | ATTR1_ROTDATA(affine_ix);
        obj->attr2 = ATTR2_PALETTE(i);
        affine_source.theta = k->theta ;
        ObjAffineSet(&affine_source, affine_ptr, 1, 8);
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
        karts[0].acceleration = (Vector2) {lut_cos(karts[0].theta) >> 1, -lut_sin(karts[0].theta) >> 1};

    random_karts();
    update_karts();
    render_karts();

}

void vblank(void)
{
    frame_counter++;
    copy_shoam_to_oam();
}





