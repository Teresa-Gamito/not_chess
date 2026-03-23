#include "../../include/util/button.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

struct Button
{
    SDL_FRect* frect;
    bool is_hovered;
    

    // TODO:
    // execute this command when pressed
    // void (*execute) ();
};

Button* button_create(double x, double y, double width, double height)
{
    Button* button = (Button*)SDL_malloc(sizeof(Button));
    if (!button)
    {
        // TODO: throw error
    }
    SDL_FRect *frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    frect->x = x;
    frect->y = y;
    frect->w = width;
    frect->h = height;
    button->frect = frect;
    button->is_hovered = false;
    return button;
}

void button_destroy(Button* button)
{
    if (button->frect) SDL_free(button->frect);
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

void button_set_frect(Button* button, SDL_FRect* frect)
{
    if (button->frect) SDL_free(button->frect);
    button->frect = frect;
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

SDL_FRect* button_get_frect(Button* button)
{
    return button->frect;
}

const bool button_is_hovered(const Button *button, double x, double y)
{
    if (x < button->frect->x) return false;
    if (x > button->frect->x + button->frect->w) return false;
    if (y < button->frect->y) return false;
    if (y > button->frect->y + button->frect->h) return false;
    return true;
}
