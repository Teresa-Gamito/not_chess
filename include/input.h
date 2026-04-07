// get the input from the player and store it in a state
// TODO: Add comments
#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

typedef struct InputState InputState;

InputState* input_state_create();
void input_state_destroy(InputState* state);

void input_begin_frame(InputState* input);

void input_update(InputState* input, const SDL_Event* event);

// ========== mouse ==========

const double input_get_mouse_x(const InputState* input);
const double input_get_mouse_y(const InputState* input);

const bool input_get_mouse_left_down(const InputState* input);
const bool input_get_mouse_left_pressed(const InputState* input);
const bool input_get_mouse_left_released(const InputState* input);

const bool input_get_mouse_right_down(const InputState* input);
const bool input_get_mouse_right_pressed(const InputState* input);
const bool input_get_mouse_right_released(const InputState* input);



// ========== keyboard ==========

const bool input_get_key_down(const InputState* input, const SDL_Scancode scancode);
const bool input_get_key_pressed(const InputState* input, const SDL_Scancode scancode);
const bool input_get_key_released(const InputState* input, const SDL_Scancode scancode);


