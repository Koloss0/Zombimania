#include "config.h"
#include "window.h"
#include "gfx/gfx.h"
#include "io/txt.h"

#include <stdio.h>

static void display_splash_text();

int main()
{
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

	display_splash_text();

	// delta time variables
	double delta = 0.0;
	double last_frame_time = 0.0;

	while (!window_should_close(window))
	{
		double time = glfwGetTime();
		delta = time - last_frame_time;
		last_frame_time = time;

		gfx_begin(&CAMERA_DEFAULT);

		gfx_background(0.0f, 0.0f, 0.0f);

		gfx_sprite2d(30.0f, 55.0f, 25.0f, 25.0f, (TextureRect){51, 51, 95, 95});
		gfx_sprite2d(50.0f, 50.0f, 25.0f, 25.0f, (TextureRect){332, 51, 95, 95});
		gfx_sprite2d(70.0f, 45.0f, 25.0f, 25.0f, (TextureRect){508, 51, 95, 95});

		gfx_end();

		window_update(window);
	}
	
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