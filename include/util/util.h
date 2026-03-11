typedef struct Position Position;
typedef struct Size Size;

// Position
// create / destroy
Position *position_create(double x, double y);
void position_destroy(Position *pos);

// set
void position_set(Position *pos, double x, double y);
Position *position_add(Position pos1, Position pos2);
double distance_between(Position pos1, Position pos2);

// Size
Size *size_create(double width, double height);
void size_destroy(Size *size);

void size_set(Size *size, double width, double height);
Size *size_scale(Size size, double sacling_factor);
