#include "include/game/board/player.h"
#include "helper_functions/error_handling.h"

struct Player
{
    int capturing_points;
    Color color;
};

Player* player_create(Color color, int starting_points)
{
    Player* player = SDL_malloc(sizeof(Player));
    verify(player == NULL, "Player could not be created: malloc");

    player->capturing_points = starting_points;
    player->color = color;

    return player;
}

void player_destroy(Player* player)
{
    verify(player == NULL, "Player does not exist");

    SDL_free(player);
}

void player_add_points(Player* player, int points)
{
    verify(player == NULL, "Player does not exist");
    player->capturing_points += points;
}

int player_get_points(const Player* player)
{
    verify(player == NULL, "Player does not exist");

    return player->capturing_points;
}
Color player_get_color(const Player* player)
{
    verify(player == NULL, "Player does not exist");

    return player->color;
}
