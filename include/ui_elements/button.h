// TODO:
// - add comments

#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <stdbool.h>
#include "../helper_functions/typeops.h"
#include "../inputstate.h"
#include "../helper_functions/helper_functions.h"

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
void button_set_size(Button* button, float width, float height);
void button_set_texture_idle(Button* button, SDL_Texture* texture);
void button_set_texture_hovered(Button* button, SDL_Texture* texture);
void button_set_texture_pressed(Button* button, SDL_Texture* texture);
void button_set_texture_all(Button* button, SDL_Texture* texture);

void button_set_on_left_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* l_arg1, 
    void* l_arg2);
void button_set_on_right_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* r_arg1, 
    void* r_arg2);
void button_set_on_middle_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* m_arg1, 
    void* m_arg2);

float button_get_x(const Button* button);
float button_get_y(const Button* button);
float button_get_width(const Button* button);
float button_get_height(const Button* button);
SDL_FRect button_get_frect(const Button* button);

TypeOps* button_ops();

