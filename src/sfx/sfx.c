#include "sfx.h"

#include "core/init.h"
#include "core/log.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include <stdbool.h>

static ma_engine engine;

bool sfx_init()
{
	REQUIRE_UNINIT();
	INIT_STATUS(UNINITIALISED);

	ma_result result = ma_engine_init(NULL, &engine);

	if (result != MA_SUCCESS)
	{
		LOG_ERROR("failed to initialise SFX: failed to initalise MiniAudio");
		sfx_shutdown();
		return false;
	}

	INIT_STATUS(INITIALISED);

	return true;
}

void sfx_shutdown()
{
	if (init_status == INITIALISED)
	{
		ma_engine_uninit(&engine);

		INIT_STATUS(UNINITIALISED);
	}
}

bool sfx_load_sound(const char* path, ma_sound *sound)
{
	REQUIRE_INIT();

	ma_result result = ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);
	return result == MA_SUCCESS;
}

void sfx_play_sound(const char* path)
{
	REQUIRE_INIT();

	ma_engine_play_sound(&engine, path, NULL);
}