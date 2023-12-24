#include <stdio.h>
#include "config.h"

int main()
{
	printf("%s v%d.%d - Compiled on %s UTC.\n", PROJECT_NAME, ZBM_VERSION_MAJOR, ZBM_VERSION_MINOR, TIMESTAMP_UTC);

	return 0;
}
