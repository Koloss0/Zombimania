#include "txt.h"
#include "core/log.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

char* io_load_txt(const char* path)
{
	FILE* fp = fopen(path, "rb");

	if (!fp)
	{
		LOG_ERROR("failed to open txt file '%s'", path);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);

	ssize_t size = ftell(fp);

	if (size <= 0)
	{
		LOG_ERROR("could not get size of txt file '%s'", path);
		return NULL;
	}

	rewind(fp);

	char* buf = malloc((size_t)size + 1);
	int bytes = fread(buf, 1, (size_t)size, fp);
	
	if (bytes != (size_t)size)
	{
		LOG_ERROR("could not read txt file '%s'", path);
		free(buf);
		return NULL;
	}

	buf[size] = '\0';

	return buf;
}
