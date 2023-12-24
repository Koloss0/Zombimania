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

static struct
{
	Sprite2DState sprite2d_state;
} gfx_state;

bool gfx_init()
{
	ASSERT(!init, "attempt to initialise GFX twice");

	if (!sprite2d_init(&gfx_state.sprite2d_state))
		return false;

	init = true;
	return true;
}

void gfx_shutdown()
{
	ASSERT(init, "attempt to shutdown GFX when uninitialised");

	sprite2d_shutdown(&gfx_state.sprite2d_state);

	init = false;
}

void gfx_begin(const Camera* camera)
{
	sprite2d_begin(&gfx_state.sprite2d_state);
}

void gfx_end()
{
	sprite2d_end(&gfx_state.sprite2d_state);
}

void gfx_flush()
{
	sprite2d_flush(&gfx_state.sprite2d_state);
}

void gfx_background(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_sprite2d(float x, float y, float width, float height, TextureRect texture_rect)
{
	sprite2d_draw(&gfx_state.sprite2d_state, x, y, width, height, texture_rect);
}