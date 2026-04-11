// Windows defined within the game
// Window != SDL_Window, SDL_Window is a desktop window while Window is a in-game window as the screen is devided into multiple sections

#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "../helper_functions/error_handling.h"
#include "../inputstate.h"
#include "object.h"
#include "button.h"
#include "textbox.h"

typedef struct Window Window;

// ========== create ==========
Window* window_create(
    double x, 
    double y, 
    double width, 
    double height, 
    SDL_Texture* background_texture
);
void window_load_textures(
    SDL_Renderer* renderer, 
    Window* window, 
    const char** paths, 
    int count
);

// ========== destroy ==========
void window_destroy(Window* window);

// ========== render ==========
void window_render(SDL_Renderer* renderer, Window* window);

// ========== update ==========
void window_update(InputState* input, Window* window);

// ========== set ==========
void window_set_position(Window* window, double x, double y);
void window_set_size(Window* window, double width, double height);
void window_set_background_texture(Window* window, SDL_Texture* texture);

void window_add_object(Window* window, Object* object);
void window_add_button(Window* window, Button* button);
void window_add_textbox(Window* window, Textbox* textbox);

// ========== get ==========
double window_get_x(const Window* window);
double window_get_y(const Window* window);
double window_get_width(const Window* window);
double window_get_height(const Window* window);
SDL_FRect* window_get_frect(const Window* window);
SDL_Texture* window_get_texture(const Window* window, int index);
