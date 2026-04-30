#include "include/ui_elements/textbox.h"
#include "helper_functions/error_handling.h"

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
    float anchor_x;
    float anchor_y;
    float scale;

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
    verify_renderer(renderer);

    Textbox* textbox = (Textbox*)SDL_malloc(sizeof(Textbox));
    verify(textbox == NULL, "Textbox could not be created: malloc");

    textbox->font = font;
    textbox->color = color;

    textbox_set_max_width(textbox, max_width);

    textbox->alignment = alignment;

    textbox->x = 0;
    textbox->y = 0;
    textbox->width = 0;
    textbox->height = 0;
    textbox->anchor_x = 0;
    textbox->anchor_y = 0;
    textbox->scale = 1;

    textbox_set_text(renderer, textbox, text);

    return textbox;
}

void textbox_destroy(Textbox* textbox)
{
    verify_textbox(textbox);

    SDL_free(textbox->text);
    SDL_DestroyTexture(textbox->texture);

    SDL_free(textbox);
}


void textbox_render(SDL_Renderer* renderer, const Textbox* textbox)
{
    verify_renderer(renderer);
    verify_textbox(textbox);

    SDL_FRect position = textbox_get_frect(textbox);
    switch (textbox->alignment)
    {
        case TEXT_CENTERED:
            position.x -= textbox->width / 2;
            break;
        case TEXT_RIGHT_ALIGNED:
            position.x -= textbox->width;
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
    verify_textbox(textbox);

    textbox->x = x;
    textbox->y = y;
}
void textbox_set_anchor(Textbox* textbox, float anchor_x, float anchor_y)
{
    verify_textbox(textbox);

    textbox->anchor_x = anchor_x;
    textbox->anchor_y = anchor_y;
}
void textbox_set_size(Textbox* textbox, float width, float height)
{
    verify_textbox(textbox);
    verify_size(width, height);

    textbox->width = width;
    textbox->height = height;
}
void textbox_set_scale(Textbox* textbox, float scale)
{
    verify_textbox(textbox);

    textbox->scale = scale;
}
void textbox_set_max_width(Textbox* textbox, int max_width)
{
    verify_textbox(textbox);

    // if TTF_RenderText_Solid_Wrapped wrapLenght is 0 then the text wraps on a new character
    // setting max_width as a negative value makes this effect possible as well
    if (max_width < 0) 
    {
        textbox->max_width = 0;
        return;
    }
    textbox->max_width = max_width;
}
void textbox_set_alignment(Textbox* textbox, TextAlignment alignment)
{
    verify_textbox(textbox);
    verify(alignment < 0 || alignment >= TEXT_ALIGNMENT_COUNT, "Invalid alignment");

    textbox->alignment = alignment;
}
void textbox_set_color(Textbox* textbox, SDL_Color* color)
{
    verify_textbox(textbox);
    verify(color == NULL, "SDL_Color does not exist");

    textbox->color = color;
}
void textbox_set_font(Textbox* textbox, TTF_Font* font)
{
    verify_textbox(textbox);
    verify(font == NULL, "TTF_Font does not exist");

    textbox->font = font;
}
static void textbox_update_texture(SDL_Renderer* renderer, Textbox* textbox)
{
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(
        textbox->font,
        textbox->text,
        0,
        *textbox->color,
        textbox->max_width
    );
    verify(surface == NULL, "Surface could not be created");

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
    verify_renderer(renderer);
    verify_textbox(textbox);

    textbox->text = SDL_strdup(text);
    textbox_update_texture(renderer, textbox);
}

float textbox_get_x(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->x;
}
float textbox_get_y(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->y;
}
float textbox_get_width(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->width;
}
float textbox_get_height(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->height;
}
int textbox_get_max_width(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->max_width;
}
TextAlignment textbox_get_alignment(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->alignment;
}
SDL_FRect textbox_get_frect(const Textbox* textbox)
{
    verify_textbox(textbox);

    SDL_FRect frect =
        {
            textbox->x * textbox->scale + textbox->anchor_x,
            textbox->y * textbox->scale + textbox->anchor_y,
            textbox->width * textbox->scale,
            textbox->height * textbox->scale,
        };
    return frect;
}
SDL_Color* textbox_get_color(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->color;
}
TTF_Font* textbox_get_font(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->font;
}
SDL_Texture* textbox_get_texture(const Textbox* textbox)
{
    verify_textbox(textbox);

    return textbox->texture;
}



static void destroy(void* textbox)
{
    verify_textbox(textbox);

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



void verify_textbox(const Textbox* textbox)
{
    verify(textbox == NULL, "Textbox does not exist");
}
