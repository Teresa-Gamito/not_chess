#include "../../include/game/gamestate.h"

struct GameState {
    // BoardState boardstate;
    Window *windows;
    // TODO:
};

GameState* game_state_create()
{
    GameState* game_state = SDL_malloc(sizeof(GameState));
    if (!game_state)
    {
        // TODO: throw error
    }
    // TODO: create structure contents
    return game_state;
}

void game_state_destroy(GameState* game_state)
{
    // TODO: free structure contents
    SDL_free(game_state);
}
