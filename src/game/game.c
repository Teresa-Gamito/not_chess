#include "include/game/game.h"
#include "game/player/player.h"

struct Game
{
    Board* board;
    // Tree* tree;

    Player* white;
    Player* black;
    Player* active_player;
};

Game* game_create()
{
    Game* game = SDL_malloc(sizeof(Game));
    verify(game == NULL, "Game could not be created: malloc");

    game->board = NULL;

    game->white = NULL;
    game->black = NULL;
    game->active_player = NULL;

    return game;
}

void game_destroy(Game* game)
{
    verify_game(game);

    board_destroy(game->board);

    player_destroy(game->white);
    player_destroy(game->black);

    SDL_free(game);

}

void game_start(Game* game)
{
    verify_game(game);

    game->board = board_create(GAME_STARTING_COL_NUM, GAME_STARTING_ROW_NUM);
    board_set_default(game->board);

    // game->tree = tree_create();

    game->white = player_create(WHITE, PLAYER_STARTING_POINTS);
    game->black = player_create(BLACK, PLAYER_STARTING_POINTS);
    game->active_player = game->white;
}

void game_advance_turn(Game* game)
{
    Player* player = game->active_player == game->white ? game->black : game->white;
    game->active_player = player;
}

Board* game_get_board(const Game* game)
{
    return game->board;
}

Player* game_get_active_player(const Game* game)
{
    return game->active_player;
}

void verify_game(const Game* game)
{
    verify(game == NULL, "game does not exist");
}
