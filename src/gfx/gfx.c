#include "gfx.h"
#include "sprite2d.h"
#include "camera.h"
#include "texture_rect.h"
#include "viewport.h"

#include "core/log.h"
#include "core/assert.h"

#include <stdbool.h>

// TODO
// [ ] perspective

static bool init = false;
static Viewport* viewport = NULL;
static struct {
	int x, y, w, h;
} screen_bounds = {0,0,0,0};

bool gfx_init(unsigned long viewport_width, unsigned long viewport_height)
{
	ASSERT(!init, "attempt to initialise GFX twice");

	viewport = viewport_create(viewport_width, viewport_height);
	if (!viewport)
	{
		LOG_ERROR("failed to initialise GFX: failed to create viewport.");
		return false;
	}

	if (!sprite2d_init(viewport_width, viewport_height))
		return false;

	init = true;
	return true;
}

void gfx_shutdown()
{
	ASSERT(init, "attempt to shutdown GFX when uninitialised");

	sprite2d_shutdown();

	viewport_destroy(viewport);

	init = false;
}

void gfx_fit_viewport(int width, int height)
{
	ASSERT(width >= 0, "invalid window width.");
	ASSERT(height >= 0, "invalid window height.");

	// aspect ratio of the viewport.
	double viewport_ar = (double)viewport->width / (double)viewport->height;

	// width and height of the window.
	double w = (double)width;
	double h = (double)height;

	if (w / h < viewport_ar)
	{ // window is taller than viewport
		double new_height = w/viewport_ar;
		screen_bounds.w = width;
		screen_bounds.h = (int)new_height;
		screen_bounds.x = 0;
		screen_bounds.y = (int)(h*0.5 - new_height*0.5);
	}
	else
	{ // window is wider than viewport
		double new_width = h*viewport_ar;
		screen_bounds.w = (int)new_width;
		screen_bounds.h = height;
		screen_bounds.x = (int)(w*0.5 - new_width*0.5);
		screen_bounds.y = 0;
	}
}

void gfx_begin(const Camera* camera)
{
	// render to viewport.
	viewport_bind(viewport);

	sprite2d_begin();
}

void gfx_end()
{
	gfx_flush();
	sprite2d_end();

	// return to normal rendering.
	viewport_unbind();
	glViewport(screen_bounds.x, screen_bounds.y, screen_bounds.w, screen_bounds.h);

	// black bars.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	viewport_draw(viewport);
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