#include "../../include/game/gamestate.h"
#include <SDL3/SDL_render.h>

struct GameState {
    Window** windows;
    int window_count;

    SDL_Texture** textures;
    int texture_count;

    Board* board;
};

GameState* gamestate_create()
{
    GameState* game = SDL_malloc(sizeof(GameState));

    game->windows = NULL;
    game->window_count = 0;

    game->windows = NULL;
    game->window_count = 0;

    game->textures = NULL;
    game->texture_count = 0;

    game->board = NULL;

    return game;
}

void gamestate_set_default(SDL_Renderer* renderer, GameState* game)
{
    gamestate_load_textures(renderer, game);

    game->board = board_create(BOARD_STARTING_WIDTH, BOARD_STARTING_HEIGHT);
    board_set_default(gamestate_get_board(game));
}

void gamestate_load_textures(SDL_Renderer* renderer, GameState* game)
{
    game->texture_count = TEXTURE_COUNT;

    const char* path[] =
    {
        PATH_TEXTURE_WHITE_PAWN,
        PATH_TEXTURE_WHITE_ROOK,
        PATH_TEXTURE_WHITE_KNIGHT,
        PATH_TEXTURE_WHITE_BISHOP,
        PATH_TEXTURE_WHITE_QUEEN,
        PATH_TEXTURE_WHITE_KING,

        PATH_TEXTURE_BLACK_PAWN,
        PATH_TEXTURE_BLACK_ROOK,
        PATH_TEXTURE_BLACK_KNIGHT,
        PATH_TEXTURE_BLACK_BISHOP,
        PATH_TEXTURE_BLACK_QUEEN,
        PATH_TEXTURE_BLACK_KING,

        PATH_TEXTURE_BOARD_WHITE,
        PATH_TEXTURE_BOARD_BLACK
    };

    for (int i = 0; i < TEXTURE_COUNT; i++)
    {
        game->textures[i] = SDL_CreateTextureFromPNG(renderer, path[i]);
    }
}

void gamestate_render(SDL_Renderer* renderer, GameState* game)
{
    
}

void gamestate_destroy(GameState* game)
{
    board_destroy(game->board);
    for (int i = 0; i < game->window_count; i++)
    {
        window_destroy(game->windows[i]);
    }
    SDL_free(game);
}

void gamestate_add_window(GameState* game, Window* window)
{
    game->windows = SDL_realloc(game->windows, (game->window_count + 1) * sizeof(Window*));
    (game->window_count)++;
    game->windows[game->window_count] = window;
}


Board* gamestate_get_board(GameState* game)
{
    return game->board;
}
