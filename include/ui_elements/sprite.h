#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "../helper_functions/helper_functions.h"
#include "../helper_functions/typeops.h"

typedef struct Sprite {
    SDL_Texture* texture;
    float x;
    float y;
    float width;
    float height;
} Sprite;

Sprite* sprite_create(SDL_Texture* texture);
TypeOps* sprite_ops();
void sprite_set_texture(Sprite* sprite, SDL_Texture* texture);

