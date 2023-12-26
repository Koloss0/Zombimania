#ifndef STATES_H
#define STATES_H

#include <stdbool.h>

typedef struct
{
	// function pointers
	void (*enter)();
	void (*update)(double delta);
	void (*exit)();
	void (*key_input)(int key, int action, int scancode, int mods);
	void (*mouse_button_input)(int button, bool pressed, int mods);
} GameState;

#define GAME_STATE_INTERFACE(namespace) \
	void namespace##_init(); \
	void namespace##_destroy(); \
	void namespace##_enter(); \
	void namespace##_exit(); \
	void namespace##_update(double delta); \
	void namespace##_key_input(int key, int action, int scancode, int mods); \
	void namespace##_mouse_button_input(int button, bool pressed, int mods)

#define NUM_STATES 2

typedef enum
{
	NONE = -1,
	MAIN_MENU = 0,
	GAME,
	// ...
} GameStateID;

// define all game states here.
GAME_STATE_INTERFACE(mm);
// ...

#endif