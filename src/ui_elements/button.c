#include "include/ui_elements/button.h"
#include "helper_functions/error_handling.h"
#include <SDL3/SDL_log.h>

typedef enum ButtonState
{
    IDLE,
    HOVERED,
    PRESSED,

    BUTTON_STATE_COUNT
} ButtonState;

struct Button
{
    ButtonState state;

    SDL_Texture* textures[BUTTON_STATE_COUNT];

    float x;
    float y;
    float width;
    float height;

    void (*on_left_click)(void* arg1, void* arg2);
    void* l_arg1;
    void* l_arg2;

    void (*on_right_click)(void* arg1, void* arg2);
    void* r_arg1;
    void* r_arg2;

    void (*on_middle_click)(void* arg1, void* arg2);
    void* m_arg1;
    void* m_arg2;
};

Button* button_create(
    SDL_Texture* texture_idle, 
    SDL_Texture* texture_hovered, 
    SDL_Texture* texture_pressed
)
{
    Button* button = (Button*)SDL_malloc(sizeof(Button));
    verify(button == NULL, "Button could not be created: malloc");

    button->textures[IDLE] = texture_idle;
    button->textures[HOVERED] = texture_hovered;
    button->textures[PRESSED] = texture_pressed;

    button->state = IDLE;

    button->on_left_click = NULL;
    button->l_arg1 = NULL;
    button->l_arg2 = NULL;

    button->on_right_click = NULL;
    button->r_arg1 = NULL;
    button->r_arg2 = NULL;

    button->on_middle_click = NULL;
    button->m_arg1 = NULL;
    button->m_arg2 = NULL;

    float w = 0;
    float h = 0;

    if (texture_idle != NULL)
    {
        SDL_GetTextureSize(button->textures[IDLE], &w, &h);
    }

    button->x = 0;
    button->y = 0;
    button->width = w;
    button->height = h;

    return button;
}

void button_destroy(Button* button)
{
    verify(button == NULL, "Button does not exist");

    SDL_free(button);
}

void button_render(SDL_Renderer* renderer, const Button* button)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(button == NULL, "Button does not exist");

    SDL_FRect frect =
        {
            button->x,
            button->y,
            button->width,
            button->height,
        };
    SDL_RenderTexture(
        renderer, 
        button->textures[button->state], 
        NULL, 
        &frect
    );
}

static void button_set_state(const InputState* input, Button* button)
{
    verify(input == NULL, "InputState does not exist");
    verify(button == NULL, "Button does not exist");

    double mouse_x = input_get_mouse_x(input);
    double mouse_y = input_get_mouse_y(input);
    bool mouse_left_down = input_get_mouse_left_down(input);
    bool mouse_right_down = input_get_mouse_right_down(input);
    bool mouse_middle_down = input_get_mouse_middle_down(input);
    SDL_FRect frect =
        {
            button->x,
            button->y,
            button->width,
            button->height
        };
    if (point_intersects_rect(mouse_x, mouse_y, &frect))
    {
        if ((mouse_left_down && button->on_left_click != NULL) || 
            (mouse_right_down && button->on_right_click != NULL) ||
            (mouse_middle_down && button->on_middle_click != NULL))
        {
            button->state = PRESSED;
            return;
        }
        button->state = HOVERED;
        return;
    }
    button->state = IDLE;
}
static void button_press(const InputState* input, Button* button)
{
    verify(input == NULL, "InputState does not exist");
    verify(button == NULL, "Button does not exist");

    bool mouse_left_released = input_get_mouse_left_released(input);
    bool mouse_right_released = input_get_mouse_right_released(input);
    bool mouse_middle_released = input_get_mouse_middle_released(input);

    if (mouse_left_released)
    {
        if(button->on_left_click != NULL)
        {
            button_set_state(input, button);
            button->on_left_click(button->l_arg1, button->l_arg2);
            return;
        }
    }
    if (mouse_right_released)
    {
        if(button->on_right_click != NULL)
        {
            button_set_state(input, button);
            button->on_right_click(button->r_arg1, button->r_arg2);
            return;
        }
    }
    if (mouse_middle_released)
    {
        if(button->on_middle_click != NULL)
        {
            button_set_state(input, button);
            button->on_middle_click(button->m_arg1, button->m_arg2);
            return;
        }
    }
}
void button_update(const InputState* input, Button* button)
{
    verify(input == NULL, "InputState does not exist");
    verify(button == NULL, "Button does not exist");

    if (button->state == PRESSED)
    {
        button_press(input, button);
        return;
    }
    button_set_state(input, button);
}

void button_set_position(Button* button, float x, float y)
{
    verify(button == NULL, "Button does not exist");

    button->x = x;
    button->y = y;
}

void button_set_size(Button* button, float width, float height)
{
    verify(button == NULL, "Button does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    button->width = width;
    button->height = height;
}

void button_set_texture_idle(Button* button, SDL_Texture* texture)
{
    button->textures[IDLE] = texture;
}
void button_set_texture_hovered(Button* button, SDL_Texture* texture)
{
    button->textures[HOVERED] = texture;
}
void button_set_texture_pressed(Button* button, SDL_Texture* texture)
{
    button->textures[PRESSED] = texture;
}
void button_set_texture_all(Button* button, SDL_Texture* texture)
{
    button_set_texture_idle(button, texture);
    button_set_texture_hovered(button, texture);
    button_set_texture_pressed(button, texture);
}

void button_set_on_left_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* l_arg1, 
    void* l_arg2
)
{
    button->on_left_click = func;
    button->l_arg1 = l_arg1;
    button->l_arg2 = l_arg2;
}
void button_set_on_right_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* r_arg1, 
    void* r_arg2
)
{
    button->on_right_click = func;
    button->r_arg1 = r_arg1;
    button->r_arg2 = r_arg2;
}
void button_set_on_middle_click_fn(
    Button* button, 
    void (*func)(void* arg1, void* arg2), 
    void* m_arg1, 
    void* m_arg2
)
{
    button->on_middle_click = func;
    button->m_arg1 = m_arg1;
    button->m_arg2 = m_arg2;
}

float button_get_x(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->x;
}
float button_get_y(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->y;
}
float button_get_width(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->width;
}
float button_get_height(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    return button->height;
}
SDL_FRect button_get_frect(const Button* button)
{
    verify(button == NULL, "Button does not exist");

    SDL_FRect frect =
        {
            button->x,
            button->y,
            button->width,
            button->height,
        };
    return frect;
}



static void destroy(void* b)
{
    Button* button = (Button*)b;
    verify(button == NULL, "Button does not exist");
    button_destroy(button);
}
static TypeOps ops =
    {
        destroy
        // is_equal,
    };
TypeOps* button_ops()
{
    return &ops;
}
