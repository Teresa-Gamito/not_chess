#include "include/game/player/player.h"

struct Player
{
    Color color;
    int capturing_points;
};

Player* player_create(Color color, int starting_points)
{
    Player* player = SDL_malloc(sizeof(Player));
    verify(player == NULL, "Player could not be created: malloc");

    player->color = color;
    player->capturing_points = starting_points;

    return player;
}

void player_destroy(Player* player)
{
    verify_player(player);
    SDL_free(player);
}

void player_add_points(Player* player, int points)
{
    verify_player(player);
    player->capturing_points += points;
}

int player_get_points(const Player* player)
{
    verify_player(player);
    return player->capturing_points;
}

Color player_get_color(const Player* player)
{
    verify_player(player);
    return player->color;
}

void verify_player(const Player* player)
{
    verify(player == NULL, "Player does not exist");
}
