#include "../../include/ui_elements/button.h"

typedef enum ButtonState
{
    IDLE,
    HOVERED,
    PRESSED
} ButtonState;

struct Button
{
    SDL_Texture* texture_idle;
    SDL_Texture* texture_hovered;
    SDL_Texture* texture_pressed;

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
    
    button->texture_idle = texture_idle;
    button->texture_hovered = texture_hovered;
    button->texture_pressed = texture_pressed;

    button->state = IDLE;

    button->on_press = on_press;

    float w, h;
    if (texture_idle == NULL)
    {
        w = 0;
        h = 0;
    }
    else 
    {
        SDL_GetTextureSize(button->texture_idle, &w, &h);
    }
    button->frect = SDL_CreateFRect(x, y, w, h);

    return button;
}



// ========== destroy ==========

void button_destroy(void* p)
{
    Button* button = (Button*)p;
    if (button == NULL)
    {
        SDL_Log("Button does not exist, could not destroy");
        return;
    }

    SDL_DestroyFRect(button->frect);
    SDL_free(button);
}



// ========== update ==========

void button_update(InputState* input, Button* button)
{
    if (input == NULL)
    {
        SDL_Log("InputState not defined");
        return;
    }
    if (button == NULL)
    {
        SDL_Log("Button does not exist, could not update");
        return;
    }

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
    if (renderer == NULL)
    {
        SDL_Log("Renderer does not exist");
        return;
    }
    if (button == NULL)
    {
        SDL_Log("Button does not exist, could not be rendered");
        return;
    }

    if (button->state == PRESSED)
    {
        SDL_RenderTexture(
            renderer, 
            button->texture_pressed, 
            NULL, 
            button->frect
        );
    }
    else if (button->state == HOVERED)
    {
        SDL_RenderTexture(
            renderer, 
            button->texture_hovered, 
            NULL, 
            button->frect
        );
    }
    else if (button->state == IDLE)
    {
        SDL_RenderTexture(
            renderer, 
            button->texture_idle, 
            NULL, 
            button->frect
        );
    }
}



// ========== set ==========

void button_set_position(Button* button, double x, double y)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return;
    }

    button->frect->x = x;
    button->frect->y = y;
}

void button_set_size(Button* button, double width, double height)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return;
    }
    if (width < 0 || height < 0)
    {
        SDL_Log("Width or height have to be positive");
        return;
    }

    button->frect->w = width;
    button->frect->h = height;
}



// ========== get ==========

double button_get_x(const Button* button)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return 0;
    }
    return button->frect->x;
}

double button_get_y(const Button* button)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return 0;
    }
    return button->frect->y;
}

double button_get_width(const Button* button)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return 0;
    }
    return button->frect->w;
}

double button_get_height(const Button* button)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return 0;
    }
    return button->frect->h;
}

SDL_FRect* button_get_frect(const Button* button)
{
    if (button == NULL)
    {
        SDL_Log("Button does not exist");
        return NULL;
    }
    return button->frect;
}
