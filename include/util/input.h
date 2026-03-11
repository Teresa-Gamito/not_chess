#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include "utils.h"

typedef struct InputState InputState;

InputState input_state_create();
void input_state_destroy(InputState *input_state);

void mouse_set_pos(InputState *input_state, Position pos);

void mouse_set_left_down(InputState *input_state, bool state);
void mouse_set_left_pressed(InputState *input_state, bool state);
void mouse_set_left_released(InputState *input_state, bool state);

void mouse_set_right_down(InputState *input_state, bool state);
void mouse_set_right_pressed(InputState *input_state, bool state);
void mouse_set_right_released(InputState *input_state, bool state);


/*
 * clears the state of all of the keyboard keys
 */
void keyboard_clear_state(InputState *input_state);

/*
 * sets a specific key as the given state
 */
void keyboard_set_key_down(InputState *input_state, SDL_Scancode code, bool state);

/*
 * sets a specific key as the given state
 */
void keyboard_set_key_pressed(InputState *input_state, SDL_Scancode code, bool state);

/*
 * sets a specific key as the given state
 */
void keyboard_set_key_released(InputState *input_state, SDL_Scancode code, bool state);


