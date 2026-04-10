#include "../../include/ui_elements/window.h"

struct Window {
    // window properties
    SDL_FRect* frect;
    SDL_Texture* texture;
    WindowContent content;

    // window content
    Object** objects;
    int object_count;
    Button** buttons;
    int button_count;
    Textbox** textboxes;
    int textbox_count;
};



// ========== create ==========

Window* window_create(
    double x, 
    double y, 
    double width, 
    double height, 
    SDL_Texture* texture,
    const WindowContent content)
{
    Window* window = (Window*)SDL_malloc(sizeof(Window));

    window->frect = SDL_CreateFRect(x, y, width, height);

    window->texture = texture;

    window->content = content;

    window->objects = NULL;
    window->object_count = 0;

    window->buttons = NULL;
    window->button_count = 0;

    window->textboxes = NULL;
    window->textbox_count = 0;

    return window;
}





// ========== destroy ==========

void window_destroy(Window* window)
{
    for (int i = 0; i < window->object_count; i++)
        object_destroy(window->objects[i]);

    for (int i = 0; i < window->button_count; i++)
        button_destroy(window->buttons[i]);

    for (int i = 0; i < window->textbox_count; i++)
        textbox_destroy(window->textboxes[i]);

    SDL_free(window);
}



// ========== render ==========
void window_render(SDL_Renderer* renderer, Window* window)
{
    // render background
    SDL_RenderTexture(renderer, window->texture, NULL, window->frect);
    
    // render objects
    for (int i = 0; i < window->object_count; i++)
        object_render(renderer, window->objects[i]);

    // render buttons
    for (int i = 0; i < window->button_count; i++)
        button_render(renderer, window->buttons[i]);

    // render textboxes
    for (int i = 0; i < window->textbox_count; i++)
        textbox_render(renderer, window->textboxes[i]);
}



// ========== update ==========

void window_update(InputState* input, Window* window)
{
    for (int i = 0; i < window->button_count; i++)
        button_update(input , window->buttons[i]);
}



// ========== set ==========

static void window_objects_update_position(Window* window)
{
    for (int i = 0; i < window->object_count; i++)
    {
        Object* object = window->objects[i];
        object_set_position(
            object, 
            object_get_x(object) + window_get_x(window), 
            object_get_y(object) + window_get_y(window)
        );
    }
}
static void window_buttons_update_position(Window* window)
{
    for (int i = 0; i < window->button_count; i++)
    {
        Button* button = window->buttons[i];
        button_set_position(
            button,
            button_get_x(button) + window_get_x(window), 
            button_get_y(button) + window_get_y(window)
        );
    }
}
static void window_textboxes_update_position(Window* window)
{
    for (int i = 0; i < window->textbox_count; i++)
    {
        Textbox* textbox = window->textboxes[i];
        textbox_set_position(
            textbox,
            textbox_get_x(textbox) + window_get_x(window), 
            textbox_get_y(textbox) + window_get_y(window)
        );
    }
}
void window_set_position(Window* window, const double x, const double y)
{
    window->frect->x = x;
    window->frect->y = y;
    window_objects_update_position(window);
    window_buttons_update_position(window);
    window_textboxes_update_position(window);
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

void window_set_texture(Window* window, SDL_Texture* texture)
{
    window->texture = texture;
}

void window_add_object(Window* window, Object* object)
{
    object_set_position(
        object, 
        object_get_x(object) + window_get_x(window), 
        object_get_y(object) + window_get_y(window)
    );
    window->object_count++;
    window->objects = SDL_realloc(window->objects, window->object_count * sizeof(Object*));
    window->objects[window->object_count - 1] = object;
}

void window_add_button(Window* window, Button* button)
{
    button_set_position(
        button,
        button_get_x(button) + window_get_x(window), 
        button_get_y(button) + window_get_y(window)
    );
    window->button_count++;
    window->buttons = SDL_realloc(window->buttons, window->button_count * sizeof(Button*));
    window->buttons[window->button_count - 1] = button;
}

void window_add_textbox(Window* window, Textbox* textbox)
{
    textbox_set_position(
        textbox,
        textbox_get_x(textbox) + window_get_x(window), 
        textbox_get_y(textbox) + window_get_y(window)
    );
    window->textbox_count++;
    window->textboxes = SDL_realloc(window->textboxes, window->textbox_count * sizeof(Textbox*));
    window->textboxes[window->textbox_count - 1] = textbox;
}



// ========== get ==========

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
