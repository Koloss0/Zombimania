#ifndef VBO_H
#define VBO_H

#include "gl.h"

typedef struct
{
	GLuint id;
} VBO;

VBO vbo_create();
void vbo_destroy(VBO vbo);

#endif