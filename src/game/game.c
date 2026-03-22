#include "../../include/game/game.h"

struct GameState {
    // TODO   
};

struct UIState {
    // TODO   
};

GameState* game_state_create()
{
    // TODO replace malloc
    GameState* game_state = malloc(sizeof(GameState));
    if (!game_state)
    {
        // throw error
    }
    // TODO create structure contents
    return game_state;
}

void game_state_destroy(GameState* game_state)
{
    if (!game_state)
    {
        // TODO throw error
        return;
    }
    // TODO free structure contents
    free(game_state);
}

UIState* ui_state_create()
{
    // TODO replace malloc
    UIState* ui_state = malloc(sizeof(UIState));
    if (!ui_state)
    {
        // TODO throw error
    }
    // TODO create structure contents
    return ui_state;
}

void ui_state_destroy(UIState* ui_state)
{
    if (!ui_state)
    {
        // TODO throw error
        return;
    }
    // TODO free structure contents
    free(ui_state);
}
