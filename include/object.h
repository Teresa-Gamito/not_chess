#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

typedef struct Object Object;

// create / destroy
Object* object_create(
    SDL_Renderer* renderer,
    const double x,
    const double y,
    const char* texture_file_name
);
void object_destroy(Object* object);


// set
void object_set_position(Object* object, const double x, const double y);
void object_set_size(Object* object, const double width, const double height);
void object_set_scale(Object* object, const double scale_factor);
void object_set_frect(Object* object, SDL_FRect* frect);

void object_update_texture(SDL_Renderer* renderer, Object* object, const char* image_file);


// get
float object_get_x(const Object* object);
float object_get_y(const Object* object);
float object_get_width(const Object* object);
float object_get_height(const Object* object);
SDL_FRect* object_get_frect(const Object* object);

SDL_Texture* object_get_texture(const Object* object);
