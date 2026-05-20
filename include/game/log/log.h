#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdarg.h>
#include "helper_functions/error_handling.h"
#include "helper_functions/helper_functions.h"
#include "helper_functions/vector.h"

typedef struct GameLog GameLog;

GameLog* gamelog_create();
void gamelog_destroy(GameLog* log);

void gamelog_add(GameLog* log, const char* msg, ...);
const char* gamelog_get(const GameLog *log);

void verify_gamelog(const GameLog* log);
