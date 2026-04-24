#include "include/helper_functions/color.h"

SDL_Color color_white()
{
    SDL_Color color = {255, 255, 255, 255};
    return color;
}
SDL_Color color_black()
{
    SDL_Color color = {0, 0, 0, 255};
    return color;
}

