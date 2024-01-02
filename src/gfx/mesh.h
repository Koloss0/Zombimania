#ifndef MESH_H
#define MESH_H

#include "texture.h"
#include "shader.h"
#include "math/matrix.h"

#include <stddef.h>

typedef struct Mesh Mesh;

typedef struct
{
	float x, y, z;
	float u, v;
} MeshVertex;

Mesh* mesh_create(MeshVertex* vertices, size_t num_verts, Texture texture, Shader shader);
void mesh_destroy(Mesh* mesh);

Shader mesh_get_shader(const Mesh* mesh);

/*
void mesh_set_vertices(MeshVertex* vertices, size_t num_vertices);
void mesh_add_quad(float x, float y, float z, float nx, float ny, float nz);
*/

void mesh_draw(Mesh* mesh);

#endif