#include "../../include/game/gamestate.h"

typedef enum GameWindows
{
    GAME_WINDOW_BOARD,
    // GAME_WINDOW_TREE,
    // GAME_WINDOW_

    GAME_WINDOW_COUNT
} GameWindows;

struct GameState {
    Board* board;
};



// ========== create ==========
GameState* gamestate_create()
{
    GameState* game = SDL_malloc(sizeof(GameState));
    verify(game == NULL, "GameState could not be created: malloc");

    game->board = NULL;

    return game;
}
void gamestate_set_default(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    game->board = board_create(
        renderer,
        BOARD_STARTING_COL_NUM,
        BOARD_STARTING_ROW_NUM,
        50,
        30,
        600,
        600
    );
    board_set_default_layout(game->board);

    board_add_piece_at(
        game->board,
        piece_create(PAWN, PIECE_WHITE),
        3,
        3
    );
}



// ========== destroy ==========
void gamestate_destroy(GameState* game)
{
    verify(game == NULL, "GameState does not exist");

    board_destroy(game->board);
    SDL_free(game);
}



// ========== render ==========
void game_render(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    Board* board = gamestate_get_board(game);

    window_render(renderer, board_get_window(board));
}



// ========== update ==========
void gamestate_update(InputState* input, GameState* game)
{
    board_update(input, gamestate_get_board(game));
}



// ========== get ==========
Board* gamestate_get_board(GameState* game)
{
    verify(game == NULL, "GameState does not exist");
    return game->board;
}
