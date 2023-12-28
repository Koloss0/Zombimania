#include "config.h"
#include "math/math.h"
#include "window.h"
#include "gfx/gfx.h"
#include "io/txt.h"
#include "fsm.h"

#include <stdio.h>

static void display_splash_text();

int main()
{
	if (!math_init())
	{
		return -1;
	}

	Window* window = window_create(&WINDOW_SETTINGS);
	
	if (!window)
	{
		return -1;
	}

	if (!gfx_init(VIEWPORT_WIDTH, VIEWPORT_HEIGHT))
	{
		window_destroy(window);
		return -1;
	}

	gfx_fit_viewport((int)WINDOW_SETTINGS.width, (int)WINDOW_SETTINGS.height);

	if (!fsm_init(INITIAL_STATE))
	{
		gfx_shutdown();
		window_destroy(window);
		return -1;
	}

	display_splash_text();

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
	
	fsm_shutdown();
	gfx_shutdown();
	window_destroy(window);
	return 0;
}

static void display_splash_text()
{
	printf("%s v%s - Compiled on %s UTC.\n", PROJECT_NAME, ZBM_VERSION, TIMESTAMP_UTC);
	
	char* splash = io_load_txt("res/splash.txt");
	printf("%s\n", splash);
}