#include "../../include/ui_elements/textbox.h"

struct Textbox
{
    TTF_Font* font;
    SDL_Color* color;

    char* text;

    int max_width;
    TextAlignment alignment;

    SDL_FRect* frect;

    SDL_Texture* texture;
};



// ========== create ==========

Textbox* textbox_create(
    SDL_Renderer* renderer,
    TTF_Font* font,
    SDL_Color* color,
    const char* text,
    const double max_width,
    TextAlignment alignment,
    const double x,
    const double y)
{
    Textbox* textbox = (Textbox*)SDL_malloc(sizeof(Textbox));

    textbox->font = font;

    textbox->color = color;

    // if TTF_RenderText_Solid_Wrapped wrapLenght is 0 then the text wraps on a new character
    // setting max_width as a negative value makes this effect possible as well
    if (max_width < 0) textbox->max_width = 0;
    else textbox->max_width = max_width;

    textbox->alignment = alignment;

    textbox->frect = SDL_CreateFRect(x, y, 0, 0); 

    textbox_set_text(renderer, textbox, text);

    return textbox;
}



// ========== destroy ==========

void textbox_destroy(Textbox* textbox)
{
    if (textbox == NULL) 
    {
        SDL_Log("Textbox does not exist, could not destroy");
        return;
    }

    if (textbox->text != NULL) SDL_free(textbox->text);
    if (textbox->texture != NULL) SDL_DestroyTexture(textbox->texture);

    SDL_free(textbox->frect);
    SDL_free(textbox);
}



// ========== render ==========

void textbox_render(SDL_Renderer* renderer, Textbox* textbox)
{
    if (renderer == NULL)
    {
        SDL_Log("Renderer does not exist");
        return;
    }
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist, could not be rendered");
        return;
    }

    SDL_FRect position = *textbox->frect;

    switch (textbox->alignment)
    {
        case TEXT_CENTERED:
            position.x -= position.w / 2;
            break;
        case TEXT_RIGHT_ALIGNED:
            position.x -= position.w;
            break;
        case TEXT_LEFT_ALIGNED:
        default:
            break;
    }
    SDL_RenderTexture(
        renderer, 
        textbox->texture, 
        NULL, 
        &position
    );
}



// ========== set ==========

void textbox_set_position(Textbox* textbox, double x, double y)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->frect->x = x;
    textbox->frect->y = y;
}

void textbox_set_size(Textbox* textbox, double width, double height)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->frect->w = width;
    textbox->frect->h = height;
}

void textbox_set_max_width(Textbox* textbox, int max_width)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->max_width = max_width;
}

void textbox_set_alignment(Textbox* textbox, TextAlignment alignment)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->alignment = alignment;
}

void textbox_set_color(Textbox* textbox, SDL_Color* color)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->color = color;
}

void textbox_set_font(Textbox* textbox, TTF_Font* font)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    textbox->font = font;
}

static void textbox_update_texture(SDL_Renderer* renderer, Textbox* textbox)
{
    if (renderer == NULL)
    {
        SDL_Log("Renderer does not exist");
        return;
    }
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(
        textbox->font,
        textbox->text,
        0,
        *textbox->color,
        textbox->max_width
    );
    if (surface == NULL)
    {
        SDL_Log("Surface could not be created");
        return;
    }
    textbox->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(
        textbox->texture, 
        &textbox->frect->w, 
        &textbox->frect->h
    );
}
void textbox_set_text(SDL_Renderer* renderer, Textbox* textbox, const char* text)
{
    if (renderer == NULL)
    {
        SDL_Log("Renderer does not exist");
        return;
    }
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return;
    }

    if (textbox->text != NULL) 
    {
        SDL_free(textbox->text);
    }
    textbox->text = SDL_strdup(text);
    textbox_update_texture(renderer, textbox);
}



// ========== get ==========

float textbox_get_x(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->frect->x;
}

float textbox_get_y(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->frect->y;
}

float textbox_get_width(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->frect->w;
}

float textbox_get_height(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->frect->h;
}

int textbox_get_max_width(Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->max_width;
}

TextAlignment textbox_get_alignment(Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return 0;
    }
    return textbox->alignment;
}

SDL_FRect* textbox_get_frect(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return NULL;
    }
    return textbox->frect;
}

SDL_Color* textbox_get_color(Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return NULL;
    }
    return textbox->color;
}

TTF_Font* textbox_get_font(const Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return NULL;
    }
    return textbox->font;
}

SDL_Texture* textbox_get_texture(Textbox* textbox)
{
    if (textbox == NULL)
    {
        SDL_Log("Textbox does not exist");
        return NULL;
    }
    return textbox->texture;
}
