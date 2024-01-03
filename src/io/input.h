#ifndef INPUT_H
#define INPUT_H

#include "core/window.h"

#include <stdbool.h>

bool input_init(const Window* window);
void input_shutdown();

void input_set_mouse_mode(MouseMode mouse_mode);

bool input_is_key_pressed(int key);
bool input_is_mouse_pressed(int button);
void input_get_mouse_pos(double* x, double* y);

#endif
