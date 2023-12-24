#ifndef SPRITE2D_H
#define SPRITE2D_H

#include "shader.h"
#include "texture_rect.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
	float x, y, width, height;
} Sprite2D;

#define SPRITES_MAX 2000

typedef struct
{
	Sprite2D sprites[SPRITES_MAX];
	size_t sprites_size;

	GLuint vao, quad_vbo, sprites_vbo;
	Shader* shader;
} Sprite2DState;

bool sprite2d_init(Sprite2DState* s);
void sprite2d_shutdown(Sprite2DState* s);

void sprite2d_begin(Sprite2DState* s);
void sprite2d_end(Sprite2DState* s);
void sprite2d_flush(Sprite2DState* s);

void sprite2d_draw(Sprite2DState* s, float x, float y, float width, float height, TextureRect texture_rect);

#endif
