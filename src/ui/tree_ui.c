#include "include/ui/tree_ui.h"

static void tree_ui_set(TreeUI* ui);

struct TreeUI
{
    Game* game;

    Window* window;

    TTF_Font* font;
    SDL_Color white;

    int selected_upgrade;
    bool is_selected;

    Sound* sound_click;
    Sound* sound_error;
};

typedef enum Arrow
{
    LINE_UD,
    LINE_LR,
    LINE_DL,
    LINE_DR,
    LINE_UL,
    LINE_UR,
    LINE_A,
    ARROW_UD
} Arrow;

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

    ui->sound_click = sound_load(PATH_SOUND_CLICK);
    ui->sound_error = sound_load(PATH_SOUND_ERROR);

    ui->selected_upgrade = 0;
    ui->is_selected = false;

    return ui;
}

void tree_ui_destroy(TreeUI* ui)
{
    window_destroy(ui->window);
    sound_unload(ui->sound_click);
    sound_unload(ui->sound_error);
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

static void tree_ui_add_button(TreeUI* ui, int index, float x, float y)
{
    Tree* tree = game_get_tree(ui->game);
    SDL_Texture* texture = window_get_texture(ui->window, TEXTURE_UPGRADE_ADD_PAWN);
    Button* button = button_create(texture, NULL, texture);
    button_set_size(button, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);
    int* i = SDL_malloc(sizeof(int));
    *i = index;
    Function* func = function_create(tree_ui_select_upgrade, ui, i);
    button_set_on_click_fn(button, MOUSE_LEFT, func);

    x *= button_get_width(button);
    y *= button_get_height(button);
    x += 0.1 * button_get_width(button);
    y += 0.1 * button_get_height(button);
    window_add_button(ui->window, button, x, y);
}

static void tree_ui_draw_line(TreeUI* ui, float x, float y, Arrow arrow)
{
    SDL_Texture* texture = window_get_texture(ui->window, arrow);
    Sprite* sprite = sprite_create(texture);
    sprite_set_size(sprite, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);
    x *= sprite_get_width(sprite);
    y *= sprite_get_height(sprite);
    x += 0.1 * sprite_get_width(sprite);
    y += 0.1 * sprite_get_height(sprite);
    window_add_sprite(ui->window, sprite, x, y);
}

static void tree_ui_set(TreeUI* ui)
{
    int map[27][21] = 
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0},
            {0, 0, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0},
            {0,-4,-2,-2,-7,-2,-2,-3, 0, 0,-1, 0, 0,-4,-2,-2,-7,-2,-2,-3, 0},
            {0,-8, 0, 0,-8, 0, 0,-1, 0, 0,-8, 0, 0,-8, 0, 0,-1, 0, 0,-1, 0},
            {0, 4, 0, 0, 5, 0, 0,-1, 0, 0, 6, 0, 0, 7, 0, 0,-1, 0, 0,-1, 0},
            {0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0,-8, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0, 8, 0, 0, 9, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0,-1, 0, 0, 0, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0,-1, 0, 0,-4,-2,-2,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0},
            {0,-8, 0, 0,-8, 0, 0,-8, 0, 0,-8, 0, 0,-8, 0, 0,-8, 0, 0,-1, 0},
            {0,10, 0, 0,11, 0, 0,12, 0, 0,13, 0, 0,14, 0, 0,15, 0, 0,-1, 0},
            {0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0,-8, 0, 0,-1, 0, 0,-1, 0, 0,-8, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0,16, 0, 0,-1, 0, 0,-1, 0, 0,17, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0, 0, 0, 0,-1, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0, 0, 0, 0,-1, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0},
            {0, 0, 0, 0,-8, 0, 0,-8, 0, 0, 0, 0, 0,-8, 0, 0, 0, 0, 0,-8, 0},
            {0, 0, 0, 0,18, 0, 0,19, 0, 0, 0, 0, 0,20, 0, 0, 0, 0, 0,21, 0},
            {0, 0, 0, 0,-1, 0, 0,-1, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0,-6,-2,-2,-2,-2,-2,-3, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-8, 0, 0,-8, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,22, 0, 0,23, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        };

    for (int col = 0; col < 21; col++)
    {
        for (int row = 0; row < 27; row++)
        {
            int curr = map[row][col];
            if (curr == 0) continue;
            if (curr > 0)
            {
                tree_ui_add_button(ui, curr - 1, col, row);
                continue;
            }
            if (curr < 0) 
            {
                tree_ui_draw_line(ui, col, row, SDL_abs(curr) - 1);
            }
        }
    }
}

Window* tree_ui_get_window(TreeUI* ui)
{
    return ui->window;
}

void tree_ui_select_upgrade(void* tree_ui, void* index)
{
    TreeUI* ui = tree_ui;
    int i = *(int*)index;
    ui->selected_upgrade = i;
    ui->is_selected = true;
    sound_play(ui->sound_click);
}

void tree_ui_deselect_upgrade(TreeUI* ui)
{
    ui->is_selected = false;
}

int tree_ui_get_selected_upgrade(TreeUI* ui)
{
    return ui->selected_upgrade;
}

bool tree_ui_has_selected_upgrade(TreeUI* ui)
{
    return ui->is_selected;
}
