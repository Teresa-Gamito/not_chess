#pragma once

#include "helper_functions/error_handling.h"

typedef enum Color
{
    WHITE,
    BLACK
} Color;

const char* color_get_name(Color color);
Color color_get_opposite(Color color);

void verify_color(Color color);
