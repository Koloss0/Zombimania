#include "input.h"

#include "core/window.h"
#include "core/init.h"

#include <stdbool.h>

static const Window* window;

bool input_init(const Window* win)
{
	ASSERT(win, "window must be defined");

	INIT_STATUS(INITIALISED);

	window = win;

	return true;
}

void input_shutdown()
{
	if (init_status == INITIALISED)
	{
		window = NULL;

		INIT_STATUS(UNINITIALISED);
	}
}

bool input_is_key_pressed(int key)
{
	REQUIRE_INIT();

	return window_is_key_pressed(window, key);
}

bool input_is_mouse_pressed(int button)
{
	REQUIRE_INIT();

	return window_is_mouse_pressed(window, button);
}

void input_get_mouse_pos(double* x, double* y)
{
	REQUIRE_INIT();

	window_get_mouse_pos(window, x, y);
}

