// Windows defined within the game
// Window != SDL_Window, SDL_Window is a desktop window while Window is a in-game window as the screen is devided into multiple sections

#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "../inputstate.h"
#include "object.h"
#include "button.h"
#include "textbox.h"

typedef enum WindowContent {
    NONE,
    BOARD,
    TREE,
    PROPERTIES,
    RULES,
    HISTORY,
    OPTIONS,
    PLAYER_1,
    PLAYER_2,
} WindowContent;

typedef struct Window Window;



// ========== create ==========

/** TODO: remake comment
    * @brief creates a window
    *
    * @param x the x position in pixels
    * @param y the y position in pixels
    * @param width the width of the window in pixels
    * @param height the height of the window in pixels
    * @param WindowContent the content of the window
    *
    * @return a pointer to the created window
    */
Window* window_create(
    double x, 
    double y, 
    double width, 
    double height, 
    SDL_Texture* texture,
    WindowContent content
);



// ========== destroy ==========

/** TODO: remake comment
    * @brief destroy the allocated window
    *
    * @param window the pointer to the window
    */
void window_destroy(Window* window);



// ========== render ==========

// TODO: add comments
void window_render(SDL_Renderer* renderer, Window* window);



// ========== update ==========

void window_update(InputState* input, Window* window);



// ========== set ==========

/*
    * @brief sets the position of the window
    *
    * @param window a pointer to the window to change the position
    * @param x the new x position
    * @param y the new y position
    */
void window_set_position(Window* window, double x, double y);

/*
    * @brief sets the size of the window
    *
    * @param window a pointer to the window to change the size
    * @param width the new width
    * @param height the new height
    */
void window_set_size(Window* window, double width, double height);

/*
    * @brief sets the content of the window
    *
    * @param window a pointer to the window to change the content
    * @param content the new id of the content for the window
    */
void window_set_content(Window* window, WindowContent content);

// TODO: add comment
void window_set_texture(Window* window, SDL_Texture* texture);
// TODO: add comment
void window_add_object(Window* window, Object* object);
// TODO: add comment
void window_add_button(Window* window, Button* button);
// TODO: add comment
void window_add_textbox(Window* window, Textbox* textbox);



// ========== get ==========

/**
    * @brief gets the x position of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the x position of the window
    */
double window_get_x(const Window* window);

/**
    * @brief gets the y position of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the y position of the window
    */
double window_get_y(const Window* window);

/**
    * @brief gets the width of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the width of the window
    */
double window_get_width(const Window* window);

/**
    * @brief gets the height of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the height of the window
    */
double window_get_height(const Window* window);

/**
    * @brief gets the SDL_FRect of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the pointer to the SDL_FRect of the window
    */
SDL_FRect* window_get_frect(const Window* window);

/**
    * @brief gets the content of the window
    *
    * @param window a pointer to the window
    *
    * @return returns the content of the window
    */
WindowContent window_get_content(const Window* window);




