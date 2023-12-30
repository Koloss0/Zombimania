#ifndef INIT_H
#define INIT_H

#include "core/assert.h"
#include "core/log.h"

enum InitStatus { UNINITIALISED, INITIALISED };
static enum InitStatus init_status = UNINITIALISED;

#define INIT_STATUS(state) init_status = state;

#define REQUIRE_INIT() \
		ASSERT(init_status == INITIALISED, \
			"module must be initialised to call this function.")

#define WARN_INIT() do { \
		if (init_status != INITIALISED) \
			LOG_WARN("module should be initialised before this function is called."); \
		} while (0)

#define REQUIRE_UNINIT() do { \
		ASSERT(init_status == UNINITIALISED, "module must not be initialised to call this function."); \
		} while (0)

#endif