#include "states.h"
#include "gfx/gfx.h"

#include <stdbool.h>

// called once at the start of the program.
void game_init()
{}

// called once before the program exits.
void game_destroy()
{}

// called whenever the state is entered.
void game_enter()
{}

// called whenever the state exits.
void game_exit()
{}

// called every frame when the state is active.
void game_update(double delta)
{
	gfx_begin(&CAMERA_DEFAULT);
	gfx_background(0.2f, 0.2f, 0.2f);
	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void game_key_input(int key, int action, int scancode, int mods)
{}

// called when a mouse button is pressed or released.
void game_mouse_button_input(int button, bool pressed, int mods)
{}
