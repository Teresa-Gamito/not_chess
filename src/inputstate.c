#include "include/inputstate.h"

typedef struct MouseState
{
    // position
    double x;
    double y;
    // left button
    bool down[MOUSE_BUTTON_COUNT];
    bool pressed[MOUSE_BUTTON_COUNT];
    bool released[MOUSE_BUTTON_COUNT];
    // mouse wheel
    float wheel;
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

static void verify_keyboard_key(SDL_Scancode key);
static void verify_mouse_button(MouseButton button);

InputState* input_create()
{
    InputState* input = SDL_malloc(sizeof(InputState));
    verify(input == NULL, "InputState could not be created: malloc");
    input->mouse = SDL_calloc(1, sizeof(MouseState));
    verify(input == NULL, "MouseState could not be created: calloc");
    input->key = SDL_calloc(1, sizeof(KeyboardState));
    verify(input == NULL, "KeyboardState could not be created: calloc");
    return input;
}

void input_destroy(InputState* input)
{
    verify_input(input);

    SDL_free(input->mouse);
    SDL_free(input->key);
    SDL_free(input);
}

static void mouse_clear(MouseState* mouse)
{
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        mouse->pressed[i] = false;
        mouse->released[i] = false;
    }
    mouse->wheel = 0;
}
static void keyboard_clear(KeyboardState* key)
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
    {
        key->pressed[i] = false;
        key->released[i] = false;
    }
}
void input_begin_frame(InputState* input)
{
    verify_input(input);

    mouse_clear(input->mouse);
    keyboard_clear(input->key);
}


// ========== update ==========
static void mouse_update(MouseState* mouse, const SDL_Event* event)
{
    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        mouse->x = event->motion.x;
        mouse->y = event->motion.y;
    }

    if (event->type == SDL_EVENT_MOUSE_WHEEL)
    {
        mouse->wheel = event->wheel.y;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (!mouse->down[MOUSE_LEFT]) mouse->down[MOUSE_LEFT] = true;
            mouse->down[MOUSE_LEFT] = true;
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            if (!mouse->down[MOUSE_RIGHT]) mouse->down[MOUSE_RIGHT] = true;
            mouse->down[MOUSE_RIGHT] = true;
        }
        if (event->button.button == SDL_BUTTON_MIDDLE)
        {
            if (!mouse->down[MOUSE_MIDDLE]) mouse->down[MOUSE_MIDDLE] = true;
            mouse->down[MOUSE_MIDDLE] = true;
        }
        return;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            mouse->down[MOUSE_LEFT] = false;
            mouse->released[MOUSE_LEFT] = true;
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            mouse->down[MOUSE_RIGHT] = false;
            mouse->released[MOUSE_RIGHT] = true;
        }
        if (event->button.button == SDL_BUTTON_MIDDLE)
        {
            mouse->down[MOUSE_MIDDLE] = false;
            mouse->released[MOUSE_MIDDLE] = true;
        }
        return;
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
        return;
    }

    if (event->type == SDL_EVENT_KEY_UP)
    {
        key->down[event->key.scancode] = false;
        key->released[event->key.scancode] = true;
        return;
    }
}
void input_update(InputState* input, const SDL_Event* event)
{
    verify_input(input);
    verify_event(event);

    keyboard_update(input->key, event);
    mouse_update(input->mouse, event);
}


// ========== mouse ==========
float mouse_get_x(const InputState* input)
{
    verify_input(input);

    return input->mouse->x;
}
float mouse_get_y(const InputState* input)
{
    verify_input(input);

    return input->mouse->y;
}
bool mouse_get_down(const InputState* input, MouseButton button)
{
    verify_input(input);
    verify_mouse_button(button);

    return input->mouse->down[button];
}
bool mouse_get_pressed(const InputState* input, MouseButton button)
{
    verify_input(input);
    verify_mouse_button(button);

    return input->mouse->pressed[button];
}
bool mouse_get_released(const InputState* input, MouseButton button)
{
    verify_input(input);
    verify_mouse_button(button);

    return input->mouse->released[button];
}
float mouse_get_wheel(const InputState* input)
{
    verify_input(input);

    return input->mouse->wheel;
}


// ========== keyboard ==========
bool keyboard_get_down(const InputState* input, SDL_Scancode key)
{
    verify_input(input);
    verify_keyboard_key(key);

    return input->key->down[key];
}
bool keyboard_get_pressed(const InputState* input, SDL_Scancode key)
{
    verify_input(input);
    verify_keyboard_key(key);

    return input->key->pressed[key];
}
bool keyboard_get_released(const InputState* input, SDL_Scancode key)
{
    verify_input(input);
    verify_keyboard_key(key);

    return input->key->released[key];
}



static void verify_keyboard_key(SDL_Scancode key)
{
    verify(key < 0 || key >= SDL_SCANCODE_COUNT, "Invalid keyboard key");
}
static void verify_mouse_button(MouseButton button)
{
    verify(button < 0 || button >= MOUSE_BUTTON_COUNT, "Invalid mouse button");
}
void verify_input(const InputState* input)
{
    verify(input == NULL, "InputState does not exist");
}
void verify_event(const SDL_Event* event)
{
    verify(event == NULL, "SDL_Event does not exist");
}
