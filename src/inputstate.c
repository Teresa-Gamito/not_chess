#include "../include/inputstate.h"
#include <SDL3/SDL_scancode.h>

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
InputState* input_create()
{
    InputState* input = (InputState*)SDL_malloc(sizeof(InputState));
    input->mouse = (MouseState*)SDL_calloc(1, sizeof(MouseState));
    input->key = (KeyboardState*)SDL_calloc(1, sizeof(KeyboardState));
    return input;
}


// ========== destroy ==========
void input_destroy(InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    SDL_free(input->mouse);
    SDL_free(input->key);
    SDL_free(input);
}


// ========== clear state ==========
static void mouse_clear(MouseState* mouse)
{
    verify(mouse == NULL, "MouseState does not exist");

    mouse->left_pressed = false;
    mouse->left_released = false;

    mouse->right_pressed = false;
    mouse->right_released = false;
}
static void keyboard_clear(KeyboardState* key)
{
    verify(key == NULL, "KeyboardState does not exist");

    for (int i = 0; i < SDL_SCANCODE_COUNT + 1; i++)
    {
        key->pressed[i] = false;
        key->released[i] = false;
    }
}
void input_begin_frame(InputState* input)
{
    mouse_clear(input->mouse);
    keyboard_clear(input->key);
}


// ========== update ==========
static void mouse_update(MouseState* mouse, const SDL_Event* event)
{
    verify(mouse == NULL, "MouseState does not exist");
    verify(event == NULL, "SDL_Event does not exist");

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
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            if (!mouse->right_down) mouse->right_pressed = true;
            mouse->right_down = true;
        }
    }
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            mouse->left_down = false;
            mouse->left_released = true;
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            mouse->right_down = false;
            mouse->right_released = true;
        }
    }
}
static void keyboard_update(KeyboardState* key, const SDL_Event* event)
{
    verify(key == NULL, "KeyboardState does not exist");
    verify(event == NULL, "SDL_Event does not exist");

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
    verify(input == NULL, "InputState does not exist");
    verify(event == NULL, "SDL_Event does not exist");

    keyboard_update(input->key, event);
    mouse_update(input->mouse, event);
}


// ========== mouse ==========
double input_get_mouse_x(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->x;
}
double input_get_mouse_y(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->y;
}

bool input_get_mouse_left_down(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->left_down;
}
bool input_get_mouse_left_pressed(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->left_pressed;
}
bool input_get_mouse_left_released(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->left_released;
}
bool input_get_mouse_right_down(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->right_down;
}
bool input_get_mouse_right_pressed(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->right_pressed;
}
bool input_get_mouse_right_released(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");

    return input->mouse->right_released;
}


// ========== keyboard ==========
bool input_get_key_down(const InputState* input, const SDL_Scancode scancode)
{
    verify(input == NULL, "InputState does not exist");
    verify(scancode < 0 || scancode >= SDL_SCANCODE_COUNT, "InputState does not exist");

    return input->key->down[scancode];
}
bool input_get_key_pressed(const InputState* input, const SDL_Scancode scancode)
{
    verify(input == NULL, "InputState does not exist");
    verify(scancode < 0 || scancode >= SDL_SCANCODE_COUNT, "InputState does not exist");

    return input->key->pressed[scancode];
}
bool input_get_key_released(const InputState* input, const SDL_Scancode scancode)
{
    verify(input == NULL, "InputState does not exist");
    verify(scancode < 0 || scancode >= SDL_SCANCODE_COUNT, "InputState does not exist");

    return input->key->released[scancode];
}
