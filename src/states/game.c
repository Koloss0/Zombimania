#include "states.h"
#include "core/fsm.h"
#include "gfx/gfx.h"
#include "gfx/mesh.h"
#include "io/bmp.h"
#include "io/input.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static Mesh* cube;
static Texture cube_texture;
static Shader cube_shader;

static Camera camera = {
	.x = 0.0, .y = 0.0, .z = 0.0,
	.rx = 0.0, .ry = 0.0
};


// called once at the start of the program.
void game_init()
{
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
void game_destroy()
{

	mesh_destroy(cube);
	shader_destroy(cube_shader);
	texture_destroy(cube_texture);

}

// called whenever the state is entered.
void game_enter()
{}

// called whenever the state exits.
void game_exit()
{}

// called every frame when the state is active.
void game_update(double delta)
{	
	if (input_is_key_pressed(GLFW_KEY_W))
	{
		camera.z += 4.0 * delta;
	}
	else if (input_is_key_pressed(GLFW_KEY_S))
	{
		camera.z -= 4.0 * delta;
	}
	else if (input_is_key_pressed(GLFW_KEY_A))
	{
		camera.x -= 4.0 * delta;
	}
	else if (input_is_key_pressed(GLFW_KEY_D))
	{
		camera.x += 4.0 * delta;
	}

	gfx_begin(&camera);
	gfx_background(0.2f, 0.2f, 0.2f);

	gfx_mesh(cube);

	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void game_key_input(int key, int action, int scancode, int mods)
{
	if(key == GLFW_KEY_ESCAPE)
	{
		exit(0);
	}
}

// called when a mouse button is pressed or released.
void game_mouse_button_input(int button, bool pressed, int mods)
{}
