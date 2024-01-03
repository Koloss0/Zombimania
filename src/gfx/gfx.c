#include "gfx.h"
#include "sprite2d.h"
#include "camera.h"
#include "texture_rect.h"
#include "viewport.h"

#include "math/math.h"

#include "core/assert.h"
#include "core/init.h"
#include "core/log.h"

#include <stdbool.h>

static Viewport* viewport = NULL;
static struct { int x, y, w, h; } screen_bounds = {0,0,0,0};
static const Camera* current_camera = NULL;

bool gfx_init(unsigned int viewport_width, unsigned int viewport_height)
{
	REQUIRE_UNINIT();
	INIT_STATUS(INITIALISED);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	viewport = viewport_create((unsigned)viewport_width, (unsigned)viewport_height);
	if (!viewport)
	{
		LOG_ERROR("failed to initialise GFX: failed to create viewport.");
		gfx_shutdown();
		return false;
	}

	if (!sprite2d_init((unsigned)viewport_width, (unsigned)viewport_height))
	{
		LOG_ERROR("failed to initialise GFX: failed to initialise Sprite2D.");
		gfx_shutdown();
		return false;
	}

	return true;
}

void gfx_shutdown()
{
	if (init_status == INITIALISED)
	{
		sprite2d_shutdown();

		if (viewport)
			viewport_destroy(viewport);

		INIT_STATUS(UNINITIALISED);
	}
}

void gfx_fit_viewport(int width, int height)
{
	REQUIRE_INIT();

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

bool gfx_screen_pos_to_viewport_pos(double* x, double* y)
{
	double sx = *x;
	double sy = *y;

	bool inside = false;
	if (sx >= screen_bounds.x && sx < screen_bounds.x + screen_bounds.w
	  && sy >= screen_bounds.y && sy < screen_bounds.y + screen_bounds.h)
	{
		inside = true;
	}
	
	sx -= (double)screen_bounds.x;
	sy -= (double)screen_bounds.y;

	sx *= (double)VIEWPORT_WIDTH;
	sy *= (double)VIEWPORT_HEIGHT;

	sx /= (double)screen_bounds.w;
	sy /= (double)screen_bounds.h;

	*x = sx;
	*y = sy;

	return inside;
}

void gfx_begin(const Camera* camera)
{
	REQUIRE_INIT();

	current_camera = camera;

	glClearDepth(1.0f);

	// render to viewport.
	viewport_bind(viewport);

	sprite2d_begin();
}

void gfx_end()
{
	REQUIRE_INIT();

	gfx_flush();
	sprite2d_end();

	// return to normal rendering.
	viewport_unbind();
	glViewport(screen_bounds.x, screen_bounds.y, screen_bounds.w, screen_bounds.h);

	// black bars.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewport_draw(viewport);
}

void gfx_flush()
{
	REQUIRE_INIT();

	sprite2d_flush();
}

void gfx_background(float r, float g, float b)
{
	REQUIRE_INIT();

	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_sprite2d(int x, int y, int width, int height, TextureRect texture_rect)
{
	REQUIRE_INIT();

	sprite2d_draw(x, y, width, height, texture_rect);
}

void gfx_mesh(Mesh* mesh)
{
	Shader shader = mesh_get_shader(mesh);

	shader_use(shader);

	Mat4 view = MAT4_IDENTITY;
	view.x = (float)current_camera->x;
	view.y = (float)current_camera->y;
	view.z = (float)current_camera->z;

	shader_set_mat4(shader, "view", &view);

	mesh_draw(mesh);
}