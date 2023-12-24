#ifndef LOG_H
#define LOG_H

#include "pp.h"

#include <stdio.h>

#define LOG_ERROR(...) \
	do { \
		fprintf(stderr, "[Core] Error: " __VA_ARGS__); \
		fprintf(stderr, " (%s, line %i)\n", __FILE__, __LINE__); \
	} while (0)

#define LOG_WARN(...) \
	do { \
		printf("[Core] Warning: " __VA_ARGS__); \
		printf(" (%s, line %i)\n", __FILE__, __LINE__); \
	} while (0)

#endif
