#include "states.h"
#include "core/fsm.h"
#include "gfx/gfx.h"
#include "gfx/mesh.h"
#include "io/bmp.h"
#include "io/input.h"
#include "config.h"
#include "sfx/sfx.h"
#include "math/math.h"

// standard libraries
#include <stdio.h>
#include <stdbool.h>

static const int NUM_BUTTONS = 3;
static const int LEFT_PAD = 24; // in px
static const int VERT_SPACE = 24; // in px
static int selected_button;

static void on_settings_pressed();
static void on_play_pressed();
static void on_quit_pressed();

typedef struct
{
	int x;
	int y;
	int height;	
	int width;
	TextureRect textrect;
	void (*callback_function)();
} Button;

static Button buttons[] = {
	{ // play
		.width=58, .height=16, 
		.textrect={.x=0,.y=32,.w=58,.h=16},
		.callback_function=on_play_pressed
	},
	{ // settings
		.width=116, .height=16,
		.textrect={.x=0,.y=17,.w=116,.h=15},
		.callback_function=on_settings_pressed
	},
	{ // quit
		.width=56, .height=16, 
		.textrect={.x=0,.y=0,.w=56,.h=16},
		.callback_function=on_quit_pressed
	}
};

// called once at the start of the program.
void mm_init()
{
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		buttons[i].x = buttons[i].width/2+LEFT_PAD;
		buttons[i].y = VIEWPORT_HEIGHT/2+(NUM_BUTTONS-i)*VERT_SPACE-(NUM_BUTTONS*VERT_SPACE/2);
	}

	selected_button = 0;

	// music by @dAmbient: https://freesound.org/people/dAmbient/sounds/251936/
	//sfx_play_sound("res/sfx/titlescreen.mp3");
}

// called once before the program exits.
void mm_destroy()
{}

// called whenever the state is entered.
void mm_enter()
{
	input_set_mouse_mode(MOUSE_MODE_NORMAL);
}

// called whenever the state exits.
void mm_exit()
{}

// called every frame when the state is active.
void mm_update(double delta)
{
	gfx_begin(&CAMERA_DEFAULT);
	gfx_background(0.11f, 0.11f, 0.11f);
	gfx_sprite2d(160,190,0,0, (TextureRect){0, 48, 197, 20}); // zombimania
	
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		gfx_sprite2d(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttons[i].textrect);
		
	}

	gfx_sprite2d(10, buttons[selected_button].y, 0, 0, (TextureRect){1, 72, 14, 16});

	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void mm_key_input(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if(key == GLFW_KEY_UP)
		{
			selected_button--;
			if(selected_button < 0)
				selected_button = 0;
		}

		if(key == GLFW_KEY_DOWN)
		{
			selected_button++;
			if(selected_button >= NUM_BUTTONS)
				selected_button = NUM_BUTTONS - 1;
		}
	}

	if (action == GLFW_PRESS)
	{
		if(key == GLFW_KEY_ENTER)
		{
			buttons[selected_button].callback_function();
		}
	}
}

// called when a mouse button is pressed or released.
void mm_mouse_button_input(int button, bool pressed, int mods)
{}

// called when the mouse moves.
void mm_mouse_movement_input(double x, double y, double delta_x, double delta_y)
{}

static void on_settings_pressed()
{
	fsm_change_state(SETTINGS);
}

static void on_play_pressed()
{
	fsm_change_state(GAME);
}

static void on_quit_pressed()
{
	fsm_exit();
}
