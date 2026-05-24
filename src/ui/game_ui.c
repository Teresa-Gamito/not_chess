#include "ui/game_ui.h"

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
    TreeUI* tree_ui;
};

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer)
{
    GameUI* ui = SDL_malloc(sizeof(GameUI));
    verify(ui == NULL, "GameUI could not be created: malloc");

    ui->game = game;

    ui->board_ui = board_ui_create(
        renderer,
        game,
        0,
        0,
        g_app_window_width / 2,
        g_app_window_height
    );

    ui->tree_ui = tree_ui_create(
        renderer, 
        game,
        g_app_window_width / 2, 
        0, 
        g_app_window_width / 2, 
        g_app_window_height
    );

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

    tree_ui_render(renderer, ui->tree_ui);
    board_ui_render(renderer, ui->board_ui);
}

static GameResult update_keys(InputState* input, GameUI* ui)
{
    (void)ui;
    if (keyboard_get_pressed(input, SDL_SCANCODE_ESCAPE))
    {
        return GAME_RESULT_EXIT;
    }
    return GAME_RESULT_CONTINUE;
}

GameResult game_ui_update(InputState* input, GameUI* ui)
{
    verify_game_ui(ui);
    verify_input(input);

    GameResult result;

    result = update_keys(input, ui);
    if (result != GAME_RESULT_CONTINUE) return result;

    tree_ui_update(input, ui->tree_ui);

    switch (board_ui_update(input, ui->board_ui))
    {
        case BOARD_RESULT_ADVANCE_TURN:
            game_advance_turn(ui->game);
            result = GAME_RESULT_CONTINUE;
            break;

        case BOARD_RESULT_WIN_WHITE:
            result = GAME_RESULT_WIN_WHITE;
            break;

        case BOARD_RESULT_WIN_BLACK:
            result = GAME_RESULT_WIN_BLACK;
            break;

        case BOARD_RESULT_CONTINUE:
        default:
            result = GAME_RESULT_CONTINUE;
            break;
    }

    return result;
}

void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
