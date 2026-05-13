#include "include/ui_elements/menu.h"

struct Menu
{
    Window* window;

    TTF_Font* font;
    SDL_Color* text_color;
};

Menu* menu_create(SDL_Renderer* renderer, float x, float y, float width, float height)
{
    Menu* menu = SDL_malloc(sizeof(Menu));

    SDL_Texture* background = SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_MENU_BACKGROUND);
    menu->window = window_create(x, y, width, height, background, 0);

    window_load_textures(renderer, menu->window, menu_textures, MENU_TEXTURE_COUNT);

    TTF_Font* font = TTF_OpenFont(FONT_PATH_MENU, FONT_SIZE_MENU);
    menu->font = font;

    menu->text_color = SDL_malloc(sizeof(SDL_Color));
    *menu->text_color = color_black();

    return menu;
}

void menu_destroy(Menu* menu)
{
    verify_menu(menu);

    TTF_CloseFont(menu->font);
    SDL_free(menu->text_color);
    window_destroy(menu->window);
    SDL_free(menu);
}

void menu_destroy_content(Menu* menu)
{
    verify_menu(menu);

    window_destroy_content(menu->window);
}

void menu_render(SDL_Renderer* renderer, Menu* menu)
{
    verify_menu(menu);

    window_render(renderer, menu->window);
}
void menu_update(InputState* input, Menu* menu)
{
    verify_menu(menu);

    window_update(input, menu->window);
}

void menu_add_button(SDL_Renderer* renderer, Menu* menu, Function* function, const char* text)
{
    verify_menu(menu);

    Window* window = menu->window;

    // add button
    SDL_Texture* texture_button = window_get_texture(window, TEXTURE_MENU_BUTTON);
    Button* button = button_create(
        texture_button, 
        texture_button, 
        texture_button
    );
    button_set_on_click_fn(button, MOUSE_LEFT, function);
    window_add_button(window, button, 0, 0);

    // add textbox
    Textbox* textbox = textbox_create(
        renderer, 
        menu->font, 
        menu->text_color, 
        text, 
        window_get_width(window), 
        TEXT_CENTERED
    );
    window_add_textbox(window, textbox, 0, 0);

    // align buttons and text
    Vector* buttons = window_get_buttons(window);
    Vector* textboxes = window_get_textboxes(window);
    int count = vector_get_count(buttons);
    for (int i = 0; i < count; i++)
    {
        float border = window_get_width(window) / 100; // TODO:
        float width = window_get_width(window) - border * 2;
        float height = (window_get_height(window) - border) / count - border;
        float x = border;
        float y = (height + border) * i + border;

        Button* curr_button = vector_get_at(buttons, i);
        button_set_position(curr_button, x, y);
        button_set_size(curr_button, width, height);

        Textbox* curr_textbox = vector_get_at(textboxes, i);
        textbox_set_position(curr_textbox, x + width / 2, y + height / 2 - textbox_get_height(curr_textbox) / 2);
    }
}

void verify_menu(const Menu* menu)
{
    verify(menu == NULL, "Menu does not exist");
}
