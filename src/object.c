#include "../include/object.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>

struct Object {
    SDL_Texture* texture;
    SDL_FRect* frect;
};


// ========== create / destroy ==========

Object* object_create(
    SDL_Renderer* renderer,
    const double x,
    const double y,
    const char* texture_file_name)
{
    Object* object = (Object*)SDL_malloc(sizeof(Object));
    // set texture
    object->texture = NULL;
    object_update_texture(renderer, object, texture_file_name);
    // set frect position and size
    object->frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    object->frect->x = x;
    object->frect->y = y;
    SDL_GetTextureSize(object->texture, &object->frect->w, &object->frect->h);
    return object;
}

void object_destroy(Object* object)
{
    SDL_DestroyTexture(object->texture);
    SDL_free(object->frect);
    SDL_free(object);
}



// ========== set ==========

void object_set_position(Object* object, const double x, const double y)
{
    object->frect->x = x;
    object->frect->y = y;
}
void object_set_size(Object* object, const double width, const double height)
{
    object->frect->w = width;
    object->frect->h = height;
}
void object_set_scale(Object* object, const double scale_factor)
{
    SDL_GetTextureSize(object->texture, &object->frect->w, &object->frect->h);
    object->frect->w *= scale_factor;
    object->frect->h *= scale_factor;
}
void object_set_frect(Object* object, SDL_FRect* frect)
{
    object->frect = frect;
}
void object_update_texture(SDL_Renderer* renderer, Object* object, const char* image_file)
{
    if (object->texture) SDL_DestroyTexture(object->texture);
    SDL_Surface* surface = SDL_LoadPNG(image_file);
    object->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}


// get
float object_get_x(const Object* object)
{
    return object->frect->x;
}
float object_get_y(const Object* object)
{
    return object->frect->y;
}
float object_get_width(const Object* object)
{
    return object->frect->w;
}
float object_get_height(const Object* object)
{
    return object->frect->h;
}
SDL_FRect* object_get_frect(const Object* object)
{
    return object->frect;
}
SDL_Texture* object_get_texture(const Object* object)
{
    return object->texture;
}
