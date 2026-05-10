#include "include/game/game.h"

struct Game
{
    Board* board;
    Tree* tree;
};

Game* game_create()
{
    Game* game = SDL_malloc(sizeof(Game));
    verify(game == NULL, "Game could not be created: malloc");

    game->board = NULL;
    game->tree = NULL;

    return game;
}

void game_destroy(Game* game)
{
    verify_game(game);

    board_destroy(game->board);
    tree_destroy(game->tree);

    SDL_free(game);

}

void game_start(Game* game)
{
    verify_game(game);

    game->board = board_create(GAME_STARTING_COL_NUM, GAME_STARTING_ROW_NUM);
    board_set_default(game->board);

    game->tree = tree_create();
    tree_set_default(game->tree);
}

Board* game_get_board(const Game* game)
{
    return game->board;
}

Tree* game_get_tree(const Game* game)
{
    return game->tree;
}

void verify_game(const Game* game)
{
    verify(game == NULL, "game does not exist");
}
