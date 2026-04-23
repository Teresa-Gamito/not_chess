#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#include "include/helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"

typedef struct Textbox Textbox;

typedef enum TextAlignment {
    TEXT_CENTERED,
    TEXT_RIGHT_ALIGNED,
    TEXT_LEFT_ALIGNED,

    TEXT_ALIGNMENT_COUNT
} TextAlignment;

Textbox* textbox_create(
    SDL_Renderer* renderer,
    TTF_Font* font,
    SDL_Color* color,
    const char* text,
    const double max_width,
    TextAlignment alignment
);
void textbox_destroy(Textbox* textbox);
void textbox_render(SDL_Renderer* renderer, const Textbox* textbox);

void textbox_set_position(Textbox* textbox, float x, float y);
void textbox_set_size(Textbox* textbox, float width, float height);
void textbox_set_max_width(Textbox* textbox, int max_width);
void textbox_set_alignment(Textbox* textbox, TextAlignment alignment);
void textbox_set_color(Textbox* textbox, SDL_Color* color);
void textbox_set_font(Textbox* textbox, TTF_Font* font);
void textbox_set_text(SDL_Renderer* renderer, Textbox* textbox, const char* text);

float textbox_get_x(const Textbox* textbox);
float textbox_get_y(const Textbox* textbox);
float textbox_get_width(const Textbox* textbox);
float textbox_get_height(const Textbox* textbox);
SDL_FRect textbox_get_frect(const Textbox* textbox);
int textbox_get_max_width(const Textbox* textbox);
TextAlignment textbox_get_alignment(const Textbox* textbox);
SDL_Color* textbox_get_color(const Textbox* textbox);
TTF_Font* textbox_get_font(const Textbox* textbox);

TypeOps* textbox_ops();
