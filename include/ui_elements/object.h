#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../helper_functions/helper_functions.h"

// TODO:
// - add animations

typedef struct Object Object;

// ========== create ==========
Object* object_create(
    const double x,
    const double y,
    SDL_Texture* texture);

// ========== destroy ==========
void object_destroy(Object* object);

// ========== render ==========
void object_render(SDL_Renderer* renderer, Object* object);

// ========== set ==========
void object_set_position(Object* object, const double x, const double y);
void object_set_size(Object* object, const double width, const double height);
void object_set_texture(Object* object, SDL_Texture* texture);

// ========== get ==========
float object_get_x(const Object* object);
float object_get_y(const Object* object);
float object_get_width(const Object* object);
float object_get_height(const Object* object);
SDL_FRect* object_get_frect(const Object* object);
SDL_Texture* object_get_texture(const Object* object);
