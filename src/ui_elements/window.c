#include "include/ui_elements/window.h"

struct Window {
    float x;
    float y;
    float width;
    float height;
    float anchor_x;
    float anchor_y;
    float scale;

    Vector* sprites;
    Vector* buttons;
    Vector* textboxes;

    Vector* textures;

    SDL_Texture* texture_background;

    bool is_draggable;
    bool is_zoomable;
    bool is_scrollable;
};

Window* window_create(
    float x,
    float y,
    float width,
    float height,
    SDL_Texture* background_texture,
    WindowFlags flags
)
{
    verify_size(width, height);

    Window* window = SDL_malloc(sizeof(Window));
    verify(window == NULL, "Window could not be created: malloc");

    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->anchor_x = x;
    window->anchor_y = y;
    window->scale = 1;

    window->sprites = vector_create(sprite_ops());
    window->buttons = vector_create(button_ops());
    window->textboxes = vector_create(textbox_ops());

    window->textures = vector_create(SDL_Texture_ops());

    window->texture_background = background_texture;

    window->is_scrollable = (flags & WINDOW_SCROLLABLE);
    window->is_zoomable = (flags & WINDOW_ZOOMABLE);
    window->is_draggable = (flags & WINDOW_DRAGGABLE);

    return window;
}

void window_load_textures(SDL_Renderer* renderer, Window* window, const char** paths, int size)
{
    verify_window(window);
    verify_renderer(renderer);

    for (int i = 0; i < size; i++)
    {
        SDL_Texture* texture = SDL_CreateTextureFromPNG(renderer, paths[i]);
        vector_add(window->textures, texture);
    }
}
void window_add_sprite(Window* window, Sprite* sprite, float x, float y)
{
    verify_window(window);
    verify_sprite(sprite);

    vector_add(window->sprites, sprite);

    sprite_set_position(sprite, x, y);
    sprite_set_anchor(sprite, window->anchor_x, window->anchor_y);
    sprite_set_scale(sprite, window->scale);
}
void window_add_button(Window* window, Button* button, float x, float y)
{
    verify_window(window);
    verify_button(button);

    vector_add(window->buttons, button);

    button_set_position(button, x, y);
    button_set_anchor(button, window->anchor_x, window->anchor_y);
    button_set_scale(button, window->scale);
}
void window_add_textbox(Window* window, Textbox* textbox, float x, float y)
{
    verify_window(window);
    verify_textbox(textbox);

    vector_add(window->textboxes, textbox);

    textbox_set_position(textbox, x, y);
    textbox_set_anchor(textbox, window->anchor_x, window->anchor_y);
    textbox_set_scale(textbox, window->scale);
}

void window_destroy(Window* window)
{
    verify_window(window);

    vector_destroy(window->sprites);
    vector_destroy(window->buttons);
    vector_destroy(window->textboxes);
    vector_destroy(window->textures);

    if (window->texture_background != NULL)
    {
        SDL_DestroyTexture(window->texture_background);
    }

    SDL_free(window);
}
void window_destroy_content(Window* window)
{
    verify_window(window);

    vector_destroy_content(window->sprites);
    vector_destroy_content(window->buttons);
    vector_destroy_content(window->textboxes);
}
void window_destroy_sprite(Window* window, Sprite* sprite)
{
    verify_window(window);
    verify_sprite(sprite);

    vector_destroy_item(window->sprites, sprite);
}
void window_destroy_button(Window* window, Button* button)
{
    verify_window(window);
    verify_button(button);

    vector_destroy_item(window->sprites, button);
}
void window_destroy_textbox(Window* window, Textbox* textbox)
{
    verify_window(window);
    verify_textbox(textbox);

    vector_destroy_item(window->sprites, textbox);
}

static void window_render_background(SDL_Renderer* renderer, const Window* window)
{
    SDL_FRect frect = window_get_frect(window);
    SDL_RenderTexture(
        renderer, 
        window->texture_background, 
        NULL, 
        &frect
    );
}
void window_render(SDL_Renderer* renderer, const Window* window)
{
    verify_renderer(renderer);
    verify_window(window);

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
    {
        Sprite* sprite = vector_get_at(window->sprites, i);
        sprite_render(renderer, sprite);
    }
    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_render(renderer, button);
    }
    for (int i = 0; i < vector_get_count(window->textboxes); i++)
    {
        Textbox* textbox = vector_get_at(window->textboxes, i);
        textbox_render(renderer, textbox);
    }
}

static void window_drag(const InputState* input, Window* window)
{
    if (!window->is_draggable)
    {
        return;
    }

    float mouse_x = mouse_get_x(input);
    float mouse_y = mouse_get_y(input);
    static float window_mouse_x;
    static float window_mouse_y;

    if (!mouse_get_down(input, MOUSE_RIGHT))
    {
        window_mouse_x = mouse_x;
        window_mouse_y = mouse_y;
        return;
    }

    int new_x = mouse_x - window_mouse_x;
    int new_y = mouse_y - window_mouse_y;
    // TODO: clamp drag
    window_set_anchor(window, window->anchor_x + new_x, window->anchor_y + new_y);

    window_mouse_x = mouse_x;
    window_mouse_y = mouse_y;
}
static void window_zoom(const InputState* input, Window* window)
{
    if (!window->is_zoomable)
    {
        return;
    }

    float mouse_wheel = mouse_get_wheel(input);
    if (mouse_wheel == 0)
    {
        return;
    }
    float new_scale = window_get_scale(window) + mouse_wheel;
    // TODO: clamp zoom
    if (new_scale < 4) 
    {
        window_set_scale(window, 4);
        return;
    }
    if (new_scale > 12)
    {
        window_set_scale(window, 12);
        return;
    }
    window_set_scale(window, new_scale);
    // TODO: zoom to mouse
}
static void window_scroll(const InputState* input, Window* window)
{
    if (!window->is_scrollable)
    {
        return;
    }

    float mouse_wheel = mouse_get_wheel(input);
    if (mouse_wheel == 0)
    {
        return;
    }
    window_set_anchor(
        window,
        window->anchor_x,
        window->anchor_y + mouse_wheel * window->scale * WINDOW_SCROLL_FACTOR
    );
}
void window_update(const InputState* input, Window* window)
{
    verify_window(window);
    verify_input(input);

    float mouse_x = mouse_get_x(input);
    float mouse_y = mouse_get_y(input);
    SDL_FRect frect = window_get_frect(window);
    if (!point_intersects_rect(mouse_x, mouse_y, &frect))
    {
        return;
    }
    window_zoom(input, window);
    window_drag(input, window);
    window_scroll(input, window);

    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_update(input, button);
    }
}

void window_set_position(Window* window, float x, float y)
{
    verify_window(window);

    window_set_anchor(
        window,
        window->anchor_x + window->x - x,
        window->anchor_y + window->y - y
    );
    window->x = x;
    window->y = y;
}
static void window_elements_update_anchor(Window* window)
{
    for (int i = 0; i < vector_get_count(window->sprites); i++)
    {
        Sprite* sprite = vector_get_at(window->sprites, i);
        sprite_set_anchor(sprite, window->anchor_x, window->anchor_y);
    }
    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_set_anchor(button, window->anchor_x, window->anchor_y);
    }
    for (int i = 0; i < vector_get_count(window->textboxes); i++)
    {
        Textbox* textbox = vector_get_at(window->textboxes, i);
        textbox_set_anchor(textbox, window->anchor_x, window->anchor_y);
    }
}
void window_set_anchor(Window* window, float anchor_x, float anchor_y)
{
    window->anchor_x = anchor_x;
    window->anchor_y = anchor_y;

    window_elements_update_anchor(window);
}
void window_set_size(Window* window, float width, float height)
{
    verify_window(window);
    verify_size(width, height);

    window->width = width;
    window->height = height;
}
static void window_elements_set_scale(Window* window, float scale)
{
    for (int i = 0; i < vector_get_count(window->sprites); i++)
    {
        Sprite* sprite = vector_get_at(window->sprites, i);
        sprite_set_scale(sprite, scale);
    }
    for (int i = 0; i < vector_get_count(window->buttons); i++)
    {
        Button* button = vector_get_at(window->buttons, i);
        button_set_scale(button, scale);
    }
    for (int i = 0; i < vector_get_count(window->textboxes); i++)
    {
        Textbox* textbox = vector_get_at(window->textboxes, i);
        textbox_set_scale(textbox, scale);
    }
}
void window_set_scale(Window* window, float scale)
{
    verify_window(window);

    window->scale = scale;
    window_elements_set_scale(window, scale);
}

void window_update_background_texture(Window* window, SDL_Texture* texture)
{
    verify_window(window);

    if (window->texture_background != NULL)
    {
        SDL_DestroyTexture(window->texture_background);
        window->texture_background = NULL;
    }
    window->texture_background = texture;
}




float window_get_x(const Window* window)
{
    verify_window(window);

    return window->x;
}
float window_get_y(const Window* window)
{
    verify_window(window);

    return window->y;
}
float window_get_width(const Window* window)
{
    verify_window(window);

    return window->width;
}
float window_get_height(const Window* window)
{
    verify_window(window);

    return window->height;
}
float window_get_scale(const Window* window)
{
    verify_window(window);

    return window->scale;
}
SDL_FRect window_get_frect(const Window* window)
{
    verify_window(window);

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
    verify_window(window);

    return vector_get_at(window->textures, index);
}
Vector* window_get_sprites(const Window* window)
{
    verify_window(window);

    return window->sprites;
}
Vector* window_get_buttons(const Window* window)
{
    verify_window(window);

    return window->buttons;
}
Vector* window_get_textboxes(const Window* window)
{
    verify_window(window);

    return window->textboxes;
}



void verify_window(const Window* window)
{
    verify(window == NULL, "Window does not exist");
}
