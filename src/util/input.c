#include "../../include/util/input.h"

struct InputState {
    /* mouse */
    Position mouse_pos;
    bool mouse_left_down;
    bool mouse_left_pressed;
    bool mouse_left_released;
    bool mouse_right_down;
    bool mouse_right_pressed;
    bool mouse_right_released;

    /* keyboard */
    bool key_down[SDL_SCANCODE_COUNT];
    bool key_pressed[SDL_SCANCODE_COUNT];
    bool key_released[SDL_SCANCODE_COUNT];
};
