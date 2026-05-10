#include "include/game/properties/properties.h"

static void add_to_string(char** string, const char* add, bool is_new_line)
{
    char* fmt = is_new_line ? "%s%s\n" : "%s%s";
    SDL_asprintf(string, fmt, *string, add);
}

static const char* color_get_name(Color color)
{
    switch (color)
    {
        case WHITE:
            return "White";
        case BLACK:
            return "Black";
        default:
            return "Invalid color";
    }
}

const char* get_piece_properties(const Piece* piece)
{
    char* properties = "";

    Color color = piece_get_color(piece);
    add_to_string(&properties, "Color: ", false);
    add_to_string(&properties, color_get_name(color), true);

    PieceType type = piece_get_type(piece);
    add_to_string(&properties, "Type: ", false);
    add_to_string(&properties, piece_type_get_name(type), true);

    add_to_string(&properties, "", true);

    char* has_moved = piece_has_moved(piece) ? "yes" : "no";
    add_to_string(&properties, "Has moved: ", false);
    add_to_string(&properties, has_moved, true);

    // TODO: keep adding more properties

    return properties;
}


static const char* tile_type_get_name(TileType type)
{
    switch (type) 
    {
        case TILE_NONE:
            return "Regular";

        default:
            return "Invalid TileType";
    }
}
const char* get_tile_properties(const Tile* tile)
{
    char* properties = "";

    Color color = tile_get_color(tile);
    add_to_string(&properties, "Color: ", false);
    add_to_string(&properties, color_get_name(color), true);

    TileType type = tile_get_type(tile);
    add_to_string(&properties, "Type: ", false);
    add_to_string(&properties, tile_type_get_name(type), true);

    add_to_string(&properties, "", true);

    // TODO: keep adding more properties

    return properties;
}
const char* get_rules();
const char* get_log();
