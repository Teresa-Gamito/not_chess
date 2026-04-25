#include "include/game/gamestate.h"
#include "appstate.h"
#include "game/board/board_ui.h"
#include "game/tree/tree_ui.h"
#include "ui_elements/window.h"

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
        window_get_x(tree_window) + window_get_width(tree_window),
        (float)g_app_window_height,
        NULL
    );
    Window* player_info_window = gamestate->player_info;

    player_window_update(renderer, gamestate);
}

static void player_window_update(SDL_Renderer* renderer, GameState* gamestate)
{
    Window* window = gamestate->player_info;
    Board* board = gamestate->board;
    window_destroy_content(window);

    Textbox* textbox;
    char* turn;

    // player white
    turn = board_get_player_white(board) == board_get_active_player(board) ? "--->  " : "        ";
    Player* player1 = board_get_player_white(board);
    char* text_player_1 = NULL;
    SDL_asprintf(
        &text_player_1, 
        "%sWHITE  |  CAPTURING POINTS: %d", 
        turn, 
        player_get_points(player1)
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        "",
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    textbox_set_text(renderer, textbox, text_player_1);
    window_add_textbox(window, textbox, 10, 10);

    // player black
    turn = board_get_player_black(board) == board_get_active_player(board) ? "--->  " : "        ";
    Player* player2 = board_get_player_black(board);
    char* text_player_2 = NULL;
    SDL_asprintf(
        &text_player_2, 
        "%sBLACK  |  CAPTURING POINTS: %d", 
        turn, 
        player_get_points(player2)
    );
    textbox = textbox_create(
        renderer, 
        gamestate->font, 
        gamestate->text_color, 
        "",
        window_get_width(window), 
        TEXT_LEFT_ALIGNED
    );
    textbox_set_text(renderer, textbox, text_player_2);
    window_add_textbox(window, textbox, 10, 70);
}
