#ifndef GFX_H
#define GFX_H

#include "gl.h"
#include "camera.h"
#include "texture_rect.h"

#include <stdbool.h>

bool gfx_init(int viewport_width, int viewport_height);
void gfx_shutdown();

void gfx_begin(const Camera* camera);
void gfx_end();
void gfx_flush();

void gfx_background(float r, float g, float b);
void gfx_sprite2d(float x, float y, float width, float height, TextureRect texture_rect);

#endif