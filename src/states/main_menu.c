#include "states.h"
#include "gfx/gfx.h"
#include <stdbool.h>
#include "config.h"
static const int NUM_BUTTONS=3;
static const int LEFT_PAD=24;//offset in pixels on left 
static const int VERT_SPACE=24;
static int selected_button;
typedef struct
{
	int x;
	int y;
	int height;	
	int width;
	TextureRect textrect;
} Button;

static Button buttons[] = {
	{
		.width=58, .height=16, 
		.textrect={.x=0,.y=32,.w=58,.h=16}
	}, //play
	{
		.width=116, .height=16,
		.textrect={.x=0,.y=17,.w=116,.h=16}
	}, //settings
	{
		.width=56, .height=16, 
		.textrect={.x=0,.y=0,.w=56,.h=16}
	} //quit
 };
// called once at the start of the program.
void mm_init()
{
	for(int i=0; i<NUM_BUTTONS; i++)
	{
		buttons[i].x=buttons[i].width/2+LEFT_PAD;
		buttons[i].y=VIEWPORT_HEIGHT/2+(NUM_BUTTONS-i)*VERT_SPACE-(NUM_BUTTONS*VERT_SPACE/2);
	}	

	selected_button=0;
	
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

	gfx_background(0.11f, 0.11f, 0.11f);

	gfx_sprite2d(160,190,0,0, (TextureRect){0,48,197,20});//zombimania
	for(int i=0; i<NUM_BUTTONS; i++)
	{
		gfx_sprite2d(buttons[i].x,buttons[i].y,buttons[i].width,buttons[i].height,buttons[i].textrect);

	}
	//gfx_sprite2d(160,130,0,0, (TextureRect){0,32,58,16});//play
	//gfx_sprite2d(160,110,0,0, (TextureRect){0,17,116,16});//settings
	
	//gfx_sprite2d(VIEWPORT_WIDTH/2,VIEWPORT_HEIGHT/2, 0, 0, (TextureRect){0,0, 221, 200});
	
	
	
	


	gfx_end();
}

// called when a key is either pressed, repeated (held down), or released.
void mm_key_input(int key, int action, int scancode, int mods)
{}

// called when a mouse button is pressed or released.
void mm_mouse_button_input(int button, bool pressed, int mods)
{}