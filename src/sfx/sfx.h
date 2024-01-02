#ifndef SFX_H
#define SFX_H

#include "miniaudio/miniaudio.h"

#include <stdbool.h>

bool sfx_init();
void sfx_shutdown();

bool sfx_load_sound(const char* path, ma_sound* sound);
void sfx_play_sound(const char* path);

#endif