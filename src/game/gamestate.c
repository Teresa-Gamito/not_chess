#include "../../include/game/gamestate.h"

struct GameState {
    Window** windows;
    int window_count;

    Board* board;
};



// ========== create ==========
GameState* gamestate_create()
{
    GameState* game = SDL_malloc(sizeof(GameState));

    game->windows = NULL;
    game->window_count = 0;

    game->windows = NULL;
    game->window_count = 0;

    game->board = NULL;

    return game;
}
void gamestate_set_default(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    game->board = board_create(BOARD_STARTING_COL_NUM, BOARD_STARTING_ROW_NUM);
    board_set_default(gamestate_get_board(game));

    Window* board_window = board_create_window(
        renderer, 
        gamestate_get_board(game), 
        50, 
        50, 
        1000, 
        1000
    );
    gamestate_add_window(game, board_window);
}



// ========== destroy ==========
void gamestate_destroy(GameState* game)
{
    verify(game == NULL, "GameState does not exist");

    board_destroy(game->board);
    for (int i = 0; i < game->window_count; i++)
    {
        window_destroy(game->windows[i]);
    }
    SDL_free(game);
}



// ========== render ==========
void game_render(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    for (int i = 0; i < game->window_count; i++)
    {
        window_render(renderer, game->windows[i]);
    }
}



// ========== set ==========
void gamestate_add_window(GameState* game, Window* window)
{
    verify(game == NULL, "GameState does not exist");
    verify(window == NULL, "Window does not exist");

    game->windows = SDL_realloc(game->windows, (game->window_count + 1) * sizeof(Window*));
    verify(game->windows == NULL, "Could not add window: realloc");
    game->windows[game->window_count] = window;
    (game->window_count)++;
}



// ========== get ==========
Board* gamestate_get_board(GameState* game)
{
    verify(game == NULL, "GameState does not exist");
    return game->board;
}


