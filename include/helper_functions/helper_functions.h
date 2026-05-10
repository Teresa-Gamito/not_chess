// other functions
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_rect.h>
#include "include/helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"


SDL_Texture* SDL_CreateTextureFromPNG(SDL_Renderer* renderer, const char* png_path);

bool rect_intersects_rect(const SDL_FRect* frect1, const SDL_FRect* frect2);
bool point_intersects_rect(double x, double y, const SDL_FRect* frect);

void log_error(const char* error_message);

SDL_FRect* SDL_CreateFRect(double x, double y, double width, double height);
void SDL_DestroyFRect(SDL_FRect* frect);

bool is_within(int value, int min, int max);
bool is_even(int x);
int sign(int value);

TypeOps* SDL_Texture_ops();
TypeOps* default_ops();

void app_quit(void* null1, void* null2);
