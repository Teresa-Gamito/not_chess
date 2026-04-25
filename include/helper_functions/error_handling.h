// used to verify invalid parameters
// exits the game in case of error
#pragma once

#include <stdbool.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <stdlib.h>

void verify(bool statement, const char* error_msg);
