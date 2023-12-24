#include "sprite2d.h"
#include "shader.h"
#include "texture_rect.h"
#include "core/log.h"

#include "gl.h"

#include <stdlib.h>
#include <stdbool.h>

// TODO:
// [ ] add colour attribute?

typedef struct
{
	float x, y;
	float u, v;
} Vertex;

static GLuint create_quad_vbo();
static GLuint create_sprites_vbo();

bool sprite2d_init(Sprite2DState* s)
{
	s->sprites_size = 0;

	// create vao
	GL_CMD(glGenVertexArrays(1, &s->vao));
	GL_CMD(glBindVertexArray(s->vao));

	// create quad vbo
	s->quad_vbo = create_quad_vbo();

	// create sprites vbo
	s->sprites_vbo = create_sprites_vbo();
	
	GL_CMD(glBindVertexArray(0));

	s->shader = shader_create("res/shaders/sprite2d.shader");
	if (!s->shader)
	{
		LOG_ERROR("failed to initialise Sprite2D: could not create shader.");
		return false;
	}

	shader_use(s->shader);

	//shader_set_sampler2d("textureImage", GL_TEXTURE0);
	shader_set_mat3(s->shader, "mvp", &MAT3_IDENTITY);

	return s;
}

void sprite2d_shutdown(Sprite2DState* s)
{
	GL_CMD(glDeleteBuffers(1, &s->quad_vbo));
	GL_CMD(glDeleteVertexArrays(1, &s->vao));

	shader_destroy(s->shader);
}

void sprite2d_draw(Sprite2DState* s, float x, float y, float width, float height, TextureRect texture_rect)
{
	if (s->sprites_size < SPRITES_MAX)
	{
		Sprite2D sprite = {
			.x = x,
			.y = y,
			.width = width,
			.height = height
		};

		s->sprites[s->sprites_size] = sprite;
		s->sprites_size++;
	}
	else
	{
		LOG_WARN("[GFX] maximum number of sprites reached. (%d)", SPRITES_MAX);
	}
}

void sprite2d_begin(Sprite2DState* s)
{
}

void sprite2d_end(Sprite2DState* s)
{
	sprite2d_flush(s);
}

void sprite2d_flush(Sprite2DState* s)
{
	GL_CMD(glBindVertexArray(s->vao));

	// TODO: bind texture here
	
	shader_use(s->shader);

	// update sprite buffer
	glBindBuffer(GL_ARRAY_BUFFER, s->sprites_vbo);
	glBufferData(GL_ARRAY_BUFFER, SPRITES_MAX * sizeof(Sprite2D), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(s->sprites_size * sizeof(Sprite2D)), s->sprites);

	// instanced draw call
	GL_CMD(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)s->sprites_size));

	// clear sprite array
	s->sprites_size = 0;
}

static GLuint create_quad_vbo()
{
	// 1x1 quad
	const Vertex quad[] = {
		{ -0.5f, -0.5f, 0.0f, 0.0f },
		{  0.5f, -0.5f, 1.0f, 0.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 1.0f, 1.0f }
	};

	GLuint quad_vbo;
	GL_CMD(glGenBuffers(1, &quad_vbo));
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, quad_vbo));
	
	// position
	GL_CMD(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x)));
	GL_CMD(glEnableVertexAttribArray(0));
	
	// texture coords
	GL_CMD(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u)));
	GL_CMD(glEnableVertexAttribArray(1));

	// pass in quad vertices
	GL_CMD(glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return quad_vbo;
}

static GLuint create_sprites_vbo()
{
	GLuint sprites_vbo;
	GL_CMD(glGenBuffers(1, &sprites_vbo));
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, sprites_vbo));

	// offsets
	GL_CMD(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite2D), (const void*)offsetof(Sprite2D, x)));
	GL_CMD(glEnableVertexAttribArray(2));
	GL_CMD(glVertexAttribDivisor(2, 1)); // configure as per instance
	
	// sizes
	GL_CMD(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite2D), (const void*)offsetof(Sprite2D, width)));
	GL_CMD(glEnableVertexAttribArray(3));
	GL_CMD(glVertexAttribDivisor(3, 1)); // configure as per instance
	
	// initialise with NULL
	GL_CMD(glBufferData(GL_ARRAY_BUFFER, SPRITES_MAX * sizeof(Sprite2D), NULL, GL_STREAM_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return sprites_vbo;
}
