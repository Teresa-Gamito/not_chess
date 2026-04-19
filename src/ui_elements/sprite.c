#include "../../include/ui_elements/sprite.h"

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



void sprite_destroy(void* sprite)
{
    Sprite* s = (Sprite*)sprite;
    verify(s == NULL, "Sprite does not exist");

    SDL_free(s);
}



bool sprite_equals(const void* sprite1, const void* sprite2)
{
    Sprite* s1 = (Sprite*)sprite1;
    Sprite* s2 = (Sprite*)sprite2;

    return s1 == s2;
}



void sprite_render(SDL_Renderer* renderer, const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(spr == NULL, "Sprite does not exist");

    SDL_FRect frect =
        {
            spr->x,
            spr->y,
            spr->width,
            spr->height
        };

    SDL_RenderTexture(
        renderer,
        spr->texture,
        NULL,
        &frect
    );
}



void sprite_set_position(void* sprite, const float x, const float y)
{
    Sprite* spr = (Sprite*)sprite;
    verify(spr == NULL, "Sprite does not exist");

    spr->x = x;
    spr->y = y;
}
void sprite_set_size(void* sprite, const float width, const float height)
{
    Sprite* spr = (Sprite*)sprite;
    verify(spr == NULL, "Sprite does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    spr->width = width;
    spr->height = height;
}
void sprite_set_texture(Sprite* sprite, SDL_Texture* texture)
{
    verify(sprite == NULL, "Sprite does not exist");

    sprite->texture = texture;
}



float sprite_get_x(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(spr == NULL, "Sprite does not exist");

    return spr->x;
}
float sprite_get_y(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(sprite == NULL, "Sprite does not exist");

    return spr->y;
}
float sprite_get_width(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(spr == NULL, "Sprite does not exist");

    return spr->width;
}
float sprite_get_height(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(sprite == NULL, "Sprite does not exist");

    return spr->height;
}
SDL_FRect sprite_get_frect(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(spr == NULL, "Sprite does not exist");

    SDL_FRect frect =
        {
            spr->x,
            spr->y,
            spr->width,
            spr->height
        };
    return frect;
}
SDL_Texture* sprite_get_texture(const void* sprite)
{
    Sprite* spr = (Sprite*)sprite;
    verify(sprite == NULL, "Sprite does not exist");

    return spr->texture;
}



static TypeOps ops =
    {
        sprite_destroy,
        sprite_equals,
        sprite_render,
        NULL,
        sprite_set_position,
        sprite_set_size,
        sprite_get_x,
        sprite_get_y,
        sprite_get_width,
        sprite_get_height,
        sprite_get_frect,
        sprite_get_texture
    };
TypeOps* sprite_ops()
{
    return &ops;
}
