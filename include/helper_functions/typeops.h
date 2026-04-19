#pragma once

#include "../inputstate.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

typedef struct TypeOps
{
    void (*destroy)(void* item);
    bool (*is_equal)(const void* item1, const void* item2);
    void (*render)(SDL_Renderer* renderer, const void* item);
    void (*update)(InputState* input, void* item);

    void (*set_pos)(void* item, float x, float y);
    void (*set_size)(void* item, float x, float y);

    float (*get_x)(const void* item);
    float (*get_y)(const void* item);
    float (*get_width)(const void* item);
    float (*get_height)(const void* item);
    SDL_FRect (*get_frect)(const void* item);
    SDL_Texture* (*get_texture)(const void* item);
} TypeOps;
