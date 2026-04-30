// renders a single sprite
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "include/helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"

typedef struct Sprite Sprite;

Sprite* sprite_create(SDL_Texture* texture);
void sprite_render(SDL_Renderer* renderer, const Sprite* sprite);
void sprite_set_position(Sprite* sprite, const float x, const float y);
void sprite_set_anchor(Sprite* sprite, float anchor_x, float anchor_y);
void sprite_set_size(Sprite* sprite, const float width, const float height);
void sprite_set_scale(Sprite* sprite, float scale);
float sprite_get_x(const Sprite* sprite);
float sprite_get_y(const Sprite* sprite);
float sprite_get_width(const Sprite* sprite);
float sprite_get_height(const Sprite* sprite);
SDL_FRect sprite_get_frect(const Sprite* sprite);
void sprite_set_texture(Sprite* sprite, SDL_Texture* texture);

TypeOps* sprite_ops();

void verify_sprite(const Sprite* sprite);
