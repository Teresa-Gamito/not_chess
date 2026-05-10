#include "include/menu/menu.h"

struct Menu
{
    SDL_Renderer* renderer;

    Window* window;
    MenuScreen screen;

    TTF_Font* font;
    SDL_Color* text_color;
};

static void menu_add_button(Menu* menu, Function* function, const char* text);
static void menu_go_to_screen(void* men, void* scr);

Menu* menu_create(SDL_Renderer* renderer, float x, float y, float width, float height, MenuScreen starting_menu)
{
    Menu* menu = SDL_malloc(sizeof(Menu));

    menu->renderer = renderer;

    SDL_Texture* background = SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_MENU_BACKGROUND);
    menu->window = window_create(x, y, width, height, background, 0);

    window_load_textures(renderer, menu->window, menu_textures, MENU_TEXTURE_COUNT);

    TTF_Font* font = TTF_OpenFont(FONT_PATH_MENU, FONT_SIZE_MENU);
    menu->font = font;

    menu->text_color = SDL_malloc(sizeof(SDL_Color));
    *menu->text_color = color_black();

    menu_go_to_screen(menu, &starting_menu);

    return menu;
}

void menu_destroy(Menu* menu)
{
    TTF_CloseFont(menu->font);
    SDL_free(menu->text_color);
    window_destroy(menu->window);
    SDL_free(menu);
}

void menu_render(SDL_Renderer* renderer, Menu* menu)
{
    window_render(renderer, menu->window);
}
void menu_update(InputState* input, Menu* menu)
{
    window_update(input, menu->window);
}

static void menu_add_button(Menu* menu, Function* function, const char* text)
{
    Window* window = menu->window;
    SDL_Renderer* renderer = menu->renderer;

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
        float border = window_get_width(window) / 100; 
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

static void menu_go_to_screen(void* men, void* scr)
{
    Menu* menu = (Menu*)men;
    MenuScreen* screen = (MenuScreen*)scr;

    window_destroy_content(menu->window);
    menu->screen = *screen;
    Function* func = NULL;
    static MenuScreen main_main = SCREEN_MENU_MAIN_MAIN;
    static MenuScreen main_options = SCREEN_MENU_MAIN_OPTIONS;
    static MenuScreen main_credits = SCREEN_MENU_MAIN_CREDITS;
    static MenuScreen main_quit = SCREEN_MENU_MAIN_QUIT;
    static MenuScreen pause_main = SCREEN_MENU_PAUSE_MAIN;
    static MenuScreen pause_options = SCREEN_MENU_PAUSE_OPTIONS;
    static MenuScreen pause_quit = SCREEN_MENU_PAUSE_QUIT;
    switch (menu->screen)
    {
        case SCREEN_MENU_MAIN_MAIN:
            func = NULL; // TODO:
            menu_add_button(menu, func, "START GAME");
            func = function_create(menu_go_to_screen, menu, &main_options);
            menu_add_button(menu, func, "OPTIONS");
            func = function_create(menu_go_to_screen, menu, &main_credits);
            menu_add_button(menu, func, "CREDITS");
            func = function_create(menu_go_to_screen, menu, &main_quit);
            menu_add_button(menu, func, "QUIT");
            break;

        case SCREEN_MENU_MAIN_OPTIONS:
            func = function_create(menu_go_to_screen, menu, &main_main);
            menu_add_button(menu, func, "BACK");
            break;

        case SCREEN_MENU_MAIN_CREDITS:
            func = function_create(menu_go_to_screen, menu, &main_main);
            menu_add_button(menu, func, "BACK");
            break;

        case SCREEN_MENU_MAIN_QUIT:
            func = function_create(app_quit, NULL, NULL);
            menu_add_button(menu, func, "QUIT");
            func = function_create(menu_go_to_screen, menu, &main_main);
            menu_add_button(menu, func, "BACK");
            break;


        case SCREEN_MENU_PAUSE_MAIN:
            func = NULL; // TODO:
            menu_add_button(menu, func, "RETURN");
            func = function_create(menu_go_to_screen, menu, &pause_options);
            menu_add_button(menu, func, "OPTIONS");
            func = function_create(menu_go_to_screen, menu, &pause_quit);
            menu_add_button(menu, func, "QUIT TO MAIN MENU");
            break;

        case SCREEN_MENU_PAUSE_OPTIONS:
            func = function_create(menu_go_to_screen, menu, &pause_main);
            menu_add_button(menu, func, "BACK");
            break;

        case SCREEN_MENU_PAUSE_QUIT:
            func = function_create(menu_go_to_screen, menu, &main_main);
            menu_add_button(menu, func, "QUIT TO MAIN MENU");

            func = function_create(menu_go_to_screen, menu, &pause_main);
            menu_add_button(menu, func, "BACK");
            break;

        default:
            break;
    }
}
