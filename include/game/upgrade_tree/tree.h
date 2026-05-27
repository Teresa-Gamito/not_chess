#pragma once

#include <SDL3/SDL_stdinc.h>
#include "data_structures/graph.h"

typedef Graph Tree;

typedef enum UpgradeType
{
    UPGRADE_PEASANT, // done
    UPGRADE_CHECKERS,
    UPGRADE_STONKS,
    UPGRADE_SHOGI, // done
    UPGRADE_EXPANSION, // done
    UPGRADE_MORE_LAND, // done
    UPGRADE_ONLY_ONE,
    UPGRADE_KNIGHTHOOD,
    UPGRADE_DISGUISE, // done
    UPGRADE_REVERSE_DISGUISE, // done
    UPGRADE_SCHOLAR, // done
    UPGRADE_ROCK_SOLID, // done
    UPGRADE_PROMOTION, // done
    UPGRADE_ROYALTY, // done
    UPGRADE_CONTROL, // done
    UPGRADE_PRECIOUS, // done
    UPGRADE_MINE, // done
    UPGRADE_DEADTOUCH,
    UPGRADE_TRAMPLE,
    UPGRADE_DEBUFF,
    UPGRADE_TEMPORARY_ROYALTY,
    UPGRADE_AGAIN,
    UPGRADE_RISE, // done
    UPGRADE_SWAMP,
    UPGRADE_RULING,
    UPGRADE_GAMBLING, // done
    UPGRADE_HELLO,
    UPGRADE_FRIENDLY_FIRE, // done
    UPGRADE_TACTICAL_ADVANTAGE,
    UPGRADE_RUN,
    UPGRADE_LANCER,

    COUNT_UPGRADE,
} UpgradeType;

static char* upgrade_name[] =
    {
        "Peasant",
        "Checkers?",
        "Stonks?!",
        "Shogi?",
        "Expansion",
        "More land!",
        "There can only be one",
        "Knighthood",
        "Disguise",
        "Disguise but reversed",
        "Scholar",
        "Rock solid",
        "Promotion",
        "Royalty",
        "Control",
        "My precious",
        "MINE!",
        "Deadtouch",
        "Trample",
        "Debuff",
        "Temporary royalty",
        "Let me do it again",
        "Rise",
        "Get out of my swamp!",
        "Ruling",
        "Gambling addiction",
        "Hello there",
        "Friendly fire",
        "Tactical advantage",
        "Ruuuun!",
        "Lancer",
    };

static char* upgrade_description[] = 
    {
        "Add one pawn your side of the board",
        "Add one checkers piece to your side of the board",
        "Add one knight, your opponent adds one pawn",
        "Add one lance to your side of the board",
        "Expand the size of the board by one tile for each side",
        "Expand the size of the board by two tiles for each side",
        "Both players sacrifice one piece",
        "Sacrifice two pawns and add a knight",
        "Convert a rook into a bishop",
        "Convert a bishop into a rook",
        "Promote a pawn to a bishop",
        "Promote a pawn to a rook",
        "Promote a pawn to a queen",
        "Promote a queen to a king",
        "Gain control of an opponent pawn",
        "Gain control of an opponent piece (except kings and queens)",
        "Gain control of an opponent piece (except kings)",
        "Add trample to a piece",
        "Add deathtouch to a piece",
        "not added yet",
        "Promote a pawn for three turns",
        "Take a turn after this one",
        "Create a wall tile",
        "Create a poison tile",
        "Add a rule: Pawns can move three tiles on their first move",
        "Add a rule: Pawns have a 30% chance of being promoted when capturing a piece",
        "Add a rule: Each turn a random piece is destroyed",
        "Add a rule: Players can capture their own pieces",
        "Add a rule: Pieces are worth one more point",
        "Add a rule: Pieces have a 25% chance of being able to move again after capturing",
        "Promote a lance",
    };

Tree* tree_create();
void tree_destroy(Tree *tree);

void tree_set_default(Tree* tree);

UpgradeType tree_upgrade_purchase(Tree* tree, int index);

int tree_get_upgrade_cost(const Tree* tree, int index);
UpgradeType tree_get_upgrade_type(const Tree* tree, int index);
bool tree_is_upgrade_available(const Tree* tree, int index);
bool tree_is_upgrade_purchased(const Tree* tree, int index);

const char* upgrade_get_name(UpgradeType type);
const char* upgrade_get_description(UpgradeType type);
