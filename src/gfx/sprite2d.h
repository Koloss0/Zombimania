#ifndef SPRITE2D_H
#define SPRITE2D_H

#include "texture_rect.h"

#include <stdbool.h>

#define SPRITES_MAX 2000

bool sprite2d_init();
void sprite2d_shutdown();

void sprite2d_begin();
void sprite2d_end();
void sprite2d_flush();

void sprite2d_draw(float x, float y, float width, float height, TextureRect texture_rect);

#endif
