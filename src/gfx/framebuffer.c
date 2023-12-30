#include "framebuffer.h"
#include "core/log.h"
#include "core/assert.h"

#include <stdlib.h>

Framebuffer framebuffer_create(Texture texture)
{
	Framebuffer framebuffer = { .id = 0 };

	GLuint id;
	GL_CMD(glGenFramebuffers(1, &id));
	GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, id));
	GL_CMD(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0));

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ASSERT(status == GL_FRAMEBUFFER_COMPLETE,
			"failed to create framebuffer.");

	GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	framebuffer.id = id;

	return framebuffer;
}

void framebuffer_destroy(Framebuffer framebuffer)
{
	GL_CMD(glDeleteFramebuffers(1, &framebuffer.id));
}

void framebuffer_bind(Framebuffer framebuffer)
{
	GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id));
}

void framebuffer_unbind()
{
	GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}