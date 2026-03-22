#include "../../include/util/input.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>

struct MouseState
{
    double x;
    double y;

    bool left_down;
    bool left_pressed;
    bool left_released;

    bool right_down;
    bool right_pressed;
    bool right_released;
};

struct KeyboardState
{
    bool down[SDL_SCANCODE_COUNT];
    bool pressed[SDL_SCANCODE_COUNT];
    bool released[SDL_SCANCODE_COUNT];
};

struct InputState {
    MouseState* mouse;
    KeyboardState* key;
};



// ========== create / destroy ==========

InputState* input_state_create()
{
    InputState* input_state = (InputState*)SDL_malloc(sizeof(InputState));
    return input_state;
}

void input_state_destroy(InputState *input_state)
{
    SDL_free(input_state);
}

void input_begin_frame(InputState* input)
{
    mouse_clear_state(input->mouse);
    keyboard_clear_state(input->key);
}


// ========== mouse ==========

void mouse_clear_state(MouseState* mouse)
{
    mouse->x = 0.0;
    mouse->y = 0.0;
    mouse->left_down = false;
    mouse->left_pressed = false;
    mouse->left_released = false;
    mouse->right_down = false;
    mouse->right_pressed = false;
    mouse->right_released = false;
}

// set
void mouse_set_pos(MouseState* mouse, double x, double y)
{
    mouse->x = x;
    mouse->y = y;
}

void mouse_set_left_down(MouseState* mouse, bool state)
{
    mouse->left_down = state;
}
void mouse_set_left_pressed(MouseState* mouse, bool state)
{
    mouse->left_pressed = state;
}
void mouse_set_left_released(MouseState* mouse, bool state)
{
    mouse->left_released = state;
}

void mouse_set_right_down(MouseState* mouse, bool state)
{
    mouse->right_down = state;
}
void mouse_set_right_pressed(MouseState* mouse, bool state)
{
    mouse->right_pressed = state;
}
void mouse_set_right_released(MouseState* mouse, bool state)
{
    mouse->right_released = state;
}

// get
const double mouse_get_pos_x(const MouseState* mouse)
{
    return mouse->x;
}
const double mouse_get_pos_y(const MouseState* mouse)
{
    return mouse->y;
}

const bool mouse_get_left_down(const MouseState* mouse)
{
    return mouse->left_down;
}
const bool mouse_get_left_pressed(const MouseState* mouse)
{
    return mouse->left_pressed;
}
const bool mouse_get_left_released(const MouseState* mouse)
{
    return mouse->left_released;
}

const bool mouse_get_right_down(const MouseState* mouse)
{
    return mouse->right_down;
}
const bool mouse_get_right_pressed(const MouseState* mouse)
{
    return mouse->right_pressed;
}
const bool mouse_get_right_released(const MouseState* mouse)
{
    return mouse->right_released;
}


// sdl event
void mouse_event(MouseState* mouse, SDL_Event* event)
{
    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        mouse->x = event->motion.x;
        mouse->y = event->motion.y;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (!mouse->left_down) mouse->left_pressed = true;
            mouse->left_down = true;
        }
    }
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            mouse->left_down = false;
            mouse->left_released = true;
        }
    }
}

// ========== keyboard ==========

void keyboard_clear_state(KeyboardState* key)
{
    for (int i = 0; i < SDL_SCANCODE_COUNT + 1; i++)
    {
        key->down[i] = false;
        key->pressed[i] = false;
        key->released[i] = false;
    }
}

// set
void keyboard_set_key_down(KeyboardState* key, SDL_Scancode scancode, bool state)
{
    key->down[scancode] = state;
}
void keyboard_set_key_pressed(KeyboardState* key, SDL_Scancode scancode, bool state)
{
    key->pressed[scancode] = state;
}
void keyboard_set_key_released(KeyboardState* key, SDL_Scancode scancode, bool state)
{
    key->released[scancode] = state;
}

// get
const bool keyboard_get_key_down(const KeyboardState* key, SDL_Scancode scancode)
{
    return key->down[scancode];
}
const bool keyboard_get_key_pressed(const KeyboardState* key, SDL_Scancode scancode)
{
    return key->pressed[scancode];
}
const bool keyboard_get_key_released(const KeyboardState* key, SDL_Scancode scancode)
{
    return key->released[scancode];
}

// sdl event
void keyboard_event(KeyboardState* key, SDL_Event* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (!event->key.repeat && !key->down[event->key.scancode])
        {
            key->pressed[event->key.scancode] = true;
        }
        key->down[event->key.scancode] = true;
    }

    if (event->type == SDL_EVENT_KEY_UP)
    {
        key->down[event->key.scancode] = false;
        key->released[event->key.scancode] = true;
    }
}
