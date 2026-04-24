#include "include/game/gamestate.h"
#include "appstate.h"
#include "game/board/board.h"
#include "game/board/board_textures.h"
#include "game/board/player.h"
#include "game/tree/node.h"
#include "helper_functions/color.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/helper_functions.h"
#include "ui_elements/textbox.h"
#include "ui_elements/window.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>

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
    TTF_CloseFont(gamestate->font);
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

    gamestate->tree = tree_create();
    tree_set_default(gamestate->tree);
    gamestate->tree_ui = tree_ui_create(
        renderer,
        gamestate->tree,
        gamestate->board_ui,
        (float)g_app_window_height,
        0,
        100,
        (float)g_app_window_height
    );

    gamestate->player_info = window_create(
        (float)g_app_window_height + 100,
        0,
        500,
        (float)g_app_window_height,
        NULL
    );

    player_window_update(renderer, gamestate);
}

static void player_window_update(SDL_Renderer* renderer, GameState* gamestate)
{
    Window* window = gamestate->player_info;
    Board* board = gamestate->board;
    window_destroy_content(window);

    Textbox* textbox;

    char turn;
    turn = board_get_player_white(board) == board_get_active_player(board) ? '-' : ' ';

    Player* player1 = board_get_player_white(board);
    char* text_player_1 = NULL;
    SDL_asprintf(&text_player_1, "%cWHITE    CAPTURING POINTS: %d", turn, player_get_points(player1));
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        "",
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    textbox_set_text(renderer, textbox, text_player_1);
    SDL_free(text_player_1);
    window_add_textbox(window, textbox, 10, 10);

    turn = board_get_player_black(board) == board_get_active_player(board) ? '-' : ' ';
    Player* player2 = board_get_player_black(board);
    char* text_player_2 = NULL;
    SDL_asprintf(&text_player_2, "%cBLACK    CAPTURING POINTS: %d", turn, player_get_points(player2));
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        "",
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    textbox_set_text(renderer, textbox, text_player_2);
    SDL_free(text_player_2);
    window_add_textbox(window, textbox, 10, 40);
}
