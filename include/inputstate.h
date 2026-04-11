// get the input from the player and store it in a state

#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <stdbool.h>
#include "helper_functions/error_handling.h"

typedef struct InputState InputState;



// ========== create ==========

/**
    * @brief create an InputState to store the input given by the user.
    * initialize with all values cleared
    * 
    * @return the allocated InputState
    */
InputState* input_create();



// ========== destroy ==========

/**
    * @brief destroy the allocated InputState
    * 
    * @param state the InputState to destroy
    */
void input_destroy(InputState* state);



// ========== reset ==========

/**
    * @brief reset the values of the state, should be called at the end of the frame
    * 
    * @param the InputState to reset
    */
void input_begin_frame(InputState* input);



// ========== update ==========

/**
    * @brief sets the values in the InputState given by the user
    * 
    * @param input the InputState to update
    * @param event the SDL_Event
    */
void input_update(InputState* input, const SDL_Event* event);

// ========== mouse ==========

/**
    * @brief get the mouse x position
    * 
    * @param input the InputState to read the values from
    *
    * @return the mouse x position
    */
double input_get_mouse_x(const InputState* input);

/**
    * @brief get the mouse y position
    * 
    * @param input the InputState to read the values from
    *
    * @return the mouse y position
    */
double input_get_mouse_y(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse left button is down
    */
bool input_get_mouse_left_down(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse left button is pressed
    */
bool input_get_mouse_left_pressed(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse left button is released
    */
bool input_get_mouse_left_released(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse right button is down
    */
bool input_get_mouse_right_down(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse right button is pressed
    */
bool input_get_mouse_right_pressed(const InputState* input);

/**
    * @brief get the state of the mouse button
    * 
    * @param input the InputState to read the values from
    *
    * @return true if the mouse right button is released
    */
bool input_get_mouse_right_released(const InputState* input);



// ========== keyboard ==========

/**
    * @brief gets the state of the keyboard keys
    * 
    * @param input the InputState to read the values from
    * @param scancode the key to check
    *
    * @return true if the desired key is down
    */
bool input_get_key_down(const InputState* input, const SDL_Scancode scancode);

/**
    * @brief gets the state of the keyboard keys
    * 
    * @param input the InputState to read the values from
    * @param scancode the key to check
    *
    * @return true if the desired key is pressed
    */
bool input_get_key_pressed(const InputState* input, const SDL_Scancode scancode);

/**
    * @brief gets the state of the keyboard keys
    * 
    * @param input the InputState to read the values from
    * @param scancode the key to check
    *
    * @return true if the desired key is released
    */
bool input_get_key_released(const InputState* input, const SDL_Scancode scancode);


