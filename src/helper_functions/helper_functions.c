#include "../../include/helper_functions/helper_functions.h"

SDL_Texture* SDL_CreateTextureFromPNG(SDL_Renderer* renderer, const char* png_path)
{
    SDL_Surface* surface = SDL_LoadPNG(png_path);
    verify(!surface, "Could not create surface");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    verify(!texture, "Could not create texture");

    SDL_DestroySurface(surface);

    return texture;
}



SDL_FRect* SDL_CreateFRect(double x, double y, double width, double height)
{
    SDL_FRect* frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    frect->x = x;
    frect->y = y;
    frect->w = width;
    frect->h = height;
    return frect;
}
void SDL_DestroyFRect(SDL_FRect* frect)
{
    SDL_free(frect);
}



bool rect_intersects_rect(const SDL_FRect* frect1, const SDL_FRect* frect2)
{
    verify(frect1 == NULL || frect2 == NULL, "FRect does not exist");
    if (frect1->x > frect2->x + frect2->w) return false;
    if (frect1->x + frect2->w < frect2->x) return false;
    if (frect1->y > frect2->y + frect2->h) return false;
    if (frect1->y + frect2->h < frect2->y) return false;
    return true;
}
bool point_intersects_rect(double x, double y, const SDL_FRect* frect)
{
    verify(frect == NULL, "FRect does not exist");
    if (x < frect->x) return false;
    if (x > frect->x + frect->w) return false;
    if (y < frect->y) return false;
    if (y > frect->y + frect->h) return false;
    return true;
}



bool is_within(int value, int min, int max)
{
    if (value <= max || value >= min)
        return true;

    return false;
}
