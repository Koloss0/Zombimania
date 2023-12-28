#ifndef TEXTURE_H
#define TEXTURE_H

#include "io/image.h"
#include "gl.h"

typedef struct
{
	GLint internal_format;
	GLuint source_format;
	GLint wrap_s, wrap_t;
	GLint filter_min, filter_mag;
} TexParameters;

typedef struct
{
	GLuint id;
	unsigned long width;
	unsigned long height;
} Texture;

Texture texture_create(Image image, const TexParameters* params);
void texture_destroy(Texture texture);

void texture_bind(Texture texture);

#endif