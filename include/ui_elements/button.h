// TODO:
// - ADD RIGHT CLICK!!!!!!!!
// - add comments

#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_log.h>
#include <stdbool.h>
#include "../inputstate.h"
#include "../helper_functions/helper_functions.h"

typedef struct Button Button;



// ========== create ==========
Button* button_create(
    SDL_Texture* texture_idle, 
    SDL_Texture* texture_hovered, 
    SDL_Texture* texture_pressed
);

// ========== destroy ==========
void button_destroy(void* button);

// ========== update ==========
void button_update(const InputState* input, void* button);

// ========== render ==========
void button_render(SDL_Renderer* renderer, void* button);

// ========== set ==========
void button_set_position(void* button, float x, float y);
void button_set_size(void* button, float width, float height);
void button_set_texture_idle(Button* button, SDL_Texture* texture);
void button_set_texture_hovered(Button* button, SDL_Texture* texture);
void button_set_texture_pressed(Button* button, SDL_Texture* texture);
void button_set_texture_all(Button* button, SDL_Texture* texture);

void button_set_on_press_func_arg0(Button* button, void (*func)());
void button_set_on_press_func_arg1(Button* button, void (*func)(void* arg1), void* arg1);
void button_set_on_press_func_arg2(Button* button, void (*func)(void* arg1, void* arg2), void* arg1, void* arg2);

// ========== get ==========
double button_get_x(const void* button);
double button_get_y(const void* button);
double button_get_width(const void* button);
double button_get_height(const void* button);
SDL_FRect button_get_frect(const void* button);
