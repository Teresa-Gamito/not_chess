#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#include "../helper_functions/helper_functions.h"
#include "object.h"

typedef struct Textbox Textbox;

typedef enum TextAlignment {
    TEXT_CENTERED,
    TEXT_RIGHT_ALIGNED,
    TEXT_LEFT_ALIGNED 
} TextAlignment;



// ========== create ==========

/** TODO: update comment
    * @brief creates a textbox
    * 
    * @param renderer the SDL_Renderer in which to render the textbox
    * @param x the x position in pixels
    * @param y the y position in pixels
    * @param max_width the maximum width in pixels in which the text is set to wrap, set as 0 to wrap only on a new line character
    * @param text the text for the textbox to display (needs to end with a null terminated characrter)
    * @param font the font of the text (created externaly)
    * @param color the color of the text (created externaly)
    *
    * @return a pointer to the created textbox
    */
Textbox* textbox_create(
    SDL_Renderer* renderer,
    TTF_Font* font,
    SDL_Color* color,
    const char* text,
    const double max_width,
    TextAlignment alignment,
    const double x,
    const double y
);



// ========== destroy ==========

/**
    * @brief destroy a textbox
    * 
    * @param textbox the pointer to the textbox
    */
void textbox_destroy(Textbox* textbox);


// ========== render ==========
/**
    * @brief render the textbox on the selected SDL_Renderer
    *
    * @param renderer the destination renderer
    * @param textbox the textbox to render
    * @param alignment where to align the text
    */
void textbox_render(SDL_Renderer* renderer, Textbox* textbox);


// ========== set ==========

/**
    * @brief sets the position of the textbox
    * 
    * @param textbox a pointer to the textbox to change the position
    * @param x the new x position
    * @param y the new y position
    */
void textbox_set_position(Textbox* textbox, const double x, const double y);

/**
    * @brief sets the size of the textbox
    * 
    * @param textbox a pointer to the textbox to change the size
    * @param width the new width
    * @param height the new height
    */
void textbox_set_size(Textbox* textbox, const double width, const double height);

// TODO: add comments
// TODO: void textbox_set_scale();

// TODO: add comments
void textbox_set_max_width(Textbox* textbox, int max_width);

// TODO: add comments
void textbox_set_alignment(Textbox* textbox, TextAlignment alignment);

/**
    * @brief sets the pointer to SDL_Color of the textbox
    * 
    * the color contains the values of red, green, blue and opacity
    * 
    * @param textbox a pointer to the textbox to change the content
    * @param color the pointer to the SDL_Color
    */
void textbox_set_color(Textbox* textbox, SDL_Color* color);

/**
    * @brief sets the pointer to TTF_Font of the textbox
    * 
    * @param textbox a pointer to the textbox to change the content
    * @param font the pointer to the TTF_Font
    */
void textbox_set_font(Textbox* textbox, TTF_Font* font);

/**
    * @brief changes the text on the textbox and updates the texture
    * 
    * @param renderer the pointer to the SDL_Renderer used
    * @param textbox a pointer to the textbox to change the content
    * @param text the new text to display
    */
void textbox_set_text(SDL_Renderer* renderer, Textbox* textbox, const char* text);



// ========== get ==========

/**
    * @brief gets the x position of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the x position of the textbox
    */
float textbox_get_x(const Textbox* textbox);

/**
    * @brief gets the y position of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the y position of the textbox
    */
float textbox_get_y(const Textbox* textbox);

/**
    * @brief gets the width of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the width of the textbox
    */
float textbox_get_width(const Textbox* textbox);

/**
    * @brief gets the height of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the height of the textbox
    */
float textbox_get_height(const Textbox* textbox);

// TODO: add comments
int textbox_get_max_width(Textbox* textbox);

// TODO: add comments
TextAlignment textbox_get_alignment(Textbox* textbox);

/**
    * @brief gets the SDL_FRect of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the pointer to the SDL_FRect of the textbox
    */
SDL_FRect* textbox_get_frect(const Textbox* textbox);

/**
    * @brief gets the SDL_Color of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the pointer to the SDL_Color of the textbox
    */
SDL_Color* textbox_get_color(Textbox* textbox);

/**
    * @brief gets the TTF_Font of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the pointer to the TTF_Font of the textbox
    */
TTF_Font* textbox_get_font(const Textbox* textbox);

/**
    * @brief gets the SDL_Texture of the textbox
    * 
    * @param textbox a pointer to the textbox
    * 
    * @return returns the pointer to the SDL_Texture of the textbox
    */
SDL_Texture* textbox_get_texture(Textbox* textbox);
