// TODO:
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
    double x, 
    double y, 
    void (*on_press)(),
    SDL_Texture* texture_idle, 
    SDL_Texture* texture_hovered, 
    SDL_Texture* texture_pressed
);



// ========== destroy ==========

void button_destroy(void* p);



// ========== update ==========

void button_update(InputState* input, Button* button);



// ========== render ==========

void button_render(SDL_Renderer* renderer, Button* button);



// ========== set ==========

void button_set_position(Button* button, double x, double y);
void button_set_size(Button* button, double width, double height);



// ========== get ==========

double button_get_x(const Button* button);
double button_get_y(const Button* button);
double button_get_width(const Button* button);
double button_get_height(const Button* button);
SDL_FRect* button_get_frect(const Button* button);
