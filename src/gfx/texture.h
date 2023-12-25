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

typedef struct Texture Texture;

Texture* texture_create();
void texture_destroy(Texture* texture);

void texture_set_image(Texture* texture, Image image, const TexParameters* params);
void texture_bind(Texture* texture);

unsigned long texture_get_width(const Texture* texture);
unsigned long texture_get_height(const Texture* texture);

#endif