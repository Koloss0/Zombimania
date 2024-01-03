#include "fsm.h"
#include "init.h"
#include "window_event.h"
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
		state.mouse_movement_input = namespace##_mouse_movement_input; \
		 \
		states[enum_name] = state; \
	} while(0)

#define DESTROY_STATE(namespace) namespace##_destroy()
#define INIT_STATE(namespace) namespace##_init()

bool fsm_init(GameStateID initial_state)
{
	REQUIRE_UNINIT();
	INIT_STATUS(INITIALISED);

	current_state = NONE;

	// create the structs for all game states.
	CREATE_STATE(MAIN_MENU, mm);
	CREATE_STATE(SETTINGS, settings);
	CREATE_STATE(GAME,game);
	// ...

	// call init functions of all game states.
	INIT_STATE(mm);
	INIT_STATE(settings);
	INIT_STATE(game);
	// ...

	fsm_change_state(initial_state);

	return true;
}

void fsm_shutdown()
{
	if (init_status == INITIALISED)
	{
		fsm_change_state(NONE);

		// destroy the structs for all game states.
		DESTROY_STATE(mm);
		DESTROY_STATE(settings);
		DESTROY_STATE(game);
		// ...

		INIT_STATUS(UNINITIALISED);
	}
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

void fsm_exit()
{
	REQUIRE_INIT();

	exit(0);
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

void fsm_on_window_event(WindowEvent event)
{
	REQUIRE_INIT();

	if (current_state != NONE)
	{
		GameState* state = &states[current_state];

		switch (event.id)
		{
			case KEY_EVENT:
			{
				KeyEventData* data = (KeyEventData*)event.data;
				state->key_input(data->key, data->scancode, data->action, data->mods);
				break;
			}
			case MOUSE_BUTTON_EVENT:
			{
				MouseButtonEventData* data = (MouseButtonEventData*)event.data;
				state->mouse_button_input(data->button, data->action, data->mods);
				break;
			}
			case MOUSE_MOVE_EVENT:
			{
				MouseMoveEventData* data = (MouseMoveEventData*)event.data;
				state->mouse_movement_input(data->x, data->y, data->delta_x, data->delta_y);
				break;
			}
			default:
				break;
		}
	}
}