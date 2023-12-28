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

Texture texture_create(Image image, const TexParameters* params)
{
	if (!params)
		params = &DEFAULT_TEX_PARAMS;
	
	Texture texture;
	
	GL_CMD(glGenTextures(1, &texture.id));
	GL_CMD(glBindTexture(GL_TEXTURE_2D, texture.id));

	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params->wrap_s));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params->wrap_t));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params->filter_min));
	GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params->filter_mag));

	GL_CMD(glTexImage2D(GL_TEXTURE_2D,
		0, params->internal_format, (GLsizei)image.width, (GLsizei)image.height,
		0, params->source_format, GL_UNSIGNED_BYTE, image.data));

	GL_CMD(glBindTexture(GL_TEXTURE_2D, 0));

	texture.width = image.width;
	texture.height = image.height;	
	
	return texture;
}

void texture_destroy(Texture texture)
{
	GL_CMD(glDeleteTextures(1, texture.id));
}

void texture_bind(Texture texture)
{
	GL_CMD(glBindTexture(GL_TEXTURE_2D, texture.id));
}