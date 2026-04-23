#include "include/ui_elements/sprite.h"

struct Sprite {
    SDL_Texture* texture;
    float x;
    float y;
    float width;
    float height;
};

Sprite* sprite_create(SDL_Texture* texture)
{
    Sprite* sprite = (Sprite*)SDL_malloc(sizeof(Sprite));
    verify(sprite == NULL, "Sprite could not be created: malloc");

    sprite->texture = texture;

    float w = 0;
    float h = 0;

    if (texture != NULL)
    {
        SDL_GetTextureSize(sprite->texture, &w, &h);
    }

    sprite->x = 0;
    sprite->y = 0;
    sprite->width = w;
    sprite->height = h;

    return sprite;
}

void sprite_destroy(Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    SDL_free(sprite);
}

void sprite_render(SDL_Renderer* renderer, const Sprite* sprite)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(sprite == NULL, "Sprite does not exist");

    SDL_FRect frect =
        {
            sprite->x,
            sprite->y,
            sprite->width,
            sprite->height
        };

    SDL_RenderTexture(
        renderer,
        sprite->texture,
        NULL,
        &frect
    );
}

void sprite_set_position(Sprite* sprite, const float x, const float y)
{
    verify(sprite == NULL, "Sprite does not exist");

    sprite->x = x;
    sprite->y = y;
}
void sprite_set_size(Sprite* sprite, const float width, const float height)
{
    verify(sprite == NULL, "Sprite does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    sprite->width = width;
    sprite->height = height;
}
void sprite_set_texture(Sprite* sprite, SDL_Texture* texture)
{
    verify(sprite == NULL, "Sprite does not exist");

    sprite->texture = texture;
}

float sprite_get_x(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    return sprite->x;
}
float sprite_get_y(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    return sprite->y;
}
float sprite_get_width(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    return sprite->width;
}
float sprite_get_height(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    return sprite->height;
}
SDL_FRect sprite_get_frect(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    SDL_FRect frect =
        {
            sprite->x,
            sprite->y,
            sprite->width,
            sprite->height
        };
    return frect;
}
SDL_Texture* sprite_get_texture(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");

    return sprite->texture;
}



static void destroy(void* s)
{
    Sprite* sprite = (Sprite*)s;
    verify(sprite == NULL, "Sprite does not exist");

    sprite_destroy(sprite);
}
static TypeOps ops =
    {
        destroy
        // is_equal
    };
TypeOps* sprite_ops()
{
    return &ops;
}
