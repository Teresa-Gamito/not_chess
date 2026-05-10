#include "game/ui/game_ui.h"
#include "inputstate.h"
#include <SDL3/SDL_log.h>

typedef enum GameUIState
{
    GAME_UI_RUNNING,
    GAME_UI_PAUSED,
    GAME_UI_END,
} GameUIState;

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
    TreeUI* tree_ui;
    // player info
    // properties
    // rules
    // buttons
    Menu* menu;

    SDL_Renderer* renderer;

    GameUIState state;
};

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer)
{
    GameUI* ui = SDL_malloc(sizeof(GameUI));

    ui->game = game;

    ui->board_ui = board_ui_create(
        renderer,
        game_get_board(ui->game),
        0,
        0,
        200,
        200
    );
    ui->tree_ui = tree_ui_create(
        renderer,
        game_get_tree(ui->game),
        ui->board_ui,
        200,
        0,
        100,
        200
    );
    ui->menu = NULL;

    ui->renderer = renderer;

    ui->state = GAME_UI_RUNNING;

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);
    if (ui->menu != NULL) menu_destroy(ui->menu);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    board_ui_render(renderer, ui->board_ui);
    tree_ui_render(renderer, ui->tree_ui);
    if (ui->menu == NULL)
    {
        return;
    }
    menu_render(renderer, ui->menu);
}
void game_ui_update(InputState* input, GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
    verify_input(input);

    if (ui->state != GAME_UI_RUNNING)
    {
        menu_update(input, ui->menu);
        return;
    }

    board_ui_update(input, ui->board_ui);
    tree_ui_update(input, ui->tree_ui);
}

void game_ui_toggle_pause(GameUI* ui)
{
    if (ui->state != GAME_UI_PAUSED)
    {
        ui->state = GAME_UI_PAUSED;
        ui->menu = menu_create(
            ui->renderer,
            (float)g_app_window_width / 4,
            (float)g_app_window_height / 2,
            (float)g_app_window_width / 4,
            (float)g_app_window_height / 2,
            SCREEN_MENU_PAUSE_MAIN
        );
    }
    else
    {
        ui->state = GAME_UI_RUNNING;
        menu_destroy(ui->menu);
        ui->menu = NULL;
    }
}
