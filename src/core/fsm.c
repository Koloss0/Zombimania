#include "fsm.h"
#include "init.h"
#include "states/states.h"

#include <stdlib.h>

static GameState states[NUM_STATES];
static GameStateID current_state;
static GameStateID next_state;

#define CREATE_STATE(enum_name, namespace) \
	do { \
		GameState state; \
		 \
		state.enter = namespace##_enter; \
		state.update = namespace##_update; \
		state.exit = namespace##_exit; \
		state.key_input = namespace##_key_input; \
		state.mouse_button_input = namespace##_mouse_button_input; \
		 \
		states[enum_name] = state; \
	} while(0)

#define DESTROY_STATE(namespace) namespace##_destroy()
#define INIT_STATE(namespace) namespace##_init()

bool fsm_init(GameStateID initial_state)
{
	INIT_FUNC();

	current_state = NONE;

	// create the structs for all game states.
	CREATE_STATE(MAIN_MENU, mm);
	// ...

	// call init functions of all game states.
	INIT_STATE(mm);
	// ...

	fsm_change_state(initial_state);

	return true;
}

void fsm_shutdown()
{
	fsm_change_state(NONE);

	// destroy the structs for all game states.
	DESTROY_STATE(mm);
	// ...

	SHUTDOWN_FUNC();
}

void fsm_queue_next_state(GameStateID state)
{
	REQUIRE_INIT();

	next_state = state;
}

void fsm_change_state(GameStateID state)
{
	REQUIRE_INIT();

	GameStateID old_state = current_state;
	current_state = state;

	if (old_state != NONE)
	{
		states[old_state].exit();
	}
	if (state != NONE)
	{
		states[state].enter();
	}
}

void fsm_update(double delta)
{
	REQUIRE_INIT();

	if (current_state != NONE)
	{
		next_state = NONE;

		states[current_state].update(delta);

		if (next_state != NONE && next_state != current_state)
		{
			fsm_change_state(next_state);
		}
	}
}

void fsm_key_input(int key, int scancode, int action, int mods)
{
	if (current_state != NONE)
	{
		states[current_state].key_input(key, scancode, action, mods);
	}
}
