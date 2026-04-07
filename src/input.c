#include "../include/input.h"
// PERF:

typedef struct MouseState
{
    // position
    double x;
    double y;
    // left button
    bool left_down;
    bool left_pressed;
    bool left_released;
    // right button
    bool right_down;
    bool right_pressed;
    bool right_released;
} MouseState;

typedef struct KeyboardState
{
    // keyboard
    bool down[SDL_SCANCODE_COUNT];
    bool pressed[SDL_SCANCODE_COUNT];
    bool released[SDL_SCANCODE_COUNT];
} KeyboardState;

struct InputState {
    MouseState* mouse;
    KeyboardState* key;
};



// ========== create ==========
InputState* input_state_create()
{
    InputState* input = (InputState*)SDL_malloc(sizeof(InputState));
    input->mouse = (MouseState*)SDL_malloc(sizeof(MouseState));
    input->key = (KeyboardState*)SDL_malloc(sizeof(KeyboardState));
    input_begin_frame(input);
    return input;
}


// ========== destroy ==========
void input_state_destroy(InputState* input)
{
    SDL_free(input);
}


// ========== clear state ==========
static void mouse_clear_state(MouseState* mouse)
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
static void keyboard_clear_state(KeyboardState* key)
{
    for (int i = 0; i < SDL_SCANCODE_COUNT + 1; i++)
    {
        key->down[i] = false;
        key->pressed[i] = false;
        key->released[i] = false;
    }
}
void input_begin_frame(InputState* input)
{
    mouse_clear_state(input->mouse);
    keyboard_clear_state(input->key);
}


// ========== update ==========
static void mouse_update(MouseState* mouse, const SDL_Event* event)
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
static void keyboard_update(KeyboardState* key, const SDL_Event* event)
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
void input_update(InputState* input, const SDL_Event* event)
{
    keyboard_update(input->key, event);
    mouse_update(input->mouse, event);
}


// ========== mouse ==========
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


// ========== keyboard ==========
const bool input_get_key_down(const InputState* input, const SDL_Scancode scancode)
{
    return input->key->down[scancode];
}
const bool input_get_key_pressed(const InputState* input, const SDL_Scancode scancode)
{
    return input->key->pressed[scancode];
}
const bool input_get_key_released(const InputState* input, const SDL_Scancode scancode)
{
    return input->key->released[scancode];
}
