#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

typedef struct Button Button;

// create / destroy
Button* button_create(double x, double y, double width, double height);
void button_destroy(Button* button);

// set
void button_set_frect(Button* button, SDL_FRect* frect);
void button_set_position(Button* button, double x, double y);
void button_set_size(Button* button, double width, double height);
void button_set_is_hovered(Button* button, bool state);

// get
SDL_FRect* button_get_frect(Button* button);
const double button_get_x(const Button* button);
const double button_get_y(const Button* button);
const double button_get_width(const Button* button);
const double button_get_height(const Button* button);
const bool button_get_is_hovered(const Button* button);
