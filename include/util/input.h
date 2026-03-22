#pragma once

#include <SDL3/SDL_events.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

typedef struct InputState InputState;
typedef struct MouseState MouseState;
typedef struct KeyboardState KeyboardState;

InputState* input_state_create();
void input_state_destroy(InputState *input_state);

void input_begin_frame(InputState* input);

// ========== mouse ==========
void mouse_clear_state(MouseState* mouse);

// set
void mouse_set_pos(MouseState* mouse, double x, double y);

void mouse_set_left_down(MouseState* mouse, bool state);
void mouse_set_left_pressed(MouseState* mouse, bool state);
void mouse_set_left_released(MouseState* mouse, bool state);

void mouse_set_right_down(MouseState* mouse, bool state);
void mouse_set_right_pressed(MouseState* mouse, bool state);
void mouse_set_right_released(MouseState* mouse, bool state);

// get
const double mouse_get_pos_x(const MouseState* mouse);
const double mouse_get_pos_y(const MouseState* mouse);

const bool mouse_get_left_down(const MouseState* mouse);
const bool mouse_get_left_pressed(const MouseState* mouse);
const bool mouse_get_left_released(const MouseState* mouse);

const bool mouse_get_right_down(const MouseState* mouse);
const bool mouse_get_right_pressed(const MouseState* mouse);
const bool mouse_get_right_released(const MouseState* mouse);

// sdl event
void mouse_event(MouseState* mouse, SDL_Event* event);



// ========== keyboard ==========
void keyboard_clear_state(KeyboardState* key);

// set
void keyboard_set_key_down(KeyboardState* key, SDL_Scancode scancode, bool state);
void keyboard_set_key_pressed(KeyboardState* key, SDL_Scancode scancode, bool state);
void keyboard_set_key_released(KeyboardState* key, SDL_Scancode scancode, bool state);

// get
const bool keyboard_get_key_down(const KeyboardState* key, SDL_Scancode scancode);
const bool keyboard_get_key_pressed(const KeyboardState* key, SDL_Scancode scancode);
const bool keyboard_get_key_released(const KeyboardState* key, SDL_Scancode scancode);

// sdl event
void keyboard_event(KeyboardState* key, SDL_Event* event);



