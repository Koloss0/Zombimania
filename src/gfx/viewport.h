#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "gl.h"
#include "framebuffer.h"
#include "texture.h"
#include "shader.h"

typedef struct
{
	unsigned long width;
	unsigned long height;
	Framebuffer framebuffer;
	Texture texture;
	Shader shader;
	GLuint quad_vbo;
	GLuint quad_vao;
} Viewport;

Viewport* viewport_create(unsigned long width, unsigned long height);
void viewport_destroy(Viewport* viewport);
void viewport_bind(Viewport* viewport);
void viewport_unbind();
void viewport_draw(Viewport* viewport);

#endif