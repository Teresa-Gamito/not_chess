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

static void board_set_default_layout(Board* board);



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
static void board_set_default_layout(Board* board)
{
    verify(board == NULL, "Board does not exist");

    board_add_piece_at(board, piece_create(KING, PIECE_WHITE), 4, 0);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_WHITE), 3, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 2, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 5, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 1, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 6, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 0, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 7, 0);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 0, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 1, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 2, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 3, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 4, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 5, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 6, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 7, 1);

    board_add_piece_at(board, piece_create(KING, PIECE_BLACK), 4, 7);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_BLACK), 3, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 2, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 1, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 0, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 7, 6);
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
