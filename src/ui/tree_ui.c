#include "include/ui/tree_ui.h"

static void tree_ui_set(TreeUI* ui);
static void purchase_upgrade(void* game, void* index);

struct TreeUI
{
    Game* game;

    Window* window;

    TTF_Font* font;
    SDL_Color white;
};

TreeUI* tree_ui_create(SDL_Renderer* renderer, Game* game, float x, float y, float width, float height)
{
    TreeUI* ui = SDL_malloc(sizeof(TreeUI));

    ui->game = game;

    SDL_Texture* texture = SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_TREE_BACKGROUND);
    ui->window = window_create(
        x, 
        y, 
        width, 
        height, 
        texture,
        WINDOW_DRAGGABLE | WINDOW_ZOOMABLE
    );
    window_load_textures(renderer, ui->window, tree_textures, TREE_TEXTURES_COUNT);

    ui->font = TTF_OpenFont(FONT_PATH, FONT_SIZE * g_app_scale);
    verify(ui->font == NULL, "Could not open font");
    ui->white = color_white();

    window_set_scale(ui->window, g_app_scale * 5);

    tree_ui_set(ui);

    return ui;
}

void tree_ui_destroy(TreeUI* ui)
{
    window_destroy(ui->window);
    TTF_CloseFont(ui->font);
    SDL_free(ui);
}

void tree_ui_render(SDL_Renderer* renderer, const TreeUI* ui)
{
    verify(ui == NULL, "TreeUI does not exist");
    window_render(renderer, ui->window);
}

void tree_ui_update(InputState* input, TreeUI* ui)
{
    window_update(input, ui->window);
}

static void tree_ui_set(TreeUI* ui)
{
    Tree* tree = game_get_tree(ui->game);
    for (int i = 0; i < graph_get_size(tree); i++)
    {
        SDL_Texture* texture = window_get_texture(ui->window, TEXTURE_NODE_ADD_PAWN);
        Button* button = button_create(texture, NULL, texture);
        button_set_size(button, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);
        int* index = SDL_malloc(sizeof(int));
        *index = i;
        Function* func = function_create(purchase_upgrade, ui->game, index);
        button_set_on_click_fn(button, MOUSE_LEFT, func);

        float x = 0;
        float y = 0;
        switch (i)
        {
            case 0:
                x = 2;
                y = 0;
                break;
            case 1:
                x = 2;
                y = 1;
                break;
            case 2:
                x = 1;
                y = 2;
                break;
            case 3:
                x = 1;
                y = 3;
                break;
            case 4:
                x = 3;
                y = 2;
                break;
            case 5:
                x = 3;
                y = 3;
                break;
            case 6:
                x = 6;
                y = 0;
                break;
            case 7:
                x = 5;
                y = 1;
                break;
            case 8:
                x = 5;
                y = 2;
                break;
            case 9:
                x = 8;
                y = 1;
                break;
            case 10:
                x = 7;
                y = 2;
                break;
            case 11:
                x = 9;
                y = 2;
                break;
            case 12:
                x = 9;
                y = 3;
                break;
        }
        x *= button_get_width(button);
        y *= button_get_height(button);

        x += 0.1 * button_get_width(button);
        y += 0.1 * button_get_height(button);

        window_add_button(ui->window, button, x, y);
    }
}

static void purchase_upgrade(void* game, void* index)
{
    int i = *(int*)index;

    game_purchase_upgrade(game, i);
}
