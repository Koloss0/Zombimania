#include "states.h"
#include "gfx/gfx.h"
#include "math/math.h"
#include "gfx/sprite2d.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>

struct Sprite
{
	double x, y;
};

#define NUM_SPRITES SPRITES_MAX

static struct Sprite sprites[NUM_SPRITES];
static int frame = 0;

// called once at the start of the program.
void mm_init()
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		struct Sprite sprite;

		//sprite.x = math_randf_gaussian((double)VIEWPORT_WIDTH / 2.0, (double)VIEWPORT_WIDTH / 16.0);
		//sprite.y = math_randf_gaussian((double)VIEWPORT_HEIGHT / 2.0, (double)VIEWPORT_HEIGHT / 16.0);

		double rx = (double)VIEWPORT_WIDTH / 2.0;
		double ry = (double)VIEWPORT_HEIGHT / 2.0;
		double x = (double)VIEWPORT_WIDTH / 2.0;
		double y = (double)VIEWPORT_HEIGHT / 2.0;
		sprite.x = math_randf_range(x - rx, x + rx);
		sprite.y = math_randf_range(y - ry, y + ry);

		sprites[i] = sprite;
	}

	frame = 0;
}

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

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sprites[i].x = (float)math_fmod(sprites[i].x + 1.0, (double)VIEWPORT_WIDTH);
		gfx_sprite2d((float)sprites[i].x, (float)sprites[i].y, 25.0f, 25.0f, (TextureRect){51, 51, 95, 95});
	}

	gfx_end();

	if (frame % 1000 == 0)
	{
		printf("frame: %d\n", frame);
	}

	frame++;
}

// called when a key is either pressed, repeated (held down), or released.
void mm_key_input(int key, int action, int scancode, int mods)
{}

// called when a mouse button is pressed or released.
void mm_mouse_button_input(int button, bool pressed, int mods)
{}