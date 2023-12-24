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

	if (!gfx_init())
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

		// no textures yet.
		gfx_sprite2d(0.0f, 0.0f, 0.5f, 0.5f, (TextureRect){0,0,1,1});

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