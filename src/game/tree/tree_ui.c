#include "include/game/tree/tree_ui.h"
#include "game/board/board.h"
#include "game/board/board_ui.h"

struct TreeUI
{
    Tree* tree;
    Window* window;

    Board* board;
    BoardUI* board_ui;
};

TreeUI* tree_ui_create(
    SDL_Renderer* renderer, 
    Tree* tree,
    BoardUI* board_ui,
    float x, 
    float y, 
    float width, 
    float height
)
{
    TreeUI* ui = SDL_malloc(sizeof(TreeUI));
    verify(ui == NULL, "TreeUI could not be created: malloc");

    ui->tree = tree;

    SDL_Texture* background_texture = SDL_CreateTextureFromPNG(
        renderer, 
        PATH_TEXTURE_WINDOW_TREE_BACKGROUND
    );
    ui->window = window_create(x, y, width, height, background_texture);
    window_load_textures(
        renderer, 
        ui->window, 
        tree_textures, 
        TREE_TEXTURES_COUNT
    );

    ui->board = board_ui_get_board(board_ui);
    ui->board_ui = board_ui;

    tree_ui_update(ui);

    return ui;
}

void tree_ui_destroy(TreeUI* ui)
{
    window_destroy(ui->window);
    SDL_free(ui);
}

Window* tree_ui_get_window(TreeUI* ui)
{
    return ui->window;
}

static void tree_ui_set_nodes(TreeUI* ui)
{
    Tree* tree = ui->tree;
    Window* window = ui->window;

    SDL_Texture* button_idle = window_get_texture(ui->window, TEXTURE_TREE_BUTTON_IDLE);
    SDL_Texture* button_hovered = window_get_texture(ui->window, TEXTURE_TREE_BUTTON_HOVERED);
    SDL_Texture* button_pressed = window_get_texture(ui->window, TEXTURE_TREE_BUTTON_PRESSED);

    SDL_Texture* texture_add_pawn = window_get_texture(ui->window, TEXTURE_NODE_ADD_PAWN);

    for (int i = 0; i < tree_get_node_count(tree); i++)
    {
        Node* node = tree_get_node_at(tree, i);

        Button* button = button_create(
            button_idle,
            button_hovered,
            button_pressed
        );
        button_set_size(button, 90, 90);
        button_set_on_left_click_fn(button, node_purchase, ui->board_ui, node);
        window_add_button(window, button, 5, 5 + (100 * i));

        Sprite* sprite = sprite_create(texture_add_pawn);
        sprite_set_size(sprite, 90, 90);
        window_add_sprite(window, sprite, 5, 5 + (100 * i));
    }
}
void tree_ui_update(TreeUI* ui)
{
    Window* window = ui->window;

    window_destroy_content(window);

    tree_ui_set_nodes(ui);
}
