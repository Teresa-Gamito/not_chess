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

    float w, h;
    if (texture == NULL)
    {
        w = 0;
        h = 0;
    }
    else 
    {
        SDL_GetTextureSize(object->texture, &w, &h);
    }
    object->frect = SDL_CreateFRect(x, y, w, h);

    return object;
}



// ========== destroy ==========
void object_destroy(Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist, could not destroy");
        return;
    }

    SDL_free(object->frect);
    SDL_free(object);
}



// ========== render ==========
void object_render(SDL_Renderer* renderer, Object* object)
{
    if (renderer == NULL)
    {
        SDL_Log("Renderer does not exist");
        return;
    }
    if (object == NULL)
    {
        SDL_Log("Object does not exist, could not be rendered");
        return;
    }

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
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return;
    }

    object->frect->x = x;
    object->frect->y = y;
}
void object_set_size(Object* object, const double width, const double height)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return;
    }
    if (width < 0 || height < 0)
    {
        SDL_Log("Width or height have to be positive");
        return;
    }

    object->frect->w = width;
    object->frect->h = height;
}
void object_set_texture(Object* object, SDL_Texture* texture)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return;
    }

    object->texture = texture;
}


// ========== set ==========
float object_get_x(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return 0;
    }
    return object->frect->x;
}
float object_get_y(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return 0;
    }
    return object->frect->y;
}
float object_get_width(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return 0;
    }
    return object->frect->w;
}
float object_get_height(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return 0;
    }
    return object->frect->h;
}
SDL_FRect* object_get_frect(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return NULL;
    }
    return object->frect;
}
SDL_Texture* object_get_texture(const Object* object)
{
    if (object == NULL)
    {
        SDL_Log("Object does not exist");
        return NULL;
    }
    return object->texture;
}

