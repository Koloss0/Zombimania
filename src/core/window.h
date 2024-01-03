#ifndef WINDOW_H
#define WINDOW_H

#include "gfx/gl.h"
#include "window_event.h"

#include <stdbool.h>

typedef struct
{
	unsigned int width, height;
	const char* title;
	bool fullscreen;
	bool vsync;
} WindowSettings;

typedef enum {
	MOUSE_MODE_NORMAL = 0,
	MOUSE_MODE_CAPTURED,
	MOUSE_MODE_HIDDEN
} MouseMode;

typedef struct Window Window;

typedef void (*WindowEventCallback)(WindowEvent e);

Window* window_create(const WindowSettings* window_settings);
void window_destroy(Window* window);

void window_update(Window* window);
bool window_should_close(Window* window);

void window_set_event_callback(Window* window, WindowEventCallback callback);
void window_set_mouse_mode(const Window* window, MouseMode mouse_mode);

void window_get_size(Window* window, int* width, int* height);

bool window_is_key_pressed(const Window* window, int key);
bool window_is_mouse_pressed(const Window* window, int button);
void window_get_mouse_pos(const Window* window, double* x, double* y);

#endif
