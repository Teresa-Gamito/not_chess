#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/error_handling.h"
#include "include/game/color.h"

typedef struct Player Player;

Player* player_create(Color color, int starting_points);
void player_destroy(Player* player);

void player_add_points(Player* player, int points);

int player_get_points(const Player* player);
Color player_get_color(const Player* player);

void verify_player(const Player* player);
