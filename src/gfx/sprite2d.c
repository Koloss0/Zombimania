#include "sprite2d.h"
#include "shader.h"
#include "texture.h"
#include "texture_rect.h"
#include "io/bmp.h"
#include "math/matrix.h"
#include "core/log.h"
#include "core/assert.h"
#include "gl.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	float x, y;
	float u, v;
} Vertex;

typedef struct
{
	float x, y, width, height;
	float uv_x, uv_y, uv_scale_x, uv_scale_y;
} Sprite2D;

static struct
{
	Sprite2D sprites[SPRITES_MAX];
	size_t sprites_size;

	GLuint vao, quad_vbo, sprites_vbo;
	Shader shader;
	Texture texture_atlas;
} state;

static GLuint create_quad_vbo();
static GLuint create_sprites_vbo();

bool sprite2d_init(unsigned long viewport_width, unsigned long viewport_height)
{
	state.sprites_size = 0;

	// create vao
	GL_CMD(glGenVertexArrays(1, &state.vao));
	GL_CMD(glBindVertexArray(state.vao));

	state.quad_vbo = create_quad_vbo();
	state.sprites_vbo = create_sprites_vbo();
	
	GL_CMD(glBindVertexArray(0));

	state.shader = shader_create("res/shaders/sprite2d.shader");
	if (state.shader.id == 0)
	{
		LOG_ERROR("failed to initialise Sprite2D: could not create shader.");
		return false;
	}

	Image texture_image = io_load_bmp("res/images/sprite2d.bmp", false);
	if (!texture_image.data)
	{
		LOG_ERROR("failed to initialise Sprite2D: could not load texture atlas.");
		return false;
	}

	state.texture_atlas = texture_create(texture_image, NULL);
	ASSERT(state.texture_atlas.id > 0,
			"failed to initialise Sprite2D: failed to create texture.");

	shader_use(state.shader);
	shader_set_int(state.shader, "textureImage", 0);

	const Mat4 mvp = mat4_ortho(0.0f, 0.0f, (float)viewport_width, (float)viewport_height);
	shader_set_mat4(state.shader, "mvp", &mvp);

	return true;
}

void sprite2d_shutdown()
{
	GL_CMD(glDeleteBuffers(1, &state.quad_vbo));
	GL_CMD(glDeleteVertexArrays(1, &state.vao));

	shader_destroy(state.shader);
}

void sprite2d_draw(int x, int y, int width, int height, TextureRect texture_rect)
{	
	if(width==0 && height==0)
	{
		width=texture_rect.w;
		height=texture_rect.h;
	}
	 else if(width==0)
	 {
		
		double prop = (double)texture_rect.w/(double)texture_rect.h;
		width=height*(int)prop;

	 }
	else if(height==0)
	{
		double prop = (double)texture_rect.w/(double)texture_rect.h;
		height=width/(int)prop;
		
	}
	

	if (state.sprites_size < SPRITES_MAX)
	{
		float tex_width = (float)state.texture_atlas.width;
		float tex_height = (float)state.texture_atlas.height;

		Sprite2D sprite = {
			.x =          (float) x,
			.y =          (float) y,
			.width =      (float)width,
			.height =     (float)height,
			.uv_x =       (float)texture_rect.x / tex_width,
			.uv_y =       (float)texture_rect.y / tex_height,
			.uv_scale_x = (float)texture_rect.w / tex_width,
			.uv_scale_y = (float)texture_rect.h / tex_height,
		};

		state.sprites[state.sprites_size] = sprite;
		state.sprites_size++;
	}
	else
	{
		LOG_WARN("[GFX] maximum number of sprites reached. (%d)", SPRITES_MAX);
	}
}

void sprite2d_begin()
{}

void sprite2d_end()
{
	sprite2d_flush();
}

void sprite2d_flush()
{
	GL_CMD(glBindVertexArray(state.vao));
	
	texture_bind(state.texture_atlas);	
	shader_use(state.shader);

	// update sprite buffer
	glBindBuffer(GL_ARRAY_BUFFER, state.sprites_vbo);
	glBufferData(GL_ARRAY_BUFFER,
			SPRITES_MAX * sizeof(Sprite2D), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
			(GLsizeiptr)(state.sprites_size * sizeof(Sprite2D)), state.sprites);

	// instanced draw call
	GL_CMD(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)state.sprites_size));

	// clear sprite array
	state.sprites_size = 0;
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
	GL_CMD(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, x)));
	GL_CMD(glEnableVertexAttribArray(0));
	
	// texture coords
	GL_CMD(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, u)));
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
	GL_CMD(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite2D), (const void*)offsetof(Sprite2D, x)));
	GL_CMD(glEnableVertexAttribArray(2));
	GL_CMD(glVertexAttribDivisor(2, 1)); // configure as per instance
	
	// sizes
	GL_CMD(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite2D), (const void*)offsetof(Sprite2D, width)));
	GL_CMD(glEnableVertexAttribArray(3));
	GL_CMD(glVertexAttribDivisor(3, 1)); // configure as per instance

	// UV offsets
	GL_CMD(glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite2D), (const void*)offsetof(Sprite2D, uv_x)));
	GL_CMD(glEnableVertexAttribArray(4));
	GL_CMD(glVertexAttribDivisor(4, 1)); // configure as per instance
	
	// UV scales
	GL_CMD(glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite2D), (const void*)offsetof(Sprite2D, uv_scale_x)));
	GL_CMD(glEnableVertexAttribArray(5));
	GL_CMD(glVertexAttribDivisor(5, 1)); // configure as per instance

	// initialise with NULL
	GL_CMD(glBufferData(GL_ARRAY_BUFFER,
			SPRITES_MAX * sizeof(Sprite2D), NULL, GL_STREAM_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return sprites_vbo;
}
