#include "core.h"
#include "init.h"
#include "config.h"
#include "math/math.h"
#include "window.h"
#include "gfx/gfx.h"
#include "fsm.h"
#include "io/txt.h"

#include <stdbool.h>

static void display_splash_text();

static Window* window = NULL;

bool core_init()
{
	INIT_FUNC();

	if (!math_init())
		return false;

	window = window_create(&WINDOW_SETTINGS);
	if (!window)
	{
		core_shutdown();
		return false;
	}

	if (!gfx_init(VIEWPORT_WIDTH, VIEWPORT_HEIGHT))
	{
		core_shutdown();
		return false;
	}

	{
		int w, h;
		window_get_size(window, &w, &h);
		gfx_fit_viewport(w, h);
	}

	if (!fsm_init(INITIAL_STATE))
	{
		core_shutdown();
		return false;
	}

	display_splash_text();

	return true;
}

void core_shutdown()
{
	fsm_shutdown();
	gfx_shutdown();

	if (window)
		window_destroy(window);
	
	math_shutdown();

	SHUTDOWN_FUNC();
}

void core_run()
{
	REQUIRE_INIT();

	// delta time variables
	double delta = 0.0;
	double last_frame_time = 0.0;

	while (!window_should_close(window))
	{
		double time = glfwGetTime();
		delta = time - last_frame_time;
		last_frame_time = time;

		fsm_update(delta);
		window_update(window);
	}
}

static void display_splash_text()
{
	printf("%s v%s - Compiled on %s UTC.\n", PROJECT_NAME, ZBM_VERSION, TIMESTAMP_UTC);
	
	char* splash = io_load_txt("res/splash.txt");
	printf("%s\n", splash);
}