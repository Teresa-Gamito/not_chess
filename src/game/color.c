#include "include/game/color.h"

const char* color_get_name(Color color)
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

Color color_get_opposite(Color color)
{
    return color == WHITE ? BLACK : WHITE;
}

void verify_color(Color color)
{
    verify(color != WHITE && color != BLACK, "Invalid color");
}
