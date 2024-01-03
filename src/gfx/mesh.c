#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "gl.h"
#include "math/matrix.h"
#include "core/log.h"
#include "core/assert.h"

#include <stddef.h>

struct Mesh
{
	GLuint vao;
	GLuint vbo;
	Texture texture;
	Shader shader;
	MeshVertex* verts;
	size_t size;
	size_t capacity;
	Mat4 model_mat;
};

static GLuint create_vbo();

Mesh* mesh_create(MeshVertex* vertices, size_t num_verts, Texture texture, Shader shader)
{
	ASSERT(texture.id, "attempt to create mesh with null texture.");
	ASSERT(shader.id, "attempt to create mesh with null shader.");

	Mesh* mesh = calloc(1, sizeof(Mesh));
	if (!mesh)
	{
		LOG_ERROR("failed to create mesh: out of memory.");
		return NULL;
	}

	// create vao.
	GLuint vao;
	GL_CMD(glGenVertexArrays(1, &vao));
	mesh->vao = vao;

	GL_CMD(glBindVertexArray(vao));

	// create vbo.
	GLuint vbo = create_vbo(vertices, num_verts);
	mesh->vbo = vbo;

	GL_CMD(glBindVertexArray(0));

	mesh->texture = texture;
	mesh->shader = shader;

	shader_use(shader);
	shader_set_int(shader, "textureImage", 0);

	Mat4 model_mat = MAT4_IDENTITY;
	model_mat.z = -6.0f;
	Mat4 projection = mat4_perspective(70.0, 1.6, 0.1, 20.0); // FIXME: shouldn't be here.
	shader_set_mat4(shader, "model", &model_mat);
	shader_set_mat4(shader, "view", &MAT4_IDENTITY);
	shader_set_mat4(shader, "projection", &projection);
	mesh->model_mat = model_mat;

/*
	// create vertex array.
	MeshVertex* verts = malloc(capacity * sizeof(MeshVertex));
	if (!verts)
	{
		LOG_ERROR("failed to create mesh: out of memory.");
		mesh_destroy(mesh);
		return NULL;
	}
	mesh->verts = verts;
*/
	mesh->verts = NULL;
	mesh->size = num_verts;
	mesh->capacity = 0;

	return mesh;
}

void mesh_destroy(Mesh* mesh)
{
	ASSERT(mesh, "attempt to destroy null mesh.");

	if (mesh->verts)
		free(mesh->verts);

	if (mesh->vbo)
		GL_CMD(glDeleteBuffers(1, &mesh->vbo));

	if (mesh->vao)
		GL_CMD(glDeleteVertexArrays(1, &mesh->vao));
		
	free(mesh);
}
/*
void mesh_set_vertices(MeshVertex* vertices, size_t num_vertices)
{
	// TODO
}

void mesh_add_quad(float x, float y, float z, float nx, float ny, float nz)
{

}
*/

Shader mesh_get_shader(const Mesh* mesh)
{
	return mesh->shader;
}

void mesh_draw(Mesh* mesh)
{
	GL_CMD(glBindVertexArray(mesh->vao));

	GL_CMD(glActiveTexture(GL_TEXTURE0));
	texture_bind(mesh->texture);
	shader_use(mesh->shader);

	//mat4_rotate_y(&mesh->model_mat, 0.0006);
	//shader_set_mat4(mesh->shader, "model", &mesh->model_mat);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->size);

	GL_CMD(glBindVertexArray(0));
}

static GLuint create_vbo(MeshVertex* verts, size_t num_verts)
{
	GLuint vbo;
	GL_CMD(glGenBuffers(1, &vbo));
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	
	// position
	GL_CMD(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(MeshVertex), (const void*)offsetof(MeshVertex, x)));
	GL_CMD(glEnableVertexAttribArray(0));
	
	// texture coords
	GL_CMD(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(MeshVertex), (const void*)offsetof(MeshVertex, u)));
	GL_CMD(glEnableVertexAttribArray(1));

	// allocate vertices.
	GL_CMD(glBufferData(GL_ARRAY_BUFFER,
			(GLsizeiptr)(num_verts * sizeof(MeshVertex)), verts, GL_STATIC_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return vbo;
}