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

	// create depth buffer.
	GLuint depth_buffer;
	GL_CMD(glGenRenderbuffers(1, &depth_buffer));
	GL_CMD(glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer));
	GL_CMD(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			(GLsizei)texture.width, (GLsizei)texture.height));
	GL_CMD(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depth_buffer));

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	ASSERT(status == GL_FRAMEBUFFER_COMPLETE,
			"failed to create framebuffer.");

	GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	framebuffer.id = id;

	return framebuffer;
}

void framebuffer_destroy(Framebuffer framebuffer)
{
	GL_CMD(glDeleteRenderbuffers(1, &framebuffer.depth_buffer_id));
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