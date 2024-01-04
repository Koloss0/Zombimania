#include "sprite3d.h"
#include "shader.h"
#include "texture.h"
#include "texture_rect.h"
#include "io/bmp.h"
#include "math/matrix.h"
#include "core/init.h"
#include "core/log.h"
#include "core/assert.h"
#include "gl.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
	float x, y, z, u, v;
} Vertex;

typedef struct
{
	float x, y, z, width, height;
	float uv_x, uv_y, uv_scale_x, uv_scale_y;
} Sprite3D;

static struct
{
	Sprite3D sprites[SPRITE3D_MAX_SPRITES];
	size_t sprites_size;

	GLuint vao, quad_vbo, sprites_vbo;
	Shader shader;
	Texture texture_atlas;

	const Camera* current_camera;
} state;

static GLuint create_quad_vbo();
static GLuint create_sprites_vbo();

bool sprite3d_init(unsigned int viewport_width, unsigned int viewport_height)
{
	REQUIRE_UNINIT();
	INIT_STATUS(INITIALISED);

	// zero-initialise all state.
	memset(&state, 0, sizeof(state));

	// create vao.
	GL_CMD(glGenVertexArrays(1, &state.vao));
	GL_CMD(glBindVertexArray(state.vao));

	// create vbo's.
	state.quad_vbo = create_quad_vbo();
	state.sprites_vbo = create_sprites_vbo();
	
	GL_CMD(glBindVertexArray(0));

	// create shader.
	state.shader = shader_create("res/shaders/sprite3d.shader");
	if (state.shader.id == 0)
	{
		LOG_ERROR("failed to initialise Sprite3D: could not create shader.");
		sprite3d_shutdown();
		return false;
	}

	// load texture image.
	Image texture_image = io_load_bmp("res/images/sprite3d.bmp", false);
	if (!texture_image.data)
	{
		LOG_ERROR("failed to initialise Sprite3D: could not load texture atlas.");
		sprite3d_shutdown();
		return false;
	}

	// create texture.
	state.texture_atlas = texture_create(texture_image, NULL);
	ASSERT(state.texture_atlas.id > 0,
			"failed to initialise Sprite3D: failed to create texture.");

	shader_use(state.shader);
	shader_set_int(state.shader, "textureImage", 0);

	Mat4 model_mat = MAT4_IDENTITY;
	const Mat4 view_mat = MAT4_IDENTITY;
	Mat4 projection = mat4_perspective(70.0, 1.6, 0.1, 20.0); // FIXME: shouldn't be here.
	shader_set_mat4(state.shader, "model", &model_mat);
	shader_set_mat4(state.shader, "view", &view_mat);
	shader_set_mat4(state.shader, "projection", &projection);

	return true;
}

void sprite3d_shutdown()
{
	if (init_status == INITIALISED)
	{
		if (state.vao > 0)
			GL_CMD(glDeleteVertexArrays(1, &state.vao));

		if (state.quad_vbo > 0)
			GL_CMD(glDeleteBuffers(1, &state.quad_vbo));

		if (state.sprites_vbo > 0)
			GL_CMD(glDeleteBuffers(1, &state.sprites_vbo));
		
		if (state.shader.id > 0)
			shader_destroy(state.shader);

		if (state.texture_atlas.id > 0)
			texture_destroy(state.texture_atlas);

		INIT_STATUS(UNINITIALISED);
	}
}

void sprite3d_draw(double x, double y, double z, double width, double height, TextureRect texture_rect)
{
	REQUIRE_INIT();

	if (width == 0.0 && height == 0.0)
	{
		width = (double)texture_rect.w;
		height = (double)texture_rect.h;
	}
	else if (width == 0.0)
	{
		double aspect = (double)texture_rect.w/(double)texture_rect.h;
		width = height*aspect;
	}
	else if (height == 0.0)
	{
		double aspect = (double)texture_rect.w/(double)texture_rect.h;
		height = width/aspect;
	}

	if (state.sprites_size < SPRITE3D_MAX_SPRITES)
	{
		float tex_width = (float)state.texture_atlas.width;
		float tex_height = (float)state.texture_atlas.height;

		Sprite3D sprite = {
			.x =          (float)x,
			.y =          (float)y,
			.z =          (float)z,
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
		LOG_WARN("[GFX] maximum number of sprites reached. (%d)", SPRITE3D_MAX_SPRITES);
	}
}

void sprite3d_begin(const Camera* camera)
{
	REQUIRE_INIT();

	state.current_camera = camera;
}

void sprite3d_end()
{
	REQUIRE_INIT();

	sprite3d_flush();
}

void sprite3d_flush()
{
	REQUIRE_INIT();

	GL_CMD(glBindVertexArray(state.vao));
	
	texture_bind(state.texture_atlas);	
	shader_use(state.shader);

	shader_set_mat4(state.shader, "view", &state.current_camera->view_mat);
	shader_set_vec3(state.shader, "cameraRight",
			state.current_camera->transform.right_x,
			state.current_camera->transform.right_y,
			state.current_camera->transform.right_z);
	shader_set_vec3(state.shader, "cameraUp",
			state.current_camera->transform.up_x,
			state.current_camera->transform.up_y,
			state.current_camera->transform.up_z);

	// update sprite buffer
	glBindBuffer(GL_ARRAY_BUFFER, state.sprites_vbo);
	glBufferData(GL_ARRAY_BUFFER,
			SPRITE3D_MAX_SPRITES * sizeof(Sprite3D), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
			(GLsizeiptr)(state.sprites_size * sizeof(Sprite3D)), state.sprites);

	// instanced draw call
	GL_CMD(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)state.sprites_size));

	// clear sprite array
	state.sprites_size = 0;
}

static GLuint create_quad_vbo()
{
	// 1x1 quad
	const Vertex quad[] = {
	//        x,     y,    z,    u,    v
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
		{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f, 1.0f }
	};

	GLuint quad_vbo;
	GL_CMD(glGenBuffers(1, &quad_vbo));
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, quad_vbo));
	
	// position
	GL_CMD(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
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
	GL_CMD(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			sizeof(Sprite3D), (const void*)offsetof(Sprite3D, x)));
	GL_CMD(glEnableVertexAttribArray(2));
	GL_CMD(glVertexAttribDivisor(2, 1)); // configure as per instance
	
	// sizes
	GL_CMD(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite3D), (const void*)offsetof(Sprite3D, width)));
	GL_CMD(glEnableVertexAttribArray(3));
	GL_CMD(glVertexAttribDivisor(3, 1)); // configure as per instance

	// UV offsets
	GL_CMD(glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite3D), (const void*)offsetof(Sprite3D, uv_x)));
	GL_CMD(glEnableVertexAttribArray(4));
	GL_CMD(glVertexAttribDivisor(4, 1)); // configure as per instance
	
	// UV scales
	GL_CMD(glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE,
			sizeof(Sprite3D), (const void*)offsetof(Sprite3D, uv_scale_x)));
	GL_CMD(glEnableVertexAttribArray(5));
	GL_CMD(glVertexAttribDivisor(5, 1)); // configure as per instance

	// initialise with NULL
	GL_CMD(glBufferData(GL_ARRAY_BUFFER,
			SPRITE3D_MAX_SPRITES * sizeof(Sprite3D), NULL, GL_STREAM_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return sprites_vbo;
}
