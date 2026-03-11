#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"

typedef enum WindowContent {
    BOARD,
    TREE,
    PROPERTIES,
    RULES,
    HISTORY,
    OPTIONS,
    PLAYER_1,
    PLAYER_2,
} WindowContent;

typedef struct Window Window;

// create / destroy
Window *window_create(Position *pos, Size *size, WindowContent content);
void window_destroy(Window *window);

// set
void window_set_posistion(Window *window, Position *pos);
void window_set_size(Window *window, Size *size);
void window_set_content(Window *window, WindowContent content);

// get
Size *window_get_size(Window *window);
Position *window_get_position(Window *window);

WindowContent window_get_content(Window *window);

void window_draw(Window window);

// testing
void window_draw_test(Window window);
