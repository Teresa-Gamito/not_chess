#pragma once

#include <stdbool.h>

#define col x
#define row y

typedef struct Pos
{
    int x;
    int y;
} Pos;

bool pos_equals(Pos pos1, Pos pos2);
Pos pos_mult(Pos pos, int x, int y);
Pos pos_mirror_x(Pos pos);
Pos pos_mirror_y(Pos pos);
Pos pos_add(Pos pos1, Pos pos2);
