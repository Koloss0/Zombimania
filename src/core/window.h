#ifndef WINDOW_H
#define WINDOW_H

#include "gfx/gl.h"

#include <stdbool.h>

typedef struct
{
	unsigned int width, height;
	const char* title;
} WindowSettings;

typedef struct Window Window;

Window* window_create(const WindowSettings* window_settings);
void window_destroy(Window* window);

void window_update(Window* window);
bool window_should_close(Window* window);

bool window_is_key_pressed(const Window* window, int key);
bool window_is_mouse_pressed(const Window* window, int button);
void window_get_mouse_pos(const Window* window, double* x, double* y);

#endif
