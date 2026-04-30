// get the input from the player and store it in a state
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <stdbool.h>
#include "include/helper_functions/error_handling.h"

typedef struct InputState InputState;

typedef enum MouseButton
{
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,

    MOUSE_BUTTON_COUNT
} MouseButton;

InputState* input_create();
void input_destroy(InputState* state);

void input_begin_frame(InputState* input);
void input_update(InputState* input, const SDL_Event* event);

float mouse_get_x(const InputState* input);
float mouse_get_y(const InputState* input);
bool mouse_get_down(const InputState* input, MouseButton button);
bool mouse_get_pressed(const InputState* input, MouseButton button);
bool mouse_get_released(const InputState* input, MouseButton button);
float mouse_get_wheel(const InputState* input);

bool keyboard_get_down(const InputState* input, SDL_Scancode key);
bool keyboard_get_pressed(const InputState* input, SDL_Scancode key);
bool keyboard_get_released(const InputState* input, SDL_Scancode key);

void verify_input(const InputState* input);
void verify_event(const SDL_Event* event);
