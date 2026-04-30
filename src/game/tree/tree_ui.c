#include "include/game/tree/tree_ui.h"

struct TreeUI
{
    SDL_Renderer* renderer;

    TTF_Font* font;
    SDL_Color* red;
    SDL_Color* green;

    Tree* tree;
    Window* window;

    Board* board;
    BoardUI* board_ui;
};

TreeUI* tree_ui_create(
    SDL_Renderer* renderer, 
    Tree* tree,
    BoardUI* board_ui,
    float x, 
    float y, 
    float width, 
    float height
)
{
    TreeUI* ui = SDL_malloc(sizeof(TreeUI));
    verify(ui == NULL, "TreeUI could not be created: malloc");

    ui->renderer = renderer;

    ui->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    verify(ui->font == NULL, "TTF_Font could not be opened");
    ui->red = SDL_malloc(sizeof(SDL_Color));
    verify(ui->red == NULL, "SDL_Color could not be created: malloc");
    *ui->red = color_red();
    ui->green = SDL_malloc(sizeof(SDL_Color));
    verify(ui->green == NULL, "SDL_Color could not be created: malloc");
    *ui->green = color_green();

    ui->tree = tree;

    SDL_Texture* background_texture = SDL_CreateTextureFromPNG(
        renderer, 
        PATH_TEXTURE_WINDOW_TREE_BACKGROUND
    );
    ui->window = window_create(x, y, width, height, background_texture);
    window_load_textures(
        renderer, 
        ui->window, 
        tree_textures, 
        TREE_TEXTURES_COUNT
    );

    ui->board = board_ui_get_board(board_ui);
    ui->board_ui = board_ui;

    tree_ui_update(ui);

    return ui;
}

void tree_ui_destroy(TreeUI* ui)
{
    TTF_CloseFont(ui->font);
    SDL_free(ui->red);
    SDL_free(ui->green);
    window_destroy(ui->window);
    SDL_free(ui);
}

Window* tree_ui_get_window(TreeUI* ui)
{
    return ui->window;
}

static void tree_ui_set_nodes(TreeUI* ui)
{
    Tree* tree = ui->tree;
    Window* window = ui->window;
    SDL_Renderer* renderer = ui->renderer;
    
    SDL_Texture* button_idle = NULL;
    SDL_Texture* button_hovered = window_get_texture(ui->window, TEXTURE_TREE_BUTTON_HOVERED);
    SDL_Texture* button_pressed = window_get_texture(ui->window, TEXTURE_TREE_BUTTON_PRESSED);

    float size = window_get_height(board_ui_get_window(ui->board_ui)) / 5;

    Player* player = board_get_active_player(ui->board);
    SDL_Texture* texture_node;
    int index;
    Node* node;
    Button* button;
    Sprite* sprite;
    Textbox* textbox;
    SDL_Color* color;
    char* text;

    // button 1 - add pawn
    index = 0;
    button = button_create(
        button_idle,
        button_hovered,
        button_pressed
    );
    button_set_size(button, size, size);
    node = tree_get_node_at(tree, 0);
    button_set_on_click_fn(
        button, 
        MOUSE_LEFT, 
        node_purchase, 
        ui->board_ui, 
        node
    );
    window_add_button(window, button, 0, size * index);

    texture_node = window_get_texture(ui->window, TEXTURE_NODE_ADD_PAWN);
    sprite = sprite_create(texture_node);
    sprite_set_size(sprite, size, size);
    window_add_sprite(window, sprite, 0, size * index);

    SDL_asprintf(&text, "COST: %d", node_get_cost(node));
    if (node_can_purchase(node, player_get_points(player)))
        color = ui->green;
    else
        color = ui->red;
    textbox = textbox_create(
        renderer, 
        ui->font, 
        color,
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, size, (size * index) + size - textbox_get_height(textbox));

    // button 2 - expand board
    index = 1;
    button = button_create(
        button_idle,
        button_hovered,
        button_pressed
    );
    button_set_size(button, size, size);
    node = tree_get_node_at(tree, 1);
    button_set_on_click_fn(
        button, 
        MOUSE_LEFT, 
        node_purchase, 
        ui->board_ui, 
        node
    );
    window_add_button(window, button, 0, size * index);

    texture_node = window_get_texture(ui->window, TEXTURE_NODE_EXPAND_BOARD);
    sprite = sprite_create(texture_node);
    sprite_set_size(sprite, size, size);
    window_add_sprite(window, sprite, 0, size * index);

    SDL_asprintf(&text, "COST: %d", node_get_cost(node));
    if (node_can_purchase(node, player_get_points(player)))
        color = ui->green;
    else
        color = ui->red;
    textbox = textbox_create(
        renderer, 
        ui->font, 
        color,
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, size, (size * index) + size - textbox_get_height(textbox));

    // button 3 - add lance
    index = 2;
    button = button_create(
        button_idle,
        button_hovered,
        button_pressed
    );
    button_set_size(button, size, size);
    node = tree_get_node_at(tree, index);
    button_set_on_click_fn(
        button, 
        MOUSE_LEFT, 
        node_purchase, 
        ui->board_ui, 
        node
    );
    window_add_button(window, button, 0, size * index);

    texture_node = window_get_texture(ui->window, TEXTURE_NODE_ADD_LANCE);
    sprite = sprite_create(texture_node);
    sprite_set_size(sprite, size, size);
    window_add_sprite(window, sprite, 0, size * index);

    SDL_asprintf(&text, "COST: %d", node_get_cost(node));
    if (node_can_purchase(node, player_get_points(player)))
        color = ui->green;
    else
        color = ui->red;
    textbox = textbox_create(
        renderer, 
        ui->font, 
        color,
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, size, (size * index) + size - textbox_get_height(textbox));

    // button 4 - add lance
    index = 3;
    button = button_create(
        button_idle,
        button_hovered,
        button_pressed
    );
    button_set_size(button, size, size);
    node = tree_get_node_at(tree, index);
    button_set_on_click_fn(
        button, 
        MOUSE_LEFT, 
        node_purchase, 
        ui->board_ui, 
        node
    );
    window_add_button(window, button, 0, size * index);

    texture_node = window_get_texture(ui->window, TEXTURE_NODE_SACRIFICE);
    sprite = sprite_create(texture_node);
    sprite_set_size(sprite, size, size);
    window_add_sprite(window, sprite, 0, size * index);

    SDL_asprintf(&text, "COST: %d", node_get_cost(node));
    if (node_can_purchase(node, player_get_points(player)))
        color = ui->green;
    else
        color = ui->red;
    textbox = textbox_create(
        renderer, 
        ui->font, 
        color,
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, size, (size * index) + size - textbox_get_height(textbox));
}
void tree_ui_update(TreeUI* ui)
{
    Window* window = ui->window;

    window_destroy_content(window);

    tree_ui_set_nodes(ui);
}
