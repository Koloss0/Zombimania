#include "states.h"
#include "gfx/gfx.h"

#include <stdbool.h>

// called once at the start of the program.
void mm_init()
{}

// called once before the program exits.
void mm_destroy()
{}

// called whenever the state is entered.
void mm_enter()
{}

// called whenever the state exits.
void mm_exit()
{}

// called every frame when the state is active.
void mm_update(double delta)
{
	gfx_begin(&CAMERA_DEFAULT);

	gfx_background(0.0f, 0.0f, 0.0f);

	gfx_sprite2d(30.0f, 55.0f, 25.0f, 25.0f, (TextureRect){51, 51, 95, 95});
	gfx_sprite2d(50.0f, 50.0f, 25.0f, 25.0f, (TextureRect){332, 51, 95, 95});
	gfx_sprite2d(70.0f, 45.0f, 25.0f, 25.0f, (TextureRect){508, 51, 95, 95});

	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void mm_key_input(int key, int action, int scancode, int mods)
{}

// called when a mouse button is pressed or released.
void mm_mouse_button_input(int button, bool pressed, int mods)
{}