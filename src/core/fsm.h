#ifndef FSM_H
#define FSM_H

#include "states/states.h"

#include <stdbool.h>

bool fsm_init(GameStateID initial_state);
void fsm_shutdown();

void fsm_queue_next_state(GameStateID state);
void fsm_change_state(GameStateID state);
void fsm_update(double delta);

#endif
