#include "include/ui/tree_ui.h"
#include "ui/ui_elements/sprite.h"

static void tree_ui_set(TreeUI* ui);

struct TreeUI
{
    Tree* tree;
    Window* window;

    TTF_Font* font;
    SDL_Color white;
};

TreeUI* tree_ui_create(SDL_Renderer* renderer, Tree* tree, float x, float y, float width, float height)
{
    TreeUI* ui = SDL_malloc(sizeof(TreeUI));
    ui->tree = tree;


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
    for (int i = 0; i < graph_get_size(ui->tree); i++)
    {
        SDL_Texture* texture = window_get_texture(ui->window, TEXTURE_NODE_ADD_PAWN);
        Sprite* sprite = sprite_create(texture);

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
        x *= sprite_get_width(sprite);
        y *= sprite_get_height(sprite);

        x += 0.1 * sprite_get_width(sprite);
        y += 0.1 * sprite_get_height(sprite);
        window_add_sprite(ui->window, sprite, x, y);
    }
}

