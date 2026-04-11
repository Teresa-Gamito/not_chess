#include "../../include/ui_elements/button.h"

typedef enum ButtonState
{
    IDLE,
    HOVERED,
    PRESSED,
} ButtonState;

struct Button
{
    SDL_Texture** textures;

    SDL_FRect* frect;

    void (*on_press)();

    ButtonState state;
};



// ========== create ==========

Button* button_create(
    double x, 
    double y, 
    void (*on_press)(),
    SDL_Texture* texture_idle, 
    SDL_Texture* texture_hovered, 
    SDL_Texture* texture_pressed)
{
    Button* button = (Button*)SDL_malloc(sizeof(Button));
    
    button->textures[IDLE] = texture_idle;
    button->textures[HOVERED] = texture_hovered;
    button->textures[PRESSED] = texture_pressed;

    button->state = IDLE;

    button->on_press = on_press;

    float w = 0;
    float h = 0;

    if (texture_idle != NULL)
    {
        SDL_GetTextureSize(button->textures[IDLE], &w, &h);
    }

    button->frect = SDL_CreateFRect(x, y, w, h);

    return button;
}



// ========== destroy ==========

void button_destroy(Button* button)
{
    verify(button == NULL, "Button does not exist");

    SDL_DestroyFRect(button->frect);
    SDL_free(button);
}



// ========== update ==========

void button_update(InputState* input, Button* button)
{
    verify(input == NULL, "InputState does not exist");
    verify(button == NULL, "Button does not exist");

    double mouse_x = input_get_mouse_x(input);
    double mouse_y = input_get_mouse_y(input);
    bool mouse_left_down = input_get_mouse_left_down(input);
    bool mouse_left_released = input_get_mouse_left_released(input);

    if (button->state == PRESSED && mouse_left_released)
    {
        button->on_press();
    }

    if (point_intersects_rect(mouse_x, mouse_y, button->frect))
    {
        if (mouse_left_down) 
        {
            button->state = PRESSED;
            return;
        }
        button->state = HOVERED;
        return;
    }
    button->state = IDLE;
}



// ========== render ==========

void button_render(SDL_Renderer* renderer, Button* button)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(button == NULL, "Button does not exist");

    SDL_RenderTexture(
        renderer, 
        button->textures[button->state], 
        NULL, 
        button->frect
    );
}



// ========== set ==========

void button_set_position(Button* button, double x, double y)
{
    verify(button == NULL, "Button does not exist");

    button->frect->x = x;
    button->frect->y = y;
}

void button_set_size(Button* button, double width, double height)
{
    verify(button == NULL, "Button does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    button->frect->w = width;
    button->frect->h = height;
}



// ========== get ==========

double button_get_x(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->frect->x;
}

double button_get_y(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->frect->y;
}

double button_get_width(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->frect->w;
}

double button_get_height(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->frect->h;
}

SDL_FRect* button_get_frect(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->frect;
}
