#include "../include/window.h"

struct Window {
    SDL_FRect* frect;
    WindowContent content;
};

Window* window_create(
    const double x, 
    const double y, 
    const double width, 
    const double height, 
    const WindowContent content)
{
    Window* window = (Window*)SDL_malloc(sizeof(Window));
    SDL_FRect* frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    if (!window || !frect)
    {
        // TODO throw error
    }
    window_set_frect(window, frect);
    window_set_position(window, x, y);
    window_set_size(window, width, height);
    window_set_content(window, content);
    return window;
}

void window_destroy(Window* window)
{
    if (!window) 
    {
        // TODO throw error
    }
    SDL_free(window);
}



void window_set_position(Window* window, const double x, const double y)
{
    window->frect->x = x;
    window->frect->y = y;
}

void window_set_size(Window* window, const double width, const double height)
{
    window->frect->w = width;
    window->frect->h = height;
}

void window_set_content(Window* window, const WindowContent content)
{
    window->content = content;
}

void window_set_frect(Window* window, SDL_FRect* frect)
{
    window->frect = frect;
}



double window_get_x(const Window* window)
{
    return window->frect->x;
}

double window_get_y(const Window* window)
{
    return window->frect->y;
}

double window_get_width(const Window* window)
{
    return window->frect->w;
}

double window_get_height(const Window* window)
{
    return window->frect->h;
}

SDL_FRect* window_get_frect(const Window* window)
{
    return window->frect;
}

WindowContent window_get_content(const Window* window)
{
    return window->content;
}



void window_draw(SDL_Renderer* renderer, Window* window)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, window_get_frect(window));
}
