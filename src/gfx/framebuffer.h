#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gl.h"
#include "texture.h"

typedef struct
{
	GLuint id;
} Framebuffer;

Framebuffer framebuffer_create(Texture texture);
void framebuffer_destroy(Framebuffer framebuffer);
void framebuffer_bind(Framebuffer framebuffer);
void framebuffer_unbind();

#endif