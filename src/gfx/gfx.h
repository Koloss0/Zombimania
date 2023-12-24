#ifndef GFX_H
#define GFX_H

#include "gl.h"
#include "camera.h"

#include <stdbool.h>

typedef struct
{
	int x, y, w, h;
} TextureRect;

bool gfx_init();
void gfx_shutdown();

void gfx_begin(Camera* camera);
void gfx_end();
void gfx_flush();

void gfx_background(float r, float g, float b);
void gfx_sprite_2d(float x, float y, float width, float height, TextureRect texture_rect);

#endif