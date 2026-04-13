#include "../../include/ui_elements/object.h"

struct Object {
    SDL_Texture* texture;
    SDL_FRect* frect;
};



// ========== create ==========
Object* object_create(
    const double x,
    const double y,
    SDL_Texture* texture)
{
    Object* object = (Object*)SDL_malloc(sizeof(Object));

    object->texture = texture;

    float w = 0;
    float h = 0;

    if (texture != NULL)
    {
        SDL_GetTextureSize(object->texture, &w, &h);
    }

    object->frect = SDL_CreateFRect(x, y, w, h);

    return object;
}



// ========== destroy ==========
void object_destroy(Object* object)
{
    verify(object == NULL, "Object does not exist");

    SDL_DestroyFRect(object->frect);
    SDL_free(object);
}



// ========== render ==========
void object_render(SDL_Renderer* renderer, Object* object)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(object == NULL, "Object does not exist");

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
    verify(object == NULL, "Object does not exist");

    object->frect->x = x;
    object->frect->y = y;
}
void object_set_size(Object* object, const double width, const double height)
{
    verify(object == NULL, "Object does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    object->frect->w = width;
    object->frect->h = height;
}
void object_set_texture(Object* object, SDL_Texture* texture)
{
    verify(object == NULL, "Object does not exist");

    object->texture = texture;
}


// ========== set ==========
float object_get_x(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->frect->x;
}
float object_get_y(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->frect->y;
}
float object_get_width(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->frect->w;
}
float object_get_height(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->frect->h;
}
SDL_FRect* object_get_frect(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->frect;
}
SDL_Texture* object_get_texture(const Object* object)
{
    verify(object == NULL, "Object does not exist");

    return object->texture;
}
