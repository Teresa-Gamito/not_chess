#include "../../include/util/util.h"
#include <cstdlib>

struct Position {
    double x;
    double y;
};

struct Size {
    double w;
    double h;
};

Position *position_create(double x, double y)
{
    Position *pos = malloc(sizeof(Position));
    if (!pos)
    {
        // throw error
        return NULL;
    }
}
Position *position_add(Position pos1, Position pos2);
double distance_between(Position pos1, Position pos2);

Size *size_create(double w, double h);
Size *size_scale(Size size, double sacling_factor);
