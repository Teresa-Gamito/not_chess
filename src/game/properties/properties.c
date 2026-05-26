#include "include/game/properties/properties.h"
#include "game/upgrade_tree/tree.h"

static void add_property(char** properties, const char* msg, ...)
{
    char* temp;
    va_list args;
    va_start(args, msg);
    SDL_vasprintf(&temp, msg, args);
    va_end(args);
    SDL_asprintf(properties, "%s%s\n", *properties, temp);
    SDL_free(temp);
}

const char* get_piece_properties(const Piece* piece)
{
    char* properties = "";

    Color color = piece_get_color(piece);
    add_property(&properties, "Color: %s", color_get_name(color));

    PieceType type = piece_get_type(piece);
    add_property(&properties, "Type: %s", piece_type_get_name(type));

    add_property(&properties, "");

    // char* has_moved = piece_has_moved(piece) ? "yes" : "no";
    // add_property(&properties, "Has moved: %s", has_moved);
    if (piece_has_moved(piece))
    {
        add_property(&properties, "Has moved");
    }

    // TODO: keep adding more properties

    return properties;
}

const char* get_tile_properties(const Tile* tile)
{
    char* properties = "";

    Color color = tile_get_color(tile);
    add_property(&properties, "Color: %s", color_get_name(color));

    TileType type = tile_get_type(tile);
    add_property(&properties, "Type: %s", tile_type_get_name(type));

    add_property(&properties, "");

    // TODO: keep adding more properties

    return properties;
}

const char* get_player_properties(const Player* player)
{
    char* properties = "";

    Color color = player_get_color(player);
    int points = player_get_points(player);
    if (points > 99)
    {
        add_property(&properties, "%s: %s points", color_get_name(color), "-");
    }
    else
    {
        add_property(&properties, "%s: %d points", color_get_name(color), points);
    }

    return properties;
}

const char* get_upgrade_properties(const Tree* tree, int index)
{
    UpgradeType type = tree_get_upgrade_type(tree, index);
    int cost = tree_get_upgrade_cost(tree, index);
    bool is_available = tree_is_upgrade_available(tree, index);
    bool is_purchased = tree_is_upgrade_purchased(tree, index);

    char* properties = "";

    add_property(&properties, "%s", upgrade_get_name(type));

    add_property(&properties, "");

    add_property(&properties, "%s", upgrade_get_description(type));

    add_property(&properties, "");

    add_property(&properties, "Cost: %d", cost);

    char* state;
    if (is_purchased) state = "Purchased";
    else if (is_available) state = "Available";
    else state = "Locked";
    add_property(&properties, "State: %s", state);

    return properties;
}
