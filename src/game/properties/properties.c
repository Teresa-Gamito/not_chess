#include "include/game/properties/properties.h"

static void add_property(char** properties, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char* temp;
    SDL_asprintf(&temp, msg, args);
    va_end(args);
    SDL_asprintf(properties, "%s%s\n", *properties, temp);
    SDL_free(temp);
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
    add_property(&properties, "Color: %s", color_get_name(color));

    PieceType type = piece_get_type(piece);
    add_property(&properties, "Type: %s", piece_type_get_name(type));

    add_property(&properties, "");

    char* has_moved = piece_has_moved(piece) ? "yes" : "no";
    add_property(&properties, "Has moved: %s", has_moved);

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
    add_property(&properties, "Color: %s", color_get_name(color));

    TileType type = tile_get_type(tile);
    add_property(&properties, "Type: %s", tile_type_get_name(type));

    add_property(&properties, "");

    // TODO: keep adding more properties

    return properties;
}
