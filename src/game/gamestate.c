#include "include/game/gamestate.h"
#include "game/board/board_window.h"
#include <SDL3/SDL_log.h>

typedef enum PlayerColor
{
    PLAYER_WHITE,
    PLAYER_BLACK
} PlayerColor;

typedef struct Player 
{
    PlayerColor color;
    int capturing_points;
} Player;


struct GameState
{
    Board* board;
    BoardUI* board_ui;

    Player* player1;
    Player* player2;
    Player* active_player;
};

static void board_set_default_layout(Board* board);
static Player* player_create(PlayerColor color);
static void player_destroy(Player* player);

GameState* gamestate_create()
{
    GameState* gamestate = SDL_malloc(sizeof(GameState));
    verify(gamestate == NULL, "GameState could not be created: malloc");

    gamestate->board = NULL;
    gamestate->board_ui = NULL;

    gamestate->player1 = NULL;
    gamestate->player2 = NULL;
    gamestate->active_player = NULL;

    return gamestate;
}
static Player* player_create(PlayerColor color)
{
    Player* player = SDL_malloc(sizeof(Player));
    verify(player == NULL, "Player could not be created: malloc");

    player->color = color;
    player->capturing_points = PLAYER_STARTING_POINTS;

    return player;
}

void gamestate_destroy(GameState* gamestate)
{
    board_destroy(gamestate->board);
    board_ui_destroy(gamestate->board_ui);
    player_destroy(gamestate->player1);
    player_destroy(gamestate->player2);
    SDL_free(gamestate);
}
static void player_destroy(Player* player)
{
    SDL_free(player);
}

void game_render(SDL_Renderer* renderer, GameState* gamestate)
{
    window_render(renderer, board_ui_get_window(gamestate->board_ui));
}

void game_update(InputState* input, GameState* gamestate)
{
    window_update(input, board_ui_get_window(gamestate->board_ui));
}

void game_start(SDL_Renderer* renderer, GameState* gamestate)
{
    gamestate->board = board_create(8, 8);
    board_set_default_layout(gamestate->board);

    gamestate->board_ui = board_ui_create(
        renderer,
        gamestate->board, 
        50, 
        50, 
        600, 
        600
    );

    gamestate->player1 = player_create(PLAYER_WHITE);
    gamestate->player2 = player_create(PLAYER_BLACK);
    gamestate->active_player = gamestate->player1;
}
static void board_set_default_layout(Board* board)
{
    verify(board == NULL, "Board does not exist");

    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 0, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 1, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 2, 0);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_BLACK), 3, 0);
    board_add_piece_at(board, piece_create(KING, PIECE_BLACK), 4, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 5, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 6, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 7, 0);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 0, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 1, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 2, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 3, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 4, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 5, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 6, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 7, 1);

    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 0, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 1, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 2, 7);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_WHITE), 3, 7);
    board_add_piece_at(board, piece_create(KING, PIECE_WHITE), 4, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 7, 6);
}
