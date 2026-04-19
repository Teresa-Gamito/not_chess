#include "../../include/game/gamestate.h"

typedef enum PlayerColor
{
    PLAYER_WHITE,
    PLAYER_BLACK
} PlayerColor;

typedef struct Player 
{
    PlayerColor color;
    int capturing_points;
} Player;

struct GameState 
{
    Board* board;
    Window* shop;

    Player* player1;
    Player* player2;

    Player* active_player;
};



static void board_set_default_layout(Board* board);
static void shop_set_default(SDL_Renderer* renderer, Window* window);


// ========== create ==========
GameState* gamestate_create()
{
    GameState* game = SDL_malloc(sizeof(GameState));
    verify(game == NULL, "GameState could not be created: malloc");

    game->board = NULL;

    game->shop = NULL;

    game->player1 = SDL_malloc(sizeof(Player));
    game->player1->capturing_points = 0;
    game->player1->color = PLAYER_WHITE;

    game->player2 = SDL_malloc(sizeof(Player));
    game->player2->capturing_points = 0;
    game->player2->color = PLAYER_BLACK;

    game->active_player = game->player1;

    return game;
}
void gamestate_set_default(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    game->board = board_create(
        renderer,
        BOARD_STARTING_COL_NUM,
        BOARD_STARTING_ROW_NUM,
        50,
        30,
        600,
        600
    );
    board_set_default_layout(game->board);

    game->shop = window_create(
        700,
        30,
        100,
        500,
        NULL
    );
    shop_set_default(renderer, game->shop);
}
static void board_set_default_layout(Board* board)
{
    verify(board == NULL, "Board does not exist");

    board_add_piece_at(board, piece_create(KING, PIECE_WHITE), 4, 0);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_WHITE), 3, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 2, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 5, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 1, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 6, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 0, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 7, 0);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 0, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 1, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 2, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 3, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 4, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 5, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 6, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 7, 1);

    board_add_piece_at(board, piece_create(KING, PIECE_BLACK), 4, 7);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_BLACK), 3, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 2, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 1, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 0, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 7, 6);
}
static void upgrade_add_pawn(GameState* game)
{
    verify(game == NULL, "GameState does not exist");

    Board* board = game->board;
    Player* player = game->active_player;

    board_task_add(board, ADD_PAWN);

    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {

        }
    }



    // TODO:
    // - verify which player got the upgrade
    // - verify if player can add to any location
    // - show possible locations to add pawn
    // - add pawn to location
}

static void shop_set_default(SDL_Renderer* renderer, Window* window)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(window == NULL, "Window does not exist");

    window_load_textures(renderer, window, shop_textures, SHOP_TEXTURE_COUNT);
    SDL_Texture* window_background = window_get_texture(window, TEXTURE_WINDOW_BACKGROUND);
    window_update_background_texture(window, window_background);

    Button* button;
    SDL_Texture* button_texture = window_get_texture(window, TEXTURE_BUTTON);

    button = button_create(
        5, 
        5, 
        button_texture,
        window_background,
        window_background
    );
    button_set_size(button, 390, 90);
    window_add_button(window, button);

    button = button_create(
        5, 
        105, 
        button_texture,
        window_background,
        window_background
    );
    button_set_size(button, 390, 90);
    window_add_button(window, button);

    button = button_create(
        5, 
        205, 
        button_texture,
        window_background,
        window_background
    );
    button_set_size(button, 390, 90);
    window_add_button(window, button);

    button = button_create(
        5, 
        305, 
        button_texture,
        window_background,
        window_background
    );
    button_set_size(button, 390, 90);
    window_add_button(window, button);

    button = button_create(
        5, 
        405, 
        button_texture,
        window_background,
        window_background
    );
    button_set_size(button, 390, 90);
    window_add_button(window, button);
}



// ========== destroy ==========
void gamestate_destroy(GameState* game)
{
    verify(game == NULL, "GameState does not exist");

    board_destroy(game->board);
    SDL_free(game);
}



// ========== render ==========
void game_render(SDL_Renderer* renderer, GameState* game)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(game == NULL, "GameState does not exist");

    Board* board = gamestate_get_board(game);

    window_render(renderer, board_get_window(board));
    window_render(renderer, game->shop);
}



// ========== update ==========
void gamestate_update(InputState* input, GameState* game)
{
    board_update(input, gamestate_get_board(game));
    window_update(input, game->shop);
}



// ========== get ==========
Board* gamestate_get_board(GameState* game)
{
    verify(game == NULL, "GameState does not exist");
    return game->board;
}
