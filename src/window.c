#include "../include/window.h"

// Windows defined within the game
// Window != SDL_Window, SDL_Window is a desktop window while Window is a in-game window as the screen is devided into multiple sections
struct Window {
    Position *pos;
    Size *size;
    WindowContent content;
};

Window *window_create(Position *pos, Size *size, WindowContent content)
{
    Window* window = (Window*)malloc(sizeof(Window));
    if (!window)
    {
        perror("Could not create window: malloc");
        exit(1);
    }
    window_set_posistion(window, pos);
    window_set_size(window, size);
    window_set_content(window, content);
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

void window_set_position(Window *window, Position *pos)
{
    window->pos = pos;
}

void window_set_size(Window *window, Size *size)
{
    window->size = size;
}

void window_set_content(Window *window, WindowContent content)
{
    window->content = content;
}


Position *window_get_position(Window *window)
{
    return window->pos;
}

Size *window_get_size(Window *window)
{
    return window->size;
}

WindowContent window_get_content(Window *window)
{
    return window->content;
}

void window_draw(Window window)
{
    // TODO
}

