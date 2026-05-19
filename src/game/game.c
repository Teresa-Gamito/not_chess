#include "include/game/game.h"

struct Game
{
    Board* board;
};

Game* game_create()
{
    Game* game = SDL_malloc(sizeof(Game));
    verify(game == NULL, "Game could not be created: malloc");

    game->board = NULL;

    return game;
}

void game_destroy(Game* game)
{
    verify_game(game);

    board_destroy(game->board);

    SDL_free(game);

}

void game_start(Game* game)
{
    verify_game(game);

    game->board = board_create(GAME_STARTING_COL_NUM, GAME_STARTING_ROW_NUM);
    board_set_default(game->board);
}

void game_restart(Game* game)
{
    game_destroy(game);
    game = game_create();
    game_start(game);
}

Board* game_get_board(const Game* game)
{
    return game->board;
}

void verify_game(const Game* game)
{
    verify(game == NULL, "game does not exist");
}
