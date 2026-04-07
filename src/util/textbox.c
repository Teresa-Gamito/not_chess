#include "../../include/util/textbox.h"

// TODO:
// replace frect and texture with Object*

struct Textbox
{
    // allocated text
    char* text;
    int max_width;
    // TTF_Font already exists
    TTF_Font* font;
    // SDL_Color already exists
    SDL_Color* color;
    // Texture is created
    SDL_Texture* texture;
    // SDL_Frect is created
    // contains x, y, width, height
    SDL_FRect* frect;
};


// ========== create ==========
Textbox* textbox_create(
    SDL_Renderer* renderer,
    const double x,
    const double y,
    const double max_width,
    const char* text,
    TTF_Font* font,
    SDL_Color* color)
{
    Textbox* textbox = (Textbox*)SDL_malloc(sizeof(Textbox));
    // set font
    textbox->font = font;
    // set color
    textbox->color = color;
    // set the text and creates the respective texture
    textbox->texture = NULL;
    textbox->text = SDL_strdup(text);
    textbox->max_width = max_width;
    textbox_update_texture(renderer, textbox);
    // set frect - position and size
    textbox->frect = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
    textbox->frect->x = x;
    textbox->frect->y = y;
    SDL_GetTextureSize(textbox->texture, &textbox->frect->w, &textbox->frect->h);
    return textbox;
}

// ========== destroy ==========
void textbox_destroy(Textbox* textbox)
{
    if (textbox->texture) SDL_DestroyTexture(textbox->texture);
    if (textbox->frect) SDL_free(textbox->frect);
    if (textbox->text) SDL_free(textbox->text);
    SDL_free(textbox);
}

// ========== render ==========
void text_render(SDL_Renderer* renderer, Textbox* textbox)
{
    SDL_RenderTexture(
        renderer, 
        textbox->texture, 
        NULL, 
        textbox->frect
    );
}


// ========== set ==========
void textbox_set_position(Textbox* textbox, double x, double y)
{
    textbox->frect->x = x;
    textbox->frect->y = y;
}
void textbox_set_size(Textbox* textbox, double width, double height)
{
    textbox->frect->w = width;
    textbox->frect->h = height;
}
void textbox_set_color(Textbox* textbox, SDL_Color* color)
{
    textbox->color = color;
}
void textbox_set_font(Textbox* textbox, TTF_Font* font)
{
    textbox->font = font;
}
void textbox_set_text(SDL_Renderer* renderer, Textbox* textbox, const char* text)
{
    if (textbox->text) SDL_free(textbox->text);
    textbox->text = SDL_strdup(text);
    textbox_update_texture(renderer, textbox);
}
void textbox_update_texture(SDL_Renderer* renderer, Textbox* textbox)
{
    if (textbox->texture) SDL_DestroyTexture(textbox->texture);
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(
        textbox->font,
        textbox->text,
        0,
       *textbox->color,
        textbox->max_width
    );
    textbox->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}


// ========== get ==========
float textbox_get_x(const Textbox* textbox)
{
    return textbox->frect->x;
}
float textbox_get_y(const Textbox* textbox)
{
    return textbox->frect->y;
}
float textbox_get_width(const Textbox* textbox)
{
    return textbox->frect->w;
}
float textbox_get_height(const Textbox* textbox)
{
    return textbox->frect->h;
}
SDL_FRect* textbox_get_frect(const Textbox* textbox)
{
    return textbox->frect;
}
SDL_Color* textbox_get_color(Textbox* textbox)
{
    return textbox->color;
}
TTF_Font* textbox_get_font(const Textbox* textbox)
{
    return textbox->font;
}
SDL_Texture* textbox_get_texture(Textbox* textbox)
{
    return textbox->texture;
}
