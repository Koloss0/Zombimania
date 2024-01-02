#include "states.h"
#include "gfx/gfx.h"
#include "gfx/mesh.h"
#include "io/bmp.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "core/fsm.h"

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

static Mesh* cube;
static Texture cube_texture;
static Shader cube_shader;

static Camera camera = {
	.x = 0.0, .y = 0.0, .z = 0.0,
	.rx = 0.0, .ry = 0.0
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

	///////////////////////////////////////////////////
	// CREATE A CUBE
	///////////////////////////////////////////////////

	// define an array of vertices where every three points draw a triangle.
	const float R = 1.0f;
	MeshVertex vertices[] = {
		// x   y   z    u     v
		// left face
		{ -R,  R,  R, 1.0f, 1.0f },
		{ -R,  R, -R, 0.0f, 1.0f },
		{ -R, -R, -R, 0.0f, 0.0f },
		{ -R, -R, -R, 0.0f, 0.0f },
		{ -R, -R,  R, 1.0f, 0.0f },
		{ -R,  R,  R, 1.0f, 1.0f },
		// front face
		{  R,  R,  R, 1.0f, 1.0f },
		{ -R,  R,  R, 0.0f, 1.0f },
		{ -R, -R,  R, 0.0f, 0.0f },
		{ -R, -R,  R, 0.0f, 0.0f },
		{  R, -R,  R, 1.0f, 0.0f },
		{  R,  R,  R, 1.0f, 1.0f },
		// right face
		{ -R,  R, -R, 1.0f, 1.0f },
		{ -R,  R,  R, 0.0f, 1.0f },
		{ -R, -R,  R, 0.0f, 0.0f },
		{ -R, -R,  R, 0.0f, 0.0f },
		{ -R, -R, -R, 1.0f, 0.0f },
		{ -R,  R, -R, 1.0f, 1.0f },
		// back face
		{ -R,  R, -R, 1.0f, 1.0f },
		{  R,  R, -R, 0.0f, 1.0f },
		{  R, -R, -R, 0.0f, 0.0f },
		{  R, -R, -R, 0.0f, 0.0f },
		{ -R, -R, -R, 1.0f, 0.0f },
		{ -R,  R, -R, 1.0f, 1.0f },
		// top face
		{  R,  R, -R, 1.0f, 1.0f },
		{ -R,  R, -R, 0.0f, 1.0f },
		{ -R,  R,  R, 0.0f, 0.0f },
		{ -R,  R,  R, 0.0f, 0.0f },
		{  R,  R,  R, 1.0f, 0.0f },
		{  R,  R, -R, 1.0f, 1.0f },
		// bottom face
		{  R, -R,  R, 1.0f, 1.0f },
		{ -R, -R,  R, 0.0f, 1.0f },
		{ -R, -R, -R, 0.0f, 0.0f },
		{ -R, -R, -R, 0.0f, 0.0f },
		{  R, -R, -R, 1.0f, 0.0f },
		{  R, -R,  R, 1.0f, 1.0f },
	};
	const size_t NUM_VERTS = sizeof(vertices) / sizeof(MeshVertex);

	// load an image for the texture.
	Image img = io_load_bmp("res/images/cube.bmp", false);

	// load image into a new texture.
	cube_texture = texture_create(img, NULL); // NULL for default settings.

	// delete the image. (or else you get a resource leak)
	image_destroy(img);

	// create a shader. (every mesh needs one)
	cube_shader = shader_create("res/shaders/cube.shader");

	cube = mesh_create(vertices, NUM_VERTS, cube_texture, cube_shader);
}

// called once before the program exits.
void mm_destroy()
{
	// don't forget to clean up. (despite nothing bad will happen if you don't)

	// in no particular order...
	mesh_destroy(cube);
	shader_destroy(cube_shader);
	texture_destroy(cube_texture);
}

// called whenever the state is entered.
void mm_enter()
{}

// called whenever the state exits.
void mm_exit()
{}

// called every frame when the state is active.
void mm_update(double delta)
{
	gfx_begin(&camera);

	gfx_background(0.11f, 0.11f, 0.11f);

	gfx_mesh(cube);

	gfx_sprite2d(160,190,0,0, (TextureRect){0, 48, 197, 20}); // zombimania
	
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		gfx_sprite2d(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttons[i].textrect);
		
	}

	gfx_sprite2d(10, buttons[selected_button].y, 0, 0, (TextureRect){1, 72, 14, 16});

	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void mm_key_input(int key, int action, int scancode, int mods)
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
	exit(0);
}
