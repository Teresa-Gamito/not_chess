#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include "helper_functions/global_variables.h"
#include "helper_functions/error_handling.h"

typedef struct Sound Sound;

Sound* sound_load(const char* path);
void sound_unload(Sound* sound);
void sound_play(const Sound* sound);
