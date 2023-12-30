#ifndef INIT_H
#define INIT_H

#include "core/assert.h"
#include "core/log.h"

static bool init = false;

#define REQUIRE_INIT() \
		ASSERT(init, "module must be initialised before this function is called.")

#define WARN_INIT() do { \
		if (!init) \
			LOG_WARN("module should be initialised before any function is called."); \
		} while (0)

#define INIT_FUNC() do { \
		ASSERT(!init, "attempt to initialise module twice."); \
		init = true; \
		} while (0)

#define SHUTDOWN_FUNC() init = false

#endif