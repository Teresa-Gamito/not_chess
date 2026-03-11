#include "../../include/util/button.h"
#include <stdlib.h>

struct Button
{
    Position pos;
    Size size;
    bool is_hovered;
};

Button *button_create(Position pos, Size size)
{
    Button *button = malloc(sizeof(Button));
    if (!button)
    {
        perror("Could not create button: malloc");
        exit(1);
    }
    button->pos = pos;
    button->size = size;
    button->is_hovered = false;
    return button;
}

void button_destroy(Button *button)
{
    free(button);
}

void button_set_position(Button *button, Position pos)
{
    button->pos = pos;
}

void button_set_size(Button *button, Size size)
{
    button->size = size;
}

void button_set_is_hovered(Button *button, bool state)
{
    button->is_hovered = state;
}

Position button_get_position(Button button)
{
    return button.pos;
}

Size button_get_size(Button button)
{
    return button.size;
}
bool button_get_is_hovered(Button button)
{
    return button.is_hovered;
}
