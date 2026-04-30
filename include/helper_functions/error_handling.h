// used to verify invalid parameters
// exits the game in case of error
#pragma once

#include <stdbool.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>

void verify(bool statement, const char* error_msg);
void verify_size(int width, int height);
void verify_renderer(const SDL_Renderer* renderer);
