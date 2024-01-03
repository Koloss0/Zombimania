#include "core.h"
#include "init.h"
#include "config.h"
#include "math/math.h"
#include "window.h"
#include "gfx/gfx.h"
#include "fsm.h"
#include "io/txt.h"
#include "io/input.h"
#include "sfx/sfx.h"

#include <stdbool.h>

static Window* window = NULL;
static bool running = false;

static void on_window_event(WindowEvent event);
static void display_splash_text();

bool core_init()
{
	REQUIRE_UNINIT();
	INIT_STATUS(INITIALISED);

	if (!math_init())
	{
		LOG_ERROR("Math failed to initialise.");
		core_shutdown();
		return false;
	}

	window = window_create(&WINDOW_SETTINGS);
	if (!window)
	{
		LOG_ERROR("window failed to initialise.");
		core_shutdown();
		return false;
	}
	window_set_event_callback(window, on_window_event);

	if (!gfx_init((unsigned)VIEWPORT_WIDTH, (unsigned)VIEWPORT_HEIGHT))
	{
		LOG_ERROR("GFX failed to initialise.");
		core_shutdown();
		return false;
	}

	{
		int w, h;
		window_get_size(window, &w, &h);
		gfx_fit_viewport(w, h);
	}
	
	if (!input_init(window))
	{
		LOG_ERROR("Input failed to initialise.");
		core_shutdown();
		return false;
	}

	if (!sfx_init())
	{
		LOG_ERROR("SFX failed to initialise.");
		core_shutdown();
		return false;
	}

	if (!fsm_init(INITIAL_STATE))
	{
		LOG_ERROR("FSM failed to initialise.");
		core_shutdown();
		return false;
	}

	display_splash_text();

	running = true;

	return true;
}

void core_shutdown()
{
	if (init_status == INITIALISED)
	{
		running = false;
		fsm_shutdown();
		sfx_shutdown();
		input_shutdown();
		gfx_shutdown();
		if (window)
			window_destroy(window);
		math_shutdown();

		INIT_STATUS(UNINITIALISED);
	}
}

void core_run()
{
	REQUIRE_INIT();

	// delta time variables
	double delta = 0.0;
	double last_frame_time = 0.0;

	while (running)
	{
		double time = glfwGetTime();
		delta = time - last_frame_time;
		last_frame_time = time;

		fsm_update(delta);
		window_update(window);
	}
}

static void on_window_event(WindowEvent event)
{
	switch (event.id)
	{
		case CLOSE_EVENT:
		{
			running = false;
			break;
		}
		case RESIZE_EVENT:
		{
			ResizeEventData* data = (ResizeEventData*)event.data;
			gfx_fit_viewport(data->width, data->height);
			break;
		}
		default:
			break;
	}

	fsm_on_window_event(event);
}

static void display_splash_text()
{
	printf("%s v%s - Compiled on %s UTC.\n", PROJECT_NAME, ZBM_VERSION, TIMESTAMP_UTC);
	
	char* splash = io_load_txt("res/splash.txt");
	printf("%s\n", splash);
}