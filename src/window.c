#include "../header/window.h"
#include <stdio.h>
#include <stdlib.h>

// Windows defined within the game
// Window != SDL_Window, SDL_Window is a desktop window while Window is a in-game window as the screen is devided into multiple sections
struct Window {
    // dimentions
    int width;
    int height;
    // position
    int pos_x;
    int pos_y;
    // content id
    WindowContent content;
};

Window* window_create()
{
    Window* window = (Window*)malloc(sizeof(Window));
    if (!window)
    {
        perror("Could not create window: malloc");
        exit(1);
    }
    return window;
}

void window_destroy(Window *window)
{
    if (!window) 
    {
        return;
    }
    free(window);
}

void window_set_size(Window *window, double width, double height)
{
    window->width = width;
    window->height = height;
}

void window_set_position(Window *window, double pos_x, double pos_y)
{
    window->pos_x = pos_x;
    window->pos_y = pos_y;
}

int window_get_width(Window *window)
{
    return window->width;
}

int window_get_height(Window *window)
{
    return window->height;
}

int window_get_pos_x(Window *window)
{
    return window->pos_x;
}

int window_get_pos_y(Window *window)
{
    return window->pos_y;
}

WindowContent window_get_content(Window *window)
{
    return window->content;
}

void window_print(Window window)
{
    // TODO
}

