#include "include/ui_elements/window.h"
#include "helper_functions/error_handling.h"

struct Window {
    float x;
    float y;
    float width;
    float height;
    float scale;

    Vector* sprites;
    Vector* buttons;
    Vector* textboxes;

    Vector* textures;

    SDL_Texture* texture_background;
};

Window* window_create(
    float x,
    float y,
    float width,
    float height,
    SDL_Texture* background_texture)
{
    verify(width < 0 || height < 0, "Invalid size");

    Window* window = SDL_malloc(sizeof(Window));
    verify(window == NULL, "Window could not be created: malloc");

    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->scale = 1;

    window->sprites = vector_create(sprite_ops());
    window->buttons = vector_create(button_ops());
    window->textboxes = vector_create(textbox_ops());

    window->textures = vector_create(SDL_Texture_ops());

    window->texture_background = background_texture;

    return window;
}

void window_load_textures(SDL_Renderer* renderer, Window* window, const char** paths, int size)
{
    verify(renderer == NULL, "Renderer does not exist");
    verify(window == NULL, "Window does not exist");
    verify(size < 0, "Invalid size");

    for (int i = 0; i < size; i++)
    {
        SDL_Texture* texture = SDL_CreateTextureFromPNG(renderer, paths[i]);
        vector_add(window->textures, texture);
    }
}
void window_add_sprite(Window* window, Sprite* sprite, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(sprite == NULL, "Sprite does not exist");

    vector_add(window->sprites, sprite);

    sprite_set_position(
        sprite, 
        window_get_x(window) + (x * window->scale),
        window_get_y(window) + (y * window->scale)
    );
    sprite_set_size(
        sprite, 
        sprite_get_width(sprite) * window->scale, 
        sprite_get_height(sprite) * window->scale
    );
}
void window_add_button(Window* window, Button* button, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(button == NULL, "Button does not exist");

    vector_add(window->buttons, button);

    button_set_position(
        button, 
        window_get_x(window) + (x * window->scale),
        window_get_y(window) + (y * window->scale)
    );
    button_set_size(
        button, 
        button_get_width(button) * window->scale, 
        button_get_height(button) * window->scale
    );
}
void window_add_textbox(Window* window, Textbox* textbox, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(textbox == NULL, "Textbox does not exist");

    vector_add(window->textboxes, textbox);

    textbox_set_position(
        textbox, 
        window_get_x(window) + (x * window->scale),
        window_get_y(window) + (y * window->scale)
    );
    textbox_set_size(
        textbox, 
        textbox_get_width(textbox) * window->scale, 
        textbox_get_height(textbox) * window->scale
    );
}

void window_destroy(Window* window)
{
    verify(window == NULL, "Window does not exist");

    if (window->sprites != NULL)
    {
        vector_destroy(window->sprites);
    }
    if (window->buttons != NULL)
    {
        vector_destroy(window->buttons);
    }
    if (window->textboxes != NULL)
    {
        vector_destroy(window->textboxes);
    }
    if (window->textures != NULL)
    {
        vector_destroy(window->textures);
    }
    if (window->texture_background != NULL)
    {
        SDL_DestroyTexture(window->texture_background);
    }

    SDL_free(window);
}
void window_destroy_content(Window* window)
{
    verify(window == NULL, "Window does not exist");

    vector_destroy_content(window->sprites);
    vector_destroy_content(window->buttons);
    vector_destroy_content(window->textboxes);
}
void window_destroy_sprite(Window* window, Sprite* sprite)
{
    verify(window == NULL, "Window does not exist");
    verify(sprite == NULL, "Sprite does not exist");

    vector_destroy_item(window->sprites, sprite);
}
void window_destroy_button(Window* window, Button* button)
{
    verify(window == NULL, "Window does not exist");
    verify(button == NULL, "Button does not exist");

    vector_destroy_item(window->sprites, button);
}
void window_destroy_textbox(Window* window, Textbox* textbox)
{
    verify(window == NULL, "Window does not exist");
    verify(textbox == NULL, "Textbox does not exist");

    vector_destroy_item(window->sprites, textbox);
}

static void window_render_background(SDL_Renderer* renderer, const Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(renderer == NULL, "Renderer does not exist");

    SDL_FRect frect =
        {
            window->x,
            window->y,
            window->width,
            window->height
        };
    SDL_RenderTexture(
        renderer, 
        window->texture_background, 
        NULL, 
        &frect
    );
}
void window_render(SDL_Renderer* renderer, Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(renderer == NULL, "Renderer does not exist");

    SDL_Rect rect =
        {
            window_get_x(window),
            window_get_y(window),
            window_get_width(window),
            window_get_height(window)
        };
    SDL_SetRenderClipRect(renderer, &rect);

    window_render_background(renderer, window);

    for (int i = 0; i < vector_get_count(window->sprites); i++)
        sprite_render(renderer, (Sprite*)vector_get_at(window->sprites, i));

    for (int i = 0; i < vector_get_count(window->buttons); i++)
        button_render(renderer, (Button*)vector_get_at(window->buttons, i));

    for (int i = 0; i < vector_get_count(window->textboxes); i++)
        textbox_render(renderer, (Textbox*)vector_get_at(window->textboxes, i));
}

void window_update(const InputState* input, Window* window)
{
    verify(window == NULL, "Window does not exist");
    verify(input == NULL, "InputState does not exist");

    for (int i = 0; i < vector_get_count(window->buttons); i++)
        button_update(input, (Button*)vector_get_at(window->buttons, i));
}

static void window_sprites_update_position(Window* window, float new_window_x, float new_window_y)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->sprites); i++)
    {
        Sprite* sprite = vector_get_at(window->sprites, i);
        sprite_set_position(
            sprite, 
            sprite_get_x(sprite) - window_get_x(window) + new_window_x, 
            sprite_get_y(sprite) - window_get_y(window) + new_window_y
        );
    }
}
static void window_buttons_update_position(Window* window, float new_window_x, float new_window_y)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_set_position(
            button,
            button_get_x(button) - window_get_x(window) + new_window_x, 
            button_get_y(button) - window_get_y(window) + new_window_y 
        );
    }
}
static void window_textboxes_update_position(Window* window, float new_window_x, float new_window_y)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->textboxes); i++)
    {
        Textbox* textbox = vector_get_at(window->textboxes, i);
        textbox_set_position(
            textbox,
            textbox_get_x(textbox) - window_get_x(window) + new_window_x,
            textbox_get_y(textbox) - window_get_y(window) + new_window_y 
        );
    }
}
void window_set_position(Window* window, float x, float y)
{
    verify(window == NULL, "Window does not exist");

    window_sprites_update_position(window, x, y);
    window_buttons_update_position(window, x, y);
    window_textboxes_update_position(window, x, y);
    window->x = x;
    window->y = y;
}
void window_set_size(Window* window, float width, float height)
{
    verify(window == NULL, "Window does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    window->width = width;
    window->height = height;
}
static void window_sprites_update_size(Window* window, float new_scale)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->sprites); i++)
    {
        Sprite* sprite = vector_get_at(window->sprites, i);
        sprite_set_size(
            sprite, 
            sprite_get_width(sprite) / window->scale * new_scale,
            sprite_get_height(sprite) / window->scale * new_scale
        );
    }
}
static void window_buttons_update_size(Window* window, float new_scale)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_set_size(
            button, 
            button_get_width(button) / window->scale * new_scale,
            button_get_height(button) / window->scale * new_scale
        );
    }
}
static void window_textboxes_update_size(Window* window, float new_scale)
{
    verify(window == NULL, "Window does not exist");

    for (int i = 0; i < vector_get_count(window->textboxes); i++)
    {
        Textbox* textbox = vector_get_at(window->textboxes, i);
        textbox_set_size(
            textbox, 
            textbox_get_width(textbox) / window->scale * new_scale,
            textbox_get_height(textbox) / window->scale * new_scale
        );
    }
}
void window_set_scale(Window* window, float scale)
{
    window_sprites_update_size(window, scale);
    window_buttons_update_size(window, scale);
    window_textboxes_update_size(window, scale);
    window->scale = scale;
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
void window_sprite_set_position(Window* window, Sprite* sprite, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(sprite == NULL, "Sprite does not exist");

    float sprite_x = window_get_x(window) - sprite_get_x(sprite) + x;
    float sprite_y = window_get_y(window) - sprite_get_y(sprite) + y;

    sprite_set_position(sprite, sprite_x, sprite_y);
}
void window_button_set_position(Window* window, Button* button, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(button == NULL, "Button does not exist");

    float button_x = window_get_x(window) - button_get_x(button) + x;
    float button_y = window_get_y(window) - button_get_y(button) + y;

    button_set_position(button, button_x, button_y);
}
void window_textbox_set_position(Window* window, Textbox* textbox, float x, float y)
{
    verify(window == NULL, "Window does not exist");
    verify(textbox == NULL, "Textbox does not exist");

    float textbox_x = window_get_x(window) - textbox_get_x(textbox) + x;
    float textbox_y = window_get_y(window) - textbox_get_y(textbox) + y;

    textbox_set_position(textbox, textbox_x, textbox_y);
}





float window_get_x(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->x;
}
float window_get_y(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->y;
}
float window_get_width(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->width;
}
float window_get_height(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    return window->height;
}
float window_get_scale(const Window* window)
{
    return window->scale;
}
SDL_FRect window_get_frect(const Window* window)
{
    verify(window == NULL, "Window does not exist");

    SDL_FRect frect =
        {
            window->x,
            window->y,
            window->width,
            window->height
        };

    return frect;
}
SDL_Texture* window_get_texture(const Window* window, int index)
{
    verify(window == NULL, "Window does not exist");

    return vector_get_at(window->textures, index);
}
