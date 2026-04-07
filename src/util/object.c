#include "../../include/util/object.h"

struct Object {
    SDL_Texture* texture;
    SDL_FRect* frect;
};


// ========== create ==========
Object* object_create(
    SDL_Renderer* renderer,
    const double x,
    const double y,
    const char* texture_file_name)
{
    Object* object = (Object*)SDL_malloc(sizeof(Object));
    // set texture
    object->texture = NULL;
    object_set_texture(renderer, object, texture_file_name);
    // set frect position and size
    object->frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    object->frect->x = x;
    object->frect->y = y;
    SDL_GetTextureSize(object->texture, &object->frect->w, &object->frect->h);
    return object;
}


// ========== destroy ==========
void object_destroy(Object* object)
{
    SDL_DestroyTexture(object->texture);
    SDL_free(object->frect);
    SDL_free(object);
}


// ========== render ==========
void object_render(SDL_Renderer* renderer, Object* object)
{
    SDL_RenderTexture(
        renderer,
        object->texture,
        NULL,
        object->frect
    );
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
    if (object->frect) SDL_free(object->frect);
    object->frect = frect;
}
void object_set_texture(SDL_Renderer* renderer, Object* object, const char* image_file)
{
    if (object->texture) SDL_DestroyTexture(object->texture);
    SDL_Surface* surface = SDL_LoadPNG(image_file);
    object->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}


// ========== set ==========
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

bool objects_intersect(const Object* object1, const Object* object2)
{
    if (object1->frect->x > object2->frect->x + object2->frect->w) return false;
    if (object1->frect->x + object1->frect->w < object2->frect->x) return false;
    if (object1->frect->y > object2->frect->y + object2->frect->h) return false;
    if (object1->frect->y + object1->frect->h < object2->frect->y) return false;
    return true;
}
