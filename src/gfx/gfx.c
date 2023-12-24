#include "gfx.h"
#include "sprite2d.h"
#include "camera.h"
#include "texture_rect.h"

#include "core/log.h"
#include "core/assert.h"

#include <stdbool.h>

// TODO
// [ ] perspective
// [ ] framebuffer

static bool init = false;

bool gfx_init()
{
	ASSERT(!init, "attempt to initialise GFX twice");

	if (!sprite2d_init())
		return false;

	init = true;
	return true;
}

void gfx_shutdown()
{
	ASSERT(init, "attempt to shutdown GFX when uninitialised");

	sprite2d_shutdown();

	init = false;
}

void gfx_begin(const Camera* camera)
{
	sprite2d_begin();
}

void gfx_end()
{
	sprite2d_end();
}

void gfx_flush()
{
	sprite2d_flush();
}

void gfx_background(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_sprite2d(float x, float y, float width, float height, TextureRect texture_rect)
{
	sprite2d_draw(x, y, width, height, texture_rect);
}