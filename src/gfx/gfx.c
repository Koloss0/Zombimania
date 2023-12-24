#include "gfx.h"
#include "camera.h"

#include "core/log.h"
#include "core/assert.h"

#include <stdbool.h>

// TODO
// [ ] perspective

static bool init = false;

bool gfx_init()
{
	ASSERT(!init, "attempt to initialise GFX twice");
	init = true;
	return true;
}

void gfx_shutdown()
{
	ASSERT(init, "attempt to shutdown GFX when uninitialised");
	init = false;
}

void gfx_begin(Camera* camera)
{}

void gfx_end()
{}

void gfx_flush()
{}

void gfx_background(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_sprite_2d(float x, float y, float width, float height, TextureRect texture_rect)
{}