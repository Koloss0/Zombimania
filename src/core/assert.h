#ifndef ASSERT_H
#define ASSERT_H

#include "pp.h"

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, ...) \
	do { \
		if (!(cond)) \
		{ \
			fprintf(stderr, "[Core] Assertion failed: '" #cond "' (%s, line %i)\n", __FILE__, __LINE__); \
			 \
			if (NUM_ARGS(__VA_ARGS__) > 0) \
			{ \
				fprintf(stderr, "  Info: " __VA_ARGS__); \
				fprintf(stderr, "\n"); \
			} \
			exit(1); \
		} \
	} while (0)

#endif
