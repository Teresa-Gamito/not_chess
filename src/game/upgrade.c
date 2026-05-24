#include "include/game/upgrade.h"

static bool is_player_side(Board* board, Player* player, Pos pos);
static bool is_player_color(Player* player, Piece* piece);

bool upgrade_needs_select(UpgradeType type)
{
    switch (type)
    {
        case UPGRADE_EXPANSION:
        case UPGRADE_MORE_LAND:
        case UPGRADE_RULING:
        case UPGRADE_GAMBLING:
        case UPGRADE_HELLO:
        case UPGRADE_FRIENDLY_FIRE:
        case UPGRADE_TACTICAL_ADVANTAGE:
        case UPGRADE_RUN:
            return false;

        default:
            return true;
    }
}

bool upgrade_pos_is_valid(Game* game, UpgradeType type, Pos pos)
{
    Board* board = game_get_board(game);
    Player* player = game_get_active_player(game);
    switch (type)
    {
        case UPGRADE_PEASANT:
        case UPGRADE_CHECKERS:
        case UPGRADE_SHOGI:
            if (!is_player_side(board, player, pos)) return false;
            if (board_has_piece_at(board, pos)) return false;
            return true;

        case UPGRADE_DISGUISE:
            if (!board_has_piece_at(board, pos)) return false;
            if (!is_player_color(player, board_get_piece_at(board,  pos))) return false;
            if (piece_get_type(board_get_piece_at(board, pos)) != ROOK) return false;
            return true;

        case UPGRADE_REVERSE_DISGUISE:
            if (!board_has_piece_at(board, pos)) return false;
            if (!is_player_color(player, board_get_piece_at(board,  pos))) return false;
            if (piece_get_type(board_get_piece_at(board, pos)) != BISHOP) return false;
            return true;

        case UPGRADE_PROMOTION:
            if (!board_has_piece_at(board, pos)) return false;
            if (!is_player_color(player, board_get_piece_at(board,  pos))) return false;
            if (piece_get_type(board_get_piece_at(board, pos)) != PAWN) return false;
            return true;

        case UPGRADE_PRECIOUS:
            if (!board_has_piece_at(board, pos)) return false;
            if (is_player_color(player, board_get_piece_at(board,  pos))) return false;
            if (piece_get_type(board_get_piece_at(board, pos)) == QUEEN) return false;
            if (piece_get_type(board_get_piece_at(board, pos)) == KING) return false;
            return true;

        case UPGRADE_RISE:
        case UPGRADE_SWAMP:
            if (board_has_piece_at(board, pos)) return false;
            return true;

        case UPGRADE_DEADTOUCH:
        case UPGRADE_TRAMPLE:
            if (!board_has_piece_at(board, pos)) return false;
            if (!is_player_color(player, board_get_piece_at(board,  pos))) return false;
            return true;

        default:
            return true;
    }
    return true;
}

void upgrade(Game* game, UpgradeType type, Pos pos)
{
    Player* player = game_get_active_player(game);
    Color color = player_get_color(player);
    Board* board = game_get_board(game);
    switch (type) 
    {
        case UPGRADE_PEASANT:
            board_add_piece_at(board, piece_create(PAWN, color), pos);
            break;

        case UPGRADE_SHOGI:
            board_add_piece_at(board, piece_create(LANCE, color), pos);
            break;

        case UPGRADE_EXPANSION:
            board_expand(board);
            break;

        case UPGRADE_MORE_LAND:
            board_expand(board);
            board_expand(board);
            break;

        case UPGRADE_DISGUISE:
            piece_set_type(board_get_piece_at(board, pos), BISHOP);
            break;

        case UPGRADE_REVERSE_DISGUISE:
            piece_set_type(board_get_piece_at(board, pos), ROOK);
            break;

        case UPGRADE_PROMOTION:
            piece_set_type(board_get_piece_at(board, pos), QUEEN);
            break;

        case UPGRADE_PRECIOUS:
            piece_set_color(board_get_piece_at(board, pos), color);
            break;

        case UPGRADE_RISE:
            tile_set_type(board_get_tile_at(board, pos), TILE_WALL);
            break;

        case UPGRADE_SWAMP:
            tile_set_type(board_get_tile_at(board, pos), TILE_POISON);
            break;

        case UPGRADE_GAMBLING:
            board_add_rule(board, RULE_PAWN_PROMOTION_CHANCE);
            break;

        default:
            break;
    }
}

static bool is_player_side(Board* board, Player* player, Pos pos)
{
    if (pos.row >= board_get_row_num(board) / 2)
    {
        if (player_get_color(player) == WHITE)
        {
            return true;
        }
    }
    else
    {
        if (player_get_color(player) == BLACK)
        {
            return true;
        }
    }
    return false;
}

static bool is_player_color(Player* player, Piece* piece)
{
    return player_get_color(player) == piece_get_color(piece);
}
