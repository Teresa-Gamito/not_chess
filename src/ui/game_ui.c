#include "ui/game_ui.h"
#include "game/game.h"
#include "helper_functions/helper_functions.h"
#include "ui/board_ui.h"

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
};

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer)
{
    GameUI* ui = SDL_malloc(sizeof(GameUI));
    verify(ui == NULL, "GameUI could not be created: malloc");

    ui->game = game;

    ui->board_ui = board_ui_create(
        renderer,
        game_get_board(ui->game),
        game_get_active_player(ui->game),
        0,
        0,
        g_app_window_width,
        g_app_window_height
    );

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

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

    switch (board_ui_update(input, ui->board_ui))
    {
        case BOARD_RESULT_ADVANCE_TURN:
            game_advance_turn(ui->game);
            board_ui_set_active_player(ui->board_ui, game_get_active_player(ui->game));
            return GAME_RESULT_CONTINUE;

        case BOARD_RESULT_WIN_WHITE:
            return GAME_RESULT_WIN_WHITE;

        case BOARD_RESULT_WIN_BLACK:
            return GAME_RESULT_WIN_BLACK;

        case BOARD_RESULT_CONTINUE:
        default:
            return GAME_RESULT_CONTINUE;
    }
}



void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
