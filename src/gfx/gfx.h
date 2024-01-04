#ifndef GFX_H
#define GFX_H

#include "gl.h"
#include "camera.h"
#include "texture_rect.h"
#include "mesh.h"

#include <stdbool.h>

bool gfx_init(unsigned int viewport_width, unsigned int viewport_height);
void gfx_shutdown();

void gfx_fit_viewport(int width, int height);
bool gfx_screen_pos_to_viewport_pos(double* x, double* y);

void gfx_begin(const Camera* camera);
void gfx_end();
void gfx_flush();

void gfx_background(float r, float g, float b);
void gfx_sprite2d(int x, int y, int width, int height, TextureRect texture_rect);
void gfx_sprite3d(double x, double y, double z, double width, double height, TextureRect texture_rect);
void gfx_mesh(Mesh* mesh);

#endif