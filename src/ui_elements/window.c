#include "../../include/ui_elements/window.h"

struct Window {
    SDL_FRect* frect;

    SDL_Texture* texture_background;

    Object** objects;
    int object_count;

    Button** buttons;
    int button_count;

    Textbox** textboxes;
    int textbox_count;

    SDL_Texture** textures;
    int texture_count;
};



// ========== create ==========

Window* window_create(
    double x, 
    double y, 
    double width, 
    double height, 
    SDL_Texture* background_texture)
{
    verify(width < 0 || height < 0, "Invalid size");

    Window* window = (Window*)SDL_malloc(sizeof(Window));

    window->frect = SDL_CreateFRect(x, y, width, height);

    window->texture_background = background_texture;

    window->objects = NULL;
    window->object_count = 0;

    window->buttons = NULL;
    window->button_count = 0;

    window->textboxes = NULL;
    window->textbox_count = 0;

    window->textures = NULL;
    window->texture_count = 0;

    return window;
}

static void window_add_texture(Window* window, SDL_Texture* texture)
{
    verify(window == NULL, "Window does not exist");
    verify(texture == NULL, "Texture does not exist");

    window->textures = SDL_realloc(
        window->textures, 
        (window->texture_count + 1) * sizeof(Object*)
    );
    verify(window->textures == NULL, "Could not add texture: realloc");
    window->textures[window->texture_count] = texture;
    window->texture_count++;
}
void window_load_textures(SDL_Renderer* renderer, Window* window, const char** paths, int size)
{
    verify(renderer == NULL, "Renderer does not exist");
    verify(window == NULL, "Window does not exist");
    verify(size < 0, "Invalid size");

    for (int i = 0; i < size; i++)
    {
        window_add_texture(
            window,
            SDL_CreateTextureFromPNG(renderer, paths[i])
        );
    }
}



// ========== destroy ==========

void window_destroy(Window* window)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < window->object_count; i++)
        object_destroy(window->objects[i]);

    for (int i = 0; i < window->button_count; i++)
        button_destroy(window->buttons[i]);

    for (int i = 0; i < window->textbox_count; i++)
        textbox_destroy(window->textboxes[i]);

    for (int i = 0; i < window->texture_count; i++)
        SDL_DestroyTexture(window->textures[i]);

    if (window->texture_background != NULL)
    {
        SDL_DestroyTexture(window->texture_background);
    }

    SDL_free(window);
}



// ========== render ==========
static void window_render_background(SDL_Renderer* renderer, Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(renderer == NULL, "Renderer does not exist");

    SDL_RenderTexture(
        renderer, 
        window->texture_background, 
        NULL, 
        window->frect
    );
}
void window_render(SDL_Renderer* renderer, Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(renderer == NULL, "Renderer does not exist");

    window_render_background(renderer, window);
    
    for (int i = 0; i < window->object_count; i++)
        object_render(renderer, window->objects[i]);

    for (int i = 0; i < window->button_count; i++)
        button_render(renderer, window->buttons[i]);

    for (int i = 0; i < window->textbox_count; i++)
        textbox_render(renderer, window->textboxes[i]);
}



// ========== update ==========

void window_update(InputState* input, Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(input == NULL, "InputState does not exist");

    for (int i = 0; i < window->button_count; i++)
        button_update(input , window->buttons[i]);
}



// ========== set ==========

static void window_objects_update_position(Window* window)
{
    verify(window == NULL, "Window does not exist");

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
    verify(window == NULL, "Window does not exist");

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
    verify(window == NULL, "Window does not exist");

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
void window_set_position(Window* window, double x, double y)
{
    verify(window == NULL, "Window does not exist");

    window->frect->x = x;
    window->frect->y = y;
    window_objects_update_position(window);
    window_buttons_update_position(window);
    window_textboxes_update_position(window);
}

void window_set_size(Window* window, double width, double height)
{
    verify(window == NULL, "Window does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    window->frect->w = width;
    window->frect->h = height;
}

void window_update_background_texture(Window* window, SDL_Texture* texture)
{
    verify(window == NULL, "Window does not exist");

    if (window->texture_background != NULL)
    {
        SDL_DestroyTexture(window->texture_background);
        window->texture_background = NULL;
    }
    window->texture_background = texture;
}




void window_add_object(Window* window, Object* object)
{
    verify(window == NULL, "Window does not exist");
    verify(object == NULL, "Object does not exist");

    object_set_position(
        object, 
        object_get_x(object) + window_get_x(window), 
        object_get_y(object) + window_get_y(window)
    );

    window->objects = SDL_realloc(
        window->objects, 
        (window->object_count + 1) * sizeof(Object*)
    );
    verify(window->objects == NULL, "Could not add object: realloc");

    window->objects[window->object_count] = object;
    window->object_count++;
}

void window_add_button(Window* window, Button* button)
{
    verify(window == NULL, "Window does not exist");
    verify(button == NULL, "Button does not exist");

    button_set_position(
        button,
        button_get_x(button) + window_get_x(window), 
        button_get_y(button) + window_get_y(window)
    );

    window->buttons = SDL_realloc(
        window->buttons, 
        (window->button_count + 1) * sizeof(Button*));
    verify(window->buttons == NULL, "Could not add button: realloc");

    window->buttons[window->button_count] = button;
    window->button_count++;
}

void window_add_textbox(Window* window, Textbox* textbox)
{
    verify(window == NULL, "Window does not exist");
    verify(textbox == NULL, "Textbox does not exist");

    textbox_set_position(
        textbox,
        textbox_get_x(textbox) + window_get_x(window), 
        textbox_get_y(textbox) + window_get_y(window)
    );

    window->textboxes = SDL_realloc(
        window->textboxes, 
        (window->textbox_count + 1) * sizeof(Textbox*)
    );
    verify(window->textboxes == NULL, "Could not add textbox: realloc");

    window->textboxes[window->textbox_count] = textbox;
    window->textbox_count++;
}



// ========== get ==========

double window_get_x(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->frect->x;
}

double window_get_y(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->frect->y;
}

double window_get_width(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->frect->w;
}

double window_get_height(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->frect->h;
}

SDL_FRect* window_get_frect(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->frect;
}
SDL_Texture* window_get_texture(const Window* window, int index)
{
    verify(window == NULL, "Window does not exist");
    verify(index < 0 || index >= window->texture_count, "Invalid index");

    return window->textures[index];
}
