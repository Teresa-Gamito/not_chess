#include "include/helper_functions/error_handling.h"

void verify(bool statement, const char* error_msg)
{
#ifndef NDEBUG
    if (!statement) return;

    SDL_Log("%s", error_msg);
    SDL_Quit();
    exit(1);
    #endif
}

void verify_size(int width, int height)
{
    verify(width < 0, "Invalid width");
    verify(height < 0, "Invalid height");
}
void verify_renderer(const SDL_Renderer* renderer)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
}
