#include "../../include/util/button.h"

struct Button
{
    SDL_FRect* frect;
    bool is_hovered;
    
    // execute this command when pressed
    // void (*execute) ();
};

Button* button_create(double x, double y, double width, double height)
{
    Button* button = (Button*)SDL_malloc(sizeof(Button));
    if (!button)
    {
        // throw error
    }
    button_set_position(button, x, y);
    button_set_size(button, width, height);
    button_set_is_hovered(button, false);
    return button;
}

void button_set_frect(Button* button, SDL_FRect* frect)
{
    button->frect = frect;
}

void button_destroy(Button* button)
{
    SDL_free(button);
}

void button_set_position(Button* button, double x, double y)
{
    button->frect->x = x;
    button->frect->y = x;
}

void button_set_size(Button* button, double width, double height)
{
    button->frect->w = width;
    button->frect->h = height;
}

void button_set_is_hovered(Button* button, bool state)
{
    button->is_hovered = state;
}



SDL_FRect* button_get_frect(Button* button)
{
    return button->frect;
}

const double button_get_x(const Button* button)
{
    return button->frect->x;
}

const double button_get_y(const Button* button)
{
    return button->frect->y;
}

const double button_get_width(const Button* button)
{
    return button->frect->w;
}

const double button_get_height(const Button* button)
{
    return button->frect->h;
}

const bool button_get_is_hovered(const Button* button)
{
    return button->is_hovered;
}
