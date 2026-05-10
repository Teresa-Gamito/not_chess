#include "include/ui_elements/button.h"

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
    float anchor_x;
    float anchor_y;
    float scale;

    Vector* on_click_fn;
};

Button* button_create(
    SDL_Texture* texture_idle, 
    SDL_Texture* texture_hovered, 
    SDL_Texture* texture_pressed
)
{
    Button* button = SDL_malloc(sizeof(Button));
    verify(button == NULL, "Button could not be created: malloc");

    button->textures[IDLE] = texture_idle;
    button->textures[HOVERED] = texture_hovered;
    button->textures[PRESSED] = texture_pressed;

    button->state = IDLE;

    button->on_click_fn = vector_create(function_ops());

    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        Function* func = function_create(NULL, NULL, NULL);
        vector_add(button->on_click_fn, func);
    }

    float w = 0;
    float h = 0;
    SDL_GetTextureSize(button->textures[IDLE], &w, &h);

    button->x = 0;
    button->y = 0;
    button->width = w;
    button->height = h;
    button->anchor_x = 0;
    button->anchor_y = 0;
    button->scale = 1;

    return button;
}

void button_destroy(Button* button)
{
    verify_button(button);

    vector_destroy(button->on_click_fn);
    SDL_free(button);
}

void button_render(SDL_Renderer* renderer, const Button* button)
{
    verify_renderer(renderer);
    verify_button(button);

    SDL_FRect frect = button_get_frect(button);
    SDL_RenderTexture(
        renderer, 
        button->textures[button->state], 
        NULL, 
        &frect
    );
}

static void button_set_state(const InputState* input, Button* button)
{
    double mouse_x = mouse_get_x(input);
    double mouse_y = mouse_get_y(input);
    SDL_FRect frect = button_get_frect(button);
    if (!point_intersects_rect(mouse_x, mouse_y, &frect))
    {
        button->state = IDLE;
        return;
    }
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        if (!mouse_get_down(input, i))
        {
            continue;
        }
        button->state = PRESSED;
        return;
    }
    button->state = HOVERED;
    return;
}
static void button_press(const InputState* input, Button* button)
{
    int i;
    for (i = 0; i < MOUSE_BUTTON_COUNT; i++)
    {
        if (!mouse_get_released(input, i))
        {
            continue;
        }
        button_set_state(input, button);
        Function* func = vector_get_at(button->on_click_fn, i);
        function_execute(func);
        return;
    }
}
void button_update(const InputState* input, Button* button)
{
    verify_input(input);
    verify_button(button);

    if (button->state == PRESSED)
    {
        button_press(input, button);
        return;
    }
    button_set_state(input, button);
}

void button_set_position(Button* button, float x, float y)
{
    verify_button(button);

    button->x = x;
    button->y = y;
}
void button_set_anchor(Button* button, float anchor_x, float anchor_y)
{
    verify_button(button);

    button->anchor_x = anchor_x;
    button->anchor_y = anchor_y;
}

void button_set_size(Button* button, float width, float height)
{
    verify_button(button);
    verify_size(width, height);

    button->width = width;
    button->height = height;
}
void button_set_scale(Button* button, float scale)
{
    verify_button(button);

    button->scale = scale;
}

void button_set_texture_idle(Button* button, SDL_Texture* texture)
{
    verify_button(button);

    button->textures[IDLE] = texture;
}
void button_set_texture_hovered(Button* button, SDL_Texture* texture)
{
    verify_button(button);

    button->textures[HOVERED] = texture;
}
void button_set_texture_pressed(Button* button, SDL_Texture* texture)
{
    verify_button(button);

    button->textures[PRESSED] = texture;
}
void button_set_texture_all(Button* button, SDL_Texture* texture)
{
    verify_button(button);

    button_set_texture_idle(button, texture);
    button_set_texture_hovered(button, texture);
    button_set_texture_pressed(button, texture);
}

void button_set_on_click_fn(Button* button, MouseButton mouse_button, Function* function)
{
    Function* old_func = vector_get_at(button->on_click_fn, mouse_button);
    vector_set_at(button->on_click_fn,function, mouse_button);
    function_destroy(old_func);
}

float button_get_x(const Button* button)
{
    verify_button(button);

    return button->x;
}
float button_get_y(const Button* button)
{
    verify_button(button);

    return button->y;
}
float button_get_width(const Button* button)
{
    verify_button(button);

    return button->width;
}
float button_get_height(const Button* button)
{
    verify_button(button);

    return button->height;
}
SDL_FRect button_get_frect(const Button* button)
{
    verify_button(button);

    SDL_FRect frect =
        {
            button->x * button->scale + button->anchor_x,
            button->y * button->scale + button->anchor_y,
            button->width * button->scale,
            button->height * button->scale,
        };
    return frect;
}



static void destroy(void* button)
{
    verify_button(button);
    button_destroy(button);
}
static TypeOps ops =
    {
        destroy
    };
TypeOps* button_ops()
{
    return &ops;
}



void verify_button(const Button* button)
{
    verify(button == NULL, "Button does not exist");
}
