#ifndef SPRITE3D_H
#define SPRITE3D_H

#include "texture_rect.h"
#include "camera.h"

#include <stdbool.h>

#define SPRITE3D_MAX_SPRITES 2000

bool sprite3d_init(unsigned int viewport_width, unsigned int viewport_height);
void sprite3d_shutdown();

void sprite3d_begin(const Camera* camera);
void sprite3d_end();
void sprite3d_flush();

void sprite3d_draw(double x, double y, double z, double width, double height, TextureRect texture_rect);

#endif
