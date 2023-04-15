#pragma once

#include <gba_sprites.h>

typedef s16* ShoamAffinePtr;

void copy_kart_pals(void);

void clear_shoam(void);

void copy_shoam_to_oam(void);

OBJATTR* give_shoam_slot(void);

u32 give_shoam_affine_slot(ShoamAffinePtr* ptr);
