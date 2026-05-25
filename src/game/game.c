#include "include/game/game.h"
#include "game/board/board.h"
#include "game/board/board_elements/piece.h"
#include "game/color.h"
#include "game/log/log.h"
#include "game/player/player.h"
#include "game/upgrade.h"

struct Game
{
    Player* white;
    Player* black;
    Player* active_player;

    Board* board;
    Tree* tree;

    Vector* rules;
    GameLog* log;

    Queue* upgrades;
};

Game* game_create()
{
    Game* game = SDL_malloc(sizeof(Game));
    verify(game == NULL, "Game could not be created: malloc");

    game->board = NULL;
    game->tree = NULL;

    game->white = NULL;
    game->black = NULL;
    game->active_player = NULL;

    game->rules = rulelist_create();
    game->log = gamelog_create();

    game->upgrades = queue_create();

    return game;
}

void game_destroy(Game* game)
{
    verify_game(game);

    board_destroy(game->board);
    tree_destroy(game->tree);

    player_destroy(game->white);
    player_destroy(game->black);

    SDL_free(game);
}

void game_start(Game* game)
{
    verify_game(game);

    game->board = board_create(GAME_STARTING_COL_NUM, GAME_STARTING_ROW_NUM);
    board_set_default(game->board);

    game->tree = tree_create();
    tree_set_default(game->tree);

    game->white = player_create(WHITE, PLAYER_STARTING_POINTS);
    game->black = player_create(BLACK, PLAYER_STARTING_POINTS);
    game->active_player = game->white;
}

bool game_try_upgrade(Game* game, Pos pos)
{
    if (queue_get_size(game->upgrades) <= 0)
    {
        return false;
    }

    UpgradeType* type = (UpgradeType*)queue_peek(game->upgrades);
    if (!upgrade_pos_is_valid(game, *type, pos))
    {
        return false;
    }

    upgrade(game, *type, pos);
    SDL_free(queue_pop(game->upgrades));
    return true;
}

bool game_try_capture(Game* game, Pos src, Pos dst)
{
    Board* board = game->board;

    if (!board_has_piece_at(board, src)) return false;
    if (!board_has_piece_at(board, dst)) return false;
    if (!board_can_piece_capture(board, src, dst)) return false;

    Piece* piece = board_get_piece_at(board, src);
    PieceType type = piece_get_type(board_get_piece_at(board, dst));
    Color player_color = player_get_color(game->active_player);
    Color piece_color = piece_get_color(piece);
    if (player_color != piece_color) return false;

    int points = board_piece_move_to(board, src, dst);
    player_add_points(game->active_player, points);

    gamelog_add(
        game->log, 
        "%s moved %s from col:%d row:%d to col:%d row:%d and captured %s for %d points",
        color_get_name(player_color),
        piece_type_get_name(piece_get_type(piece)),
        src.col,
        src.row,
        dst.col,
        dst.row,
        piece_type_get_name(type),
        points
    );

    return true;
}

bool game_try_move(Game* game, Pos src, Pos dst)
{
    Board* board = game->board;

    if (!board_has_piece_at(board, src)) return false;
    if (!board_can_piece_move_to(board, src, dst)) return false;

    Piece* piece = board_get_piece_at(board, src);
    Color player_color = player_get_color(game->active_player);
    Color piece_color = piece_get_color(piece);
    if (player_color != piece_color) return false;

    board_piece_move_to(board, src, dst);

    gamelog_add(
        game->log, 
        "%s moved %s from col:%d row:%d to col:%d row:%d",
        color_get_name(player_color),
        piece_type_get_name(piece_get_type(piece)),
        src.col,
        src.row,
        dst.col,
        dst.row
    );

    return true;
}

void game_advance_turn(Game* game)
{
    Player* player = game->active_player == game->white ? game->black : game->white;
    game->active_player = player;
}

void game_purchase_upgrade(Game* game, int index)
{
    Tree* tree = game->tree;
    int cost = tree_get_upgrade_cost(tree, index);
    if (player_get_points(game->active_player) < cost)
    {
        return;
    }
    if (!tree_is_upgrade_available(tree, index))
    {
        return;
    }

    player_add_points(game->active_player, -cost);

    UpgradeType type = tree_upgrade_purchase(tree, index);

    UpgradeType* upgrade = SDL_malloc(sizeof(UpgradeType));
    *upgrade = type;
    queue_push(game->upgrades, upgrade);
    if (!upgrade_needs_select(type))
    {
        game_try_upgrade(game, (Pos){0, 0});
    }
}

Board* game_get_board(const Game* game)
{
    return game->board;
}

Tree* game_get_tree(const Game* game)
{
    return game->tree;
}

RuleList* game_get_rules(const Game* game)
{
    return game->rules;
}

GameLog* game_get_log(const Game* game)
{
    return game->log;
}

Queue* game_get_upgrade_queue(const Game* game)
{
    return game->upgrades;
}

Player* game_get_active_player(const Game* game)
{
    return game->active_player;
}

Player* game_get_opponent(const Game* game)
{
    return game->active_player == game->white ? game->black : game->white;
}

void verify_game(const Game* game)
{
    verify(game == NULL, "game does not exist");
}
