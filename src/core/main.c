#include "core.h"

int main()
{
	if (!core_init())
		return -1;
	
	core_run();

	core_shutdown();
	
	return 0;
}