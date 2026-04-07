#include "../../include/game/game.h"

struct GameState {
    Window** windows; /* array of pointers to windows */
    int window_count; /* number of windows */

    BoardState* board;
};

GameState* game_create()
{
    GameState* game = SDL_malloc(sizeof(GameState));

    game->windows = NULL;
    game->window_count = 0;

    game->board = board_create();

    return game;
}

void game_destroy(GameState* game)
{
    board_destroy(game->board);
    for (int i = 0; i < game->window_count; i++)
    {
        window_destroy(game->windows[i]);
    }
    SDL_free(game);
}

void game_add_window(GameState* game, Window* window)
{
    game->windows = SDL_realloc(game->windows, (game->window_count + 1) * sizeof(Window*));
    (game->window_count)++;
    game->windows[game->window_count] = window;
}
