#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_rect.h>
#include "ui_elements/sprite.h"
#include "ui_elements/button.h"
#include "ui_elements/textbox.h"
#include "data_structures/vector.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/global_variables.h"

#define WINDOW_SCROLL_FACTOR 3

typedef Uint8 WindowFlags;
#define WINDOW_SCROLLABLE       1ULL<<0
#define WINDOW_ZOOMABLE         1ULL<<1
#define WINDOW_DRAGGABLE        1ULL<<2

typedef struct Window Window;

// ========== create ==========
Window* window_create(
    float x, 
    float y, 
    float width, 
    float height, 
    SDL_Texture* background_texture,
    WindowFlags flags
);
void window_load_textures(
    SDL_Renderer* renderer, 
    Window* window, 
    const char** paths, 
    int count
);
void window_add_sprite(Window* window, Sprite* sprite, float x, float y);
void window_add_button(Window* window, Button* button, float x, float y);
void window_add_textbox(Window* window, Textbox* textbox, float x, float y);

// ========== destroy ==========
void window_destroy(Window* window);
void window_unload_textures(Window* window);
void window_destroy_content(Window* window);
void window_destroy_sprite(Window *window, Sprite* sprite);
void window_destroy_button(Window *window, Button* button);
void window_destroy_textbox(Window *window, Textbox* textbox);

// ========== render ==========
void window_render(SDL_Renderer* renderer, const Window* window);

// ========== update ==========
void window_update(const InputState* input, Window* window);

// ========== set ==========
void window_set_position(Window* window, float x, float y);
void window_set_anchor(Window* window, float anchor_x, float anchor_y);
void window_set_size(Window* window, float width, float height);
void window_set_scale(Window* window, float scale);
void window_update_background_texture(Window* window, SDL_Texture* texture);

// ========== get ==========
float window_get_x(const Window* window);
float window_get_y(const Window* window);
float window_get_width(const Window* window);
float window_get_height(const Window* window);
float window_get_scale(const Window* window);
SDL_FRect window_get_frect(const Window* window);
SDL_Texture* window_get_texture(const Window* window, int index);
const Vector* window_get_sprites(const Window* window);
const Vector* window_get_buttons(const Window* window);
const Vector* window_get_textboxes(const Window* window);

void verify_window(const Window* window);
