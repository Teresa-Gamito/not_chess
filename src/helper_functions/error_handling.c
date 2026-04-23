#include "include/helper_functions/error_handling.h"

void verify(bool statement, const char* error_msg)
{
    if (!statement) return;

    SDL_Log("%s", error_msg);
    SDL_Quit();
    exit(1);
}

