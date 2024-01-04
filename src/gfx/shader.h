#ifndef SHADER_H
#define SHADER_H

#include "gl.h"
#include "math/matrix.h"

typedef struct
{
	GLuint id;
} Shader;

Shader shader_create(const char* path);
void shader_destroy(Shader shader);

void shader_use(const Shader shader);

void shader_set_int(const Shader shader, const char* name, GLint value);
void shader_set_vec2(const Shader shader, const char* name, float x, float y);
void shader_set_vec3(const Shader shader, const char* name, float x, float y, float z);
void shader_set_vec4(const Shader shader, const char* name, float x, float y, float z, float w);
void shader_set_mat3(const Shader shader, const char* name, const Mat3* mat3);
void shader_set_mat4(const Shader shader, const char* name, const Mat4* mat4);

#endif
