#include "include/helper_functions/position.h"

bool pos_equals(Pos pos1, Pos pos2)
{
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

Pos pos_mult(Pos pos, int x, int y)
{
    return (Pos){pos.x * x, pos.y * y};
}

Pos pos_mirror_x(Pos pos)
{
    return (Pos){-pos.x, pos.y};
}

Pos pos_mirror_y(Pos pos)
{
    return (Pos){pos.x, -pos.y};
}

Pos pos_add(Pos pos1, Pos pos2)
{
    return (Pos){pos1.x + pos2.x, pos1.y + pos2.y};
}
