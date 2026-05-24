#include "include/ui/ui_elements/sprite.h"

struct Sprite {
    SDL_Texture* texture;

    float x;
    float y;

    float width;
    float height;

    float anchor_x;
    float anchor_y;

    float scale;
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
    sprite->anchor_x = 0;
    sprite->anchor_y = 0;
    sprite->scale = 1;

    return sprite;
}

void sprite_destroy(Sprite* sprite)
{
    verify_sprite(sprite);

    SDL_free(sprite);
}

void sprite_render(SDL_Renderer* renderer, const Sprite* sprite)
{
    verify_renderer(renderer);
    verify_sprite(sprite);

    SDL_FRect frect = sprite_get_frect(sprite);
    SDL_RenderTexture(
        renderer,
        sprite->texture,
        NULL,
        &frect
    );
}

void sprite_set_position(Sprite* sprite, float x, float y)
{
    verify_sprite(sprite);

    sprite->x = x;
    sprite->y = y;
}
void sprite_set_anchor(Sprite* sprite, float anchor_x, float anchor_y)
{
    verify_sprite(sprite);

    sprite->anchor_x = anchor_x;
    sprite->anchor_y = anchor_y;
}
void sprite_set_size(Sprite* sprite, float width, float height)
{
    verify_sprite(sprite);
    verify_size(width, height);

    sprite->width = width;
    sprite->height = height;
}
void sprite_set_scale(Sprite* sprite, float scale)
{
    verify_sprite(sprite);

    sprite->scale = scale;
}
void sprite_set_texture(Sprite* sprite, SDL_Texture* texture)
{
    verify_sprite(sprite);

    sprite->texture = texture;
}

float sprite_get_x(const Sprite* sprite)
{
    verify_sprite(sprite);

    return sprite->x;
}
float sprite_get_y(const Sprite* sprite)
{
    verify_sprite(sprite);

    return sprite->y;
}
float sprite_get_width(const Sprite* sprite)
{
    verify_sprite(sprite);

    return sprite->width;
}
float sprite_get_height(const Sprite* sprite)
{
    verify_sprite(sprite);

    return sprite->height;
}
SDL_FRect sprite_get_frect(const Sprite* sprite)
{
    verify_sprite(sprite);

    SDL_FRect frect =
        {
            sprite->x * sprite->scale + sprite->anchor_x,
            sprite->y * sprite->scale + sprite->anchor_y,
            sprite->width * sprite->scale,
            sprite->height * sprite->scale,
        };
    return frect;
}
SDL_Texture* sprite_get_texture(const Sprite* sprite)
{
    verify_sprite(sprite);

    return sprite->texture;
}

void verify_sprite(const Sprite* sprite)
{
    verify(sprite == NULL, "Sprite does not exist");
}
