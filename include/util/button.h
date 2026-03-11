#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"

typedef struct Button Button;

// create / destroy
Button *button_create(Position pos, Size size);
void button_destroy(Button *button);

// set
void button_set_position(Button *button, Position pos);
void button_set_size(Button *buttoon, Size size);
void button_set_is_hovered(Button *button, bool state);

// get
Position button_get_position(Button button);
Size button_get_size(Button button);
bool button_get_is_hovered(Button button);
