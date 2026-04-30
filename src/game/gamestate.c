#include "include/game/gamestate.h"
#include "appstate.h"
#include "game/board/board_ui.h"
#include "game/tree/tree_ui.h"
#include "helper_functions/global_variables.h"
#include "ui_elements/textbox.h"
#include "ui_elements/window.h"
#include <SDL3/SDL_stdinc.h>

static void player_window_update(SDL_Renderer* renderer, GameState* gamestate);

struct GameState
{
    TTF_Font* font;
    SDL_Color* text_color;

    Board* board;
    BoardUI* board_ui;

    Tree* tree;
    TreeUI* tree_ui;

    Window* player_info;
};

GameState* gamestate_create()
{
    GameState* gamestate = SDL_malloc(sizeof(GameState));
    verify(gamestate == NULL, "GameState could not be created: malloc");

    gamestate->font = NULL;
    gamestate->text_color = NULL;

    gamestate->board = NULL;
    gamestate->board_ui = NULL;

    gamestate->tree = NULL;
    gamestate->tree_ui = NULL;

    gamestate->player_info = NULL;

    return gamestate;
}

void gamestate_destroy(GameState* gamestate)
{
    verify(gamestate == NULL, "GameState does not exist");

    board_destroy(gamestate->board);
    board_ui_destroy(gamestate->board_ui);
    if (gamestate->font != NULL) 
    {
        TTF_CloseFont(gamestate->font);
    }
    SDL_free(gamestate);

}

void game_render(SDL_Renderer* renderer, GameState* gamestate)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    player_window_update(renderer, gamestate);
    window_render(renderer, board_ui_get_window(gamestate->board_ui));
    window_render(renderer, tree_ui_get_window(gamestate->tree_ui));
    window_render(renderer, gamestate->player_info);
}

void game_update(InputState* input, GameState* gamestate)
{
    verify(input == NULL, "InputState does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    window_update(input, board_ui_get_window(gamestate->board_ui));
    window_update(input, tree_ui_get_window(gamestate->tree_ui));
    window_update(input, gamestate->player_info);
}

void game_start(SDL_Renderer* renderer, GameState* gamestate)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(gamestate == NULL, "GameState does not exist");

    gamestate->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    verify(gamestate->font == NULL, "Font could not be created");
    gamestate->text_color = SDL_malloc(sizeof(SDL_Color));
    verify(gamestate->text_color == NULL, "Color could not be created: malloc");
    *gamestate->text_color = color_white();

    gamestate->board = board_create(8, 8);
    board_set_default(gamestate->board);

    gamestate->board_ui = board_ui_create(
        renderer,
        gamestate->board,
        0,
        0,
        (float)g_app_window_height,
        (float)g_app_window_height
    );
    Window* board_window = board_ui_get_window(gamestate->board_ui);
    (void)board_window;

    gamestate->tree = tree_create();
    tree_set_default(gamestate->tree);
    gamestate->tree_ui = tree_ui_create(
        renderer,
        gamestate->tree,
        gamestate->board_ui,
        window_get_width(board_ui_get_window(gamestate->board_ui)),
        0,
        (float)g_app_window_height / 5,
        (float)g_app_window_height
    );
    Window* tree_window = tree_ui_get_window(gamestate->tree_ui);

    gamestate->player_info = window_create(
        window_get_x(tree_window) + window_get_width(tree_window),
        0,
        g_app_window_width - window_get_x(tree_window) + window_get_width(tree_window),
        (float)g_app_window_height,
        NULL,
        0
    );
    Window* player_info_window = gamestate->player_info;
    (void)player_info_window;
    player_window_update(renderer, gamestate);
}

static void player_window_update(SDL_Renderer* renderer, GameState* gamestate)
{
    Window* window = gamestate->player_info;
    Board* board = gamestate->board;
    window_destroy_content(window);

    Textbox* textbox;
    char* turn;
    Player* player;
    char* text;

    // player white
    player = board_get_player_white(board);
    turn = player == board_get_active_player(board) ? "--->  " : "        ";
    SDL_asprintf(
        &text, 
        "%sWHITE  |  CAPTURING POINTS: %d", 
        turn, 
        player_get_points(player)
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        text,
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, 10, 10);

    // player black
    player = board_get_player_black(board);
    turn = player == board_get_active_player(board) ? "--->  " : "        ";
    SDL_asprintf(
        &text, 
        "%sBLACK  |  CAPTURING POINTS: %d", 
        turn, 
        player_get_points(player)
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        text,
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(window, textbox, 10, 70);

    SDL_asprintf(
        &text, 
        "Infinite points: %d", 
        infinite_points
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(
        window, 
        textbox, 
        g_app_window_width - window_get_x(window),
        window_get_height(window) - textbox_get_height(textbox)
    );
    SDL_asprintf(
        &text, 
        "Can buy more upgrades: %d", 
        can_purchace_multiple_times
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        text,
        window_get_width(window), 
        TEXT_RIGHT_ALIGNED
    );
    SDL_free(text);
    window_add_textbox(
        window, 
        textbox, 
        g_app_window_width - window_get_x(window),
        window_get_height(window) - textbox_get_height(textbox) - 40
    );
}
