// processes input from the player when pressing with a mouse
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <stdbool.h>
#include "helper_functions/helper_functions.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/function.h"
#include "helper_functions/typeops.h"
#include "helper_functions/vector.h"
#include "inputstate.h"

typedef struct Button Button;

Button* button_create(
    SDL_Texture* texture_idle,
    SDL_Texture* texture_hovered,
    SDL_Texture* texture_pressed
);
void button_destroy(Button* button);

void button_render(SDL_Renderer* renderer, const Button* button);
void button_update(const InputState* input, Button* button);

void button_set_position(Button* button, float x, float y);
void button_set_anchor(Button* button, float anchor_x, float anchor_y);
void button_set_size(Button* button, float width, float height);
void button_set_scale(Button* button, float scale);
void button_set_texture_idle(Button* button, SDL_Texture* texture);
void button_set_texture_hovered(Button* button, SDL_Texture* texture);
void button_set_texture_pressed(Button* button, SDL_Texture* texture);
void button_set_texture_all(Button* button, SDL_Texture* texture);

void button_set_on_click_fn(Button* button, MouseButton mouse_button, Function* function);

float button_get_x(const Button* button);
float button_get_y(const Button* button);
float button_get_width(const Button* button);
float button_get_height(const Button* button);
SDL_FRect button_get_frect(const Button* button);

TypeOps* button_ops();

void verify_button(const Button* button);
