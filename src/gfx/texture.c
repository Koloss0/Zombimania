#include "texture.h"
#include "gl.h"
#include "io/image.h"
#include "core/assert.h"
#include "core/log.h"

const TexParameters DEFAULT_TEX_PARAMS = {
	.internal_format = GL_RGBA,
	.source_format = GL_RGBA,
	.wrap_s = GL_REPEAT,
	.wrap_t = GL_REPEAT,
	.filter_min = GL_NEAREST,
	.filter_mag = GL_NEAREST
};

struct Texture
{
	GLuint id;
	unsigned long width;
	unsigned long height;
};

Texture* texture_create()
{
	Texture* texture = malloc(sizeof(Texture));

	if (texture)
		GL_CMD(glGenTextures(1, &texture->id));
	else
		LOG_ERROR("failed to create texture: out of memory.");		
	
	return texture;
}

void texture_destroy(Texture* texture)
{
	ASSERT(texture, "Attempt to destroy null texture");

	GL_CMD(glDeleteTextures(1, &texture->id));
	free(texture);
}

void texture_set_image(Texture* texture, Image image, const TexParameters* params)
{
	ASSERT(texture, "attempt to set image of null texture.");

	if (!params)
		params = &DEFAULT_TEX_PARAMS;

	GL_CMD(glBindTexture(GL_TEXTURE_2D, texture->id));

	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params->wrap_s));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params->wrap_t));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params->filter_min));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params->filter_mag));

	GL_CMD(glTexImage2D(GL_TEXTURE_2D,
		0, params->internal_format, (GLsizei)image.width, (GLsizei)image.height,
		0, params->source_format, GL_UNSIGNED_BYTE, image.data));

	GL_CMD(glBindTexture(GL_TEXTURE_2D, 0));

	texture->width = image.width;
	texture->height = image.height;
}

void texture_bind(Texture* texture)
{
	ASSERT(texture, "attempt to bind null texture.");

	GL_CMD(glBindTexture(GL_TEXTURE_2D, texture->id));
}

unsigned long texture_get_width(const Texture* texture)
{
	return texture->width;
}

unsigned long texture_get_height(const Texture* texture)
{
	return texture->height;
}