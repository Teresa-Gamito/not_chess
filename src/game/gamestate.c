#include "include/game/gamestate.h"
#include "appstate.h"
#include "game/board/board.h"
#include "helper_functions/error_handling.h"
#include "ui_elements/window.h"

struct GameState
{
    Board* board;
    BoardUI* board_ui;

    Tree* tree;
    TreeUI* tree_ui;
};

GameState* gamestate_create()
{
    GameState* gamestate = SDL_malloc(sizeof(GameState));
    verify(gamestate == NULL, "GameState could not be created: malloc");

    gamestate->board = NULL;
    gamestate->board_ui = NULL;

    gamestate->tree = NULL;
    gamestate->tree_ui = NULL;

    return gamestate;
}

void gamestate_destroy(GameState* gamestate)
{
    verify(gamestate == NULL, "GameState does not exist");

    board_destroy(gamestate->board);
    board_ui_destroy(gamestate->board_ui);
    SDL_free(gamestate);
}

void game_render(SDL_Renderer* renderer, GameState* gamestate)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    window_render(renderer, board_ui_get_window(gamestate->board_ui));
    window_render(renderer, tree_ui_get_window(gamestate->tree_ui));
}

void game_update(InputState* input, GameState* gamestate)
{
    verify(input == NULL, "InputState does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    window_update(input, board_ui_get_window(gamestate->board_ui));
    window_update(input, tree_ui_get_window(gamestate->tree_ui));
}

void game_start(SDL_Renderer* renderer, GameState* gamestate)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    gamestate->board = board_create(8, 8);
    board_set_default(gamestate->board);
    gamestate->board_ui = board_ui_create(
        renderer,
        gamestate->board,
        0,
        0,
        (float)g_app_window_height,
        (float)g_app_window_height
    );

    gamestate->tree = tree_create();
    tree_set_default(gamestate->tree);
    gamestate->tree_ui = tree_ui_create(
        renderer,
        gamestate->tree,
        gamestate->board,
        (float)g_app_window_height,
        0,
        100,
        (float)g_app_window_height
    );

}
