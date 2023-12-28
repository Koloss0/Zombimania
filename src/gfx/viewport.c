#include "viewport.h"
#include "core/log.h"
#include "core/assert.h"

#include <stdlib.h>

static GLuint create_quad_vbo();

Viewport* viewport_create(unsigned long width, unsigned long height)
{
	Viewport* viewport = malloc(sizeof(Viewport));
	if (!viewport)
	{
		LOG_ERROR("failed to create viewport: out of memory.");
		return NULL;
	}

	Image image = {
		.width  = width,
		.height = height,
		.data   = NULL,
	};

	Texture texture = texture_create(image, NULL);
	ASSERT(texture.id > 0, "failed to create viewport: failed to create texture.");

	Framebuffer framebuffer = framebuffer_create(texture);

	// create vao.
	GLuint quad_vao;
	GL_CMD(glGenVertexArrays(1, &quad_vao));
	GL_CMD(glBindVertexArray(quad_vao));

	GLuint quad_vbo = create_quad_vbo();

	GL_CMD(glBindVertexArray(0));

	Shader shader = shader_create("res/shaders/viewport.shader");
	if (shader.id == 0)
	{
		LOG_ERROR("failed to create viewport: failed to create shader.");
		// FIXME: delete quad vbo & vao
		framebuffer_destroy(framebuffer);
		texture_destroy(texture);
		free(viewport);
		return NULL;
	}

	shader_use(shader);
	shader_set_int(shader, "screenTexture", 0);

	viewport->width       = width;
	viewport->height      = height;
	viewport->framebuffer = framebuffer;
	viewport->texture     = texture;
	viewport->shader      = shader;
	viewport->quad_vbo    = quad_vbo;
	viewport->quad_vao    = quad_vao;

	return viewport;
}

void viewport_destroy(Viewport* viewport)
{
	framebuffer_destroy(viewport->framebuffer);
	texture_destroy(viewport->texture);
	free(viewport);
}

void viewport_bind(Viewport* viewport)
{
	framebuffer_bind(viewport->framebuffer);

	glViewport(0, 0, (GLsizei)viewport->width, (GLsizei)viewport->height);
}

void viewport_unbind()
{
	framebuffer_unbind();
}

void viewport_draw(Viewport* viewport)
{
	GL_CMD(glBindVertexArray(viewport->quad_vao));

	GL_CMD(glActiveTexture(GL_TEXTURE0));
	texture_bind(viewport->texture);
	shader_use(viewport->shader);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static GLuint create_quad_vbo()
{
	struct Vertex
	{
		float x, y, u, v;
	};

	// 2x2 quad
	const struct Vertex quad[] = {
		{ -1.0f, -1.0f, 0.0f, 0.0f },
		{  1.0f, -1.0f, 1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f, 1.0f },
		{  1.0f,  1.0f, 1.0f, 1.0f }
	};

	GLuint quad_vbo;
	GL_CMD(glGenBuffers(1, &quad_vbo));
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, quad_vbo));
	
	// position
	GL_CMD(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(struct Vertex), (const void*)offsetof(struct Vertex, x)));
	GL_CMD(glEnableVertexAttribArray(0));
	
	// texture coords
	GL_CMD(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(struct Vertex), (const void*)offsetof(struct Vertex, u)));
	GL_CMD(glEnableVertexAttribArray(1));

	// pass in quad vertices
	GL_CMD(glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW));
	
	GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return quad_vbo;
}