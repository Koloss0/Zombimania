#ifndef STATES_H
#define STATES_H

#include <stdbool.h>

typedef struct
{
	// function pointers
	void (*enter)();
	void (*update)(double delta);
	void (*exit)();
	void (*key_input)(int key, int scancode, int action, int mods);
	void (*mouse_button_input)(int button, bool pressed, int mods);
	void (*mouse_movement_input)(double x, double y, double delta_x, double delta_y);
} GameState;

#define GAME_STATE_INTERFACE(namespace) \
	void namespace##_init(); \
	void namespace##_destroy(); \
	void namespace##_enter(); \
	void namespace##_exit(); \
	void namespace##_update(double delta); \
	void namespace##_key_input(int key, int scancode, int action, int mods); \
	void namespace##_mouse_button_input(int button, bool pressed, int mods); \
	void namespace##_mouse_movement_input(double x, double y, double delta_x, double delta_y)

#define NUM_STATES 3

typedef enum
{
	NONE = -1,
	MAIN_MENU = 0,
	SETTINGS,
	GAME
	// ...
} GameStateID;

// define all game states here.
GAME_STATE_INTERFACE(mm);
GAME_STATE_INTERFACE(settings);
GAME_STATE_INTERFACE(game);
// ...

#endif