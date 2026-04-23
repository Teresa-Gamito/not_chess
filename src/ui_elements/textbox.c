#include "include/ui_elements/textbox.h"

struct Textbox
{
    TTF_Font* font;
    SDL_Color* color;

    char* text;

    int max_width;
    TextAlignment alignment;

    float x;
    float y;
    float width;
    float height;

    SDL_Texture* texture;
};

Textbox* textbox_create(
    SDL_Renderer* renderer,
    TTF_Font* font,
    SDL_Color* color,
    const char* text,
    const double max_width,
    TextAlignment alignment)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(font == NULL, "TTF_Font does not exist");
    verify(color == NULL, "SDL_Color does not exist");
    verify(alignment < 0 || alignment >= TEXT_ALIGNMENT_COUNT, "Invalid alignment");

    Textbox* textbox = (Textbox*)SDL_malloc(sizeof(Textbox));

    textbox->font = font;

    textbox->color = color;

    // if TTF_RenderText_Solid_Wrapped wrapLenght is 0 then the text wraps on a new character
    // setting max_width as a negative value makes this effect possible as well
    if (max_width < 0) textbox->max_width = 0;
    else textbox->max_width = max_width;

    textbox->alignment = alignment;

    textbox->x = 0;
    textbox->y = 0;
    textbox->width = 0;
    textbox->height = 0;

    textbox_set_text(renderer, textbox, text);

    return textbox;
}

void textbox_destroy(Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    if (textbox->text != NULL) SDL_free(textbox->text);
    if (textbox->texture != NULL) SDL_DestroyTexture(textbox->texture);

    SDL_free(textbox);
}


void textbox_render(SDL_Renderer* renderer, const Textbox* textbox)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(textbox == NULL, "Textbox does not exist");
    verify(textbox->texture == NULL, "Textbox does not have a texture");

    SDL_FRect position = 
        {
            textbox->x,
            textbox->y,
            textbox->width,
            textbox->height
        };

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

void textbox_set_position(Textbox* textbox, float x, float y)
{
    verify(textbox == NULL, "Textbox does not exist");

    textbox->x = x;
    textbox->y = y;
}
void textbox_set_size(Textbox* textbox, float width, float height)
{
    verify(textbox == NULL, "Textbox does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    textbox->width = width;
    textbox->height = height;
}
void textbox_set_max_width(Textbox* textbox, int max_width)
{
    verify(textbox == NULL, "Textbox does not exist");

    if (max_width < 0) 
    {
        textbox->max_width = 0;
        return;
    }
    textbox->max_width = max_width;
}
void textbox_set_alignment(Textbox* textbox, TextAlignment alignment)
{
    verify(textbox == NULL, "Textbox does not exist");
    verify(alignment < 0 || alignment >= TEXT_ALIGNMENT_COUNT, "Invalid alignment");

    textbox->alignment = alignment;
}
void textbox_set_color(Textbox* textbox, SDL_Color* color)
{
    verify(textbox == NULL, "Textbox does not exist");
    verify(color == NULL, "SDL_Color does not exist");

    textbox->color = color;
}
void textbox_set_font(Textbox* textbox, TTF_Font* font)
{
    verify(textbox == NULL, "Textbox does not exist");
    verify(font == NULL, "TTF_Font does not exist");

    textbox->font = font;
}
static void textbox_update_texture(SDL_Renderer* renderer, Textbox* textbox)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(textbox == NULL, "Textbox does not exist");

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
        &textbox->width,
        &textbox->height
    );
}
void textbox_set_text(SDL_Renderer* renderer, Textbox* textbox, const char* text)
{
    verify(renderer == NULL, "Renderer does not exist");
    verify(textbox == NULL, "Textbox does not exist");

    if (textbox->text != NULL) 
    {
        SDL_free(textbox->text);
    }
    textbox->text = SDL_strdup(text);
    textbox_update_texture(renderer, textbox);
}

float textbox_get_x(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->x;
}
float textbox_get_y(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->y;
}
float textbox_get_width(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->width;
}
float textbox_get_height(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->height;
}
int textbox_get_max_width(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->max_width;
}
TextAlignment textbox_get_alignment(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->alignment;
}
SDL_FRect textbox_get_frect(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    SDL_FRect frect =
        {
            textbox->x,
            textbox->y,
            textbox->width,
            textbox->height
        };
    return frect;
}
SDL_Color* textbox_get_color(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->color;
}
TTF_Font* textbox_get_font(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->font;
}
SDL_Texture* textbox_get_texture(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");

    return textbox->texture;
}



static void destroy(void* t)
{
    Textbox* textbox = (Textbox*)t;
    verify(textbox == NULL, "Textbox does not exist");

    textbox_destroy(textbox);
}
TypeOps ops =
    {
        destroy
        //is_equal,
    };

TypeOps* textbox_ops()
{
    return &ops;
}
