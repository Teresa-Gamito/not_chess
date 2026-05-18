#include "include/game/board/board.h"

static const char board_default_layout[] = "RNBQKBNRPPPPPPPP00000000000000000000000000000000pppppppprnbqkbnr";

static int translate_position(const Board* board, int col, int row);
static bool board_piece_has_clear_path(const Board* board, int src_col, int src_row, int dst_col, int dst_row);

struct Board
{
    int col_num;
    int row_num;

    Vector* tiles;
    Vector* pieces;

    Player* player1;
    Player* player2;
    Player** active_player;

    GameLog* log;

    bool has_game_ended;
};

Board* board_create(int col_num, int row_num)
{
    Board* board = SDL_malloc(sizeof(Board));
    verify(board == NULL, "Board could not be created: malloc");

    board->col_num = col_num;
    board->row_num = row_num;

    board->tiles = vector_create(tile_ops());
    board->pieces = vector_create(piece_ops());

    Tile* tile;
    for (int col = 0; col < col_num; col++)
    {
        for (int row = 0; row < row_num; row++)
        {
            Color color = tile_get_default_color(col, row);
            tile = tile_create(TILE_NONE, color);
            vector_add(board->tiles, tile);
            vector_add(board->pieces, NULL);
        }
    }

    board->player1 = player_create(WHITE, 0);
    board->player2 = player_create(BLACK, 0);
    board->active_player = &board->player1;

    board->log = gamelog_create();

    board->has_game_ended = false;

    return board;
}

void board_destroy(Board* board)
{
    verify(board == NULL, "Board does not exist");

    vector_destroy(board->tiles);
    vector_destroy(board->pieces);

    player_destroy(board->player1);
    player_destroy(board->player2);

    gamelog_destroy(board->log);

    SDL_free(board);
}

void board_set_default(Board* board)
{
    verify_board(board);

    PieceType type;
    Color color;
    for (int i = 0; i < board_get_col_num(board) * board_get_row_num(board); i++)
    {
        switch (board_default_layout[i])
        {
            case 'P':
            case 'p':
                type = PAWN;
                break;
            case 'R':
            case 'r':
                type = ROOK;
                break;
            case 'N':
            case 'n':
                type = KNIGHT;
                break;
            case 'B':
            case 'b':
                type = BISHOP;
                break;
            case 'Q':
            case 'q':
                type = QUEEN;
                break;
            case 'K':
            case 'k':
                type = KING;
                break;
            case 'L':
            case 'l':
                type = LANCE;
                break;
            default:
                continue;
        }
        if (board_default_layout[i] > 'a')
        {
            color = WHITE;
        }
        else color = BLACK;

        Piece* piece = piece_create(type, color);
        board_add_piece_at(board, piece, i % board_get_row_num(board), i / board_get_row_num(board));
    }
}

bool board_game_ended(const Board* board)
{
    verify_board(board);
    return board->has_game_ended;
}

bool board_can_add_piece_at(const Board* board, int col, int row)
{
    verify_board(board);
    verify_board_pos(board, col, row);

    if (board_has_piece_at(board, col, row))
    {
        return false;
    }
    return true;
}

void board_add_piece_at(Board* board, Piece* piece, int col, int row)
{
    verify_board(board);
    verify_piece(piece);
    verify_board_pos(board, col, row);
    verify(!board_can_add_piece_at(board, col, row), "Could not add piece, position already occupied");

    int pos = translate_position(board, col, row);
    vector_set_at(board->pieces, piece, pos);
}

void board_piece_remove(Board* board, int col, int row)
{
    verify_board(board);
    verify_board_pos(board, col, row);
    verify(!board_has_piece_at(board, col, row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, col, row);
    piece_destroy(piece);
    int pos = translate_position(board, col, row);
    vector_set_at(board->pieces, NULL, pos);
}

bool board_has_piece_at(const Board* board, int col, int row)
{
    verify_board(board);
    verify_board_pos(board, col, row);

    return board_get_piece_at(board, col, row) != NULL;
}

Piece* board_get_piece_at(const Board* board, int col, int row)
{
    verify_board(board);
    verify_board_pos(board, col, row);

    int pos = translate_position(board, col, row);
    return vector_get_at(board->pieces, pos);
}

int board_piece_get_col(const Board* board, const Piece* piece)
{
    verify_board(board);
    verify_piece(piece);

    int pos = vector_item_get_index(board->pieces, piece);
    return pos % board->col_num;
}

int board_piece_get_row(const Board* board, const Piece* piece)
{
    verify_board(board);
    verify_piece(piece);

    int pos = vector_item_get_index(board->pieces, piece);
    return pos / board->col_num;
}

bool board_can_piece_move_to(const Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify_board(board);
    verify_board_pos(board, src_col, src_row);
    verify_board_pos(board, dst_col, dst_row);
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);
    if (board_has_piece_at(board, dst_col, dst_row))
    {
        return false;
    }
    if (!piece_can_move_to(piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    if (piece_requires_clear_path(piece) && !board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }

    return true;
}

static bool board_can_piece_promote(Board* board, int col, int row)
{
    Player* player = board_get_active_player(board);
    Color color = player_get_color(player);

    Piece* piece = board_get_piece_at(board, col, row);
    if (color == WHITE && row == 0)
    {
        return true;
    }
    if (color == BLACK && row == board_get_row_num(board) - 1)
    {
        return true;
    }
    return false;
}

void board_piece_move_to(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify_board(board);
    verify_board_pos(board, src_col, src_row);
    verify_board_pos(board, dst_col, dst_row);
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);

    if (board_has_piece_at(board, dst_col, dst_row))
    {
        Piece* dst_piece = board_get_piece_at(board, dst_col, dst_row);
        if (!piece_can_capture(piece, dst_piece, src_col, src_row, dst_col, dst_row))
        {
            return;
        }
        if (!board_can_piece_capture(board, src_col, src_row, dst_col, dst_row))
        {
            return;
        }
        piece_destroy(board_get_piece_at(board, dst_col, dst_row));
    }

    int src_pos = translate_position(board, src_col, src_row);
    vector_set_at(board->pieces, NULL, src_pos);
    int dst_pos = translate_position(board, dst_col, dst_row);
    vector_set_at(board->pieces, piece, dst_pos);

    if (board_can_piece_promote(board, dst_col, dst_row))
    {
        PieceType old_type = piece_get_type(piece);
        piece_promote(piece);

        gamelog_add(
            board->log, 
            "%s promoted %s to a %s at col:%d row:%d",
            board_get_active_player(board) == board_get_player_white(board) ? "WHITE" : "BLACK",
            piece_type_get_name(old_type),
            piece_type_get_name(piece_get_type(piece)),
            dst_col,
            dst_row
        );
    }
    piece_set_moved(piece);

    gamelog_add(
        board->log,
        "%s moved %s from col:%d row:%d to col:%d row:%d",
        board_get_active_player(board) == board_get_player_white(board) ? "WHITE" : "BLACK",
        piece_type_get_name(piece_get_type(piece)),
        src_col,
        src_row,
        dst_col,
        dst_row

    );
}
bool board_can_piece_capture(const Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");
    verify(!board_has_piece_at(board, dst_col, dst_row), "Piece does not exist");

    Piece* src_piece = board_get_piece_at(board, src_col, src_row);
    Piece* dst_piece = board_get_piece_at(board, dst_col, dst_row);
    if (!piece_can_capture(src_piece, dst_piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    if (piece_requires_clear_path(src_piece) && !board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    return true;

}
void board_piece_capture(Board* board, Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    Player* player = board_get_active_player(board);
    int points = piece_get_points(piece);
    player_add_points(player, points);

    gamelog_add(
        board->log,
        "%s captured %s at col:%d row:%d and recieved %d capturing points",
        player == board_get_player_white(board) ? "WHITE" : "BLACK",
        piece_type_get_name(piece_get_type(piece)),
        board_piece_get_col(board, piece),
        board_piece_get_row(board, piece),
        points
    );
}

static bool board_piece_has_clear_path(const Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    // get the distance between the source tile and the next one
    int col_step = sign(dst_col - src_col);
    int row_step = sign(dst_row - src_row);
    // get the next position
    int next_col = src_col + col_step;
    int next_row = src_row + row_step;

    // if the next position is the destination then there was no occupied tile along the way
    if (next_col == dst_col && next_row == dst_row)
    {
        return true;
    }
    // if the position is occupied then there is no clear path
    // this excludes the starting and destination positions
    if (board_has_piece_at(board, next_col, next_row))
    {
        return false;
    }
    // verify the next position
    return board_piece_has_clear_path(board, next_col, next_row, dst_col, dst_row);
}

static void board_add_col(Board* board)
{
    board->col_num++;
    int col_num = board->col_num;
    int row_num = board->row_num;

    // increase vector size
    for (int row = 0; row < row_num; row++)
    {
        vector_add(board->tiles, NULL);
        vector_add(board->pieces, NULL);
    }

    // adjust positions
    int old_pos;
    int new_pos;
    for (int i = col_num * row_num - 1; i >= col_num; i--)
    {
        old_pos = i - (i / col_num);
        new_pos = i;
        vector_swap(board->pieces, old_pos, new_pos);
        vector_swap(board->tiles, old_pos, new_pos);
    }

    // add new spaces
    for (int row = 0; row < row_num; row++)
    {
        Color color = tile_get_default_color(col_num - 1, row);
        Tile* tile = tile_create(TILE_NONE, color);
        int pos = translate_position(board, col_num - 1, row);
        vector_set_at(board->tiles, tile, pos);
    }
}

static void board_add_row(Board* board)
{
    verify_board(board);

    board->row_num++;
    int col_num = board->col_num;
    int row_num = board->row_num;

    for (int col = 0; col < col_num; col++)
    {
        Color color = tile_get_default_color(col, row_num - 1);
        Tile* tile = tile_create(TILE_NONE, color);
        vector_add(board->tiles, tile);
        vector_add(board->pieces, NULL);
    }
}

void board_expand(Board* board)
{
    verify_board(board);

    board_add_col(board);
    board_add_col(board);
    board_add_row(board);
    board_add_row(board);

    int col_num = board->col_num;
    int row_num = board->row_num;

    // adjust piece position
    for (int col = col_num - 1; col >= 0; col--)
    {
        for (int row = row_num - 1; row >= 0; row--)
        {
            int new_col = col;
            int new_row = row;
            int old_col = col - 1;
            int old_row = row - 1;
            if (old_col < 0) old_col = col_num - 1;
            if (old_row < 0) old_row = row_num - 1;

            int old_pos = translate_position(board, old_col, old_row);
            int new_pos = translate_position(board, new_col, new_row);

            vector_swap(board->pieces, old_pos, new_pos);
            vector_swap(board->tiles, old_pos, new_pos);
        }
    }

    char* log_msg = NULL;
    SDL_asprintf(
        &log_msg,
        "%s expanded the size of the board",
        board_get_active_player(board) == board_get_player_white(board) ? "WHITE" : "BLACK"
    );
    gamelog_add(board->log, log_msg);
    SDL_free(log_msg);
}

Tile* board_get_tile_at(const Board* board, int col, int row)
{
    verify_board(board);
    verify_board_pos(board, col, row);

    int pos = translate_position(board, col, row);
    return vector_get_at(board->tiles, pos);
}

int board_tile_get_col(const Board* board, const Tile* tile)
{
    verify_board(board);
    verify_tile(tile);

    int pos = vector_item_get_index(board->tiles, tile);
    return pos % board->col_num;
}

int board_tile_get_row(const Board* board, const Tile* tile)
{
    verify_board(board);
    verify_tile(tile);

    int pos = vector_item_get_index(board->tiles, tile);
    return pos / board->col_num;
}



int board_get_col_num(const Board* board)
{
    verify_board(board);

    return board->col_num;
}

int board_get_row_num(const Board* board)
{
    verify_board(board);

    return board->row_num;
}

static int translate_position(const Board* board, int col, int row)
{
    return col + row * board->col_num;
}

static bool board_player_has_king(const Board* board, const Player* player)
{
    Color color = player_get_color(player);
    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {
            if (!board_has_piece_at(board, col, row))
            {
                continue;
            }
            Piece* piece = board_get_piece_at(board, col, row);
            if (piece_get_type(piece) == KING && piece_get_color(piece) == color)
            {
                return true;
            }
        }
    }
    return false;
}

static bool has_game_ended(const Board* board)
{
    Player* player = board_get_opponent(board);
    if (!board_player_has_king(board, player))
    {
        return true;
    }
    return false;
}

void advance_turn(Board* board)
{
    verify_board(board);

    if (has_game_ended(board))
    {
        board->has_game_ended = true;
        return;
    }

    if (*board->active_player == board->player1)
    {
        board->active_player = &board->player2;
    }
    else
    {
        board->active_player = &board->player1;
    }

    gamelog_add(
        board->log, 
        "===== %s turn =====",
        board_get_active_player(board) == board_get_player_white(board) ? "WHITE" : "BLACK"
    );
}

bool is_player_side_of_board(const Board* board, const Player* player, int row)
{
    verify_board(board);
    verify_player(player);

    if (player_get_color(player) == WHITE && row >= board_get_row_num(board) / 2)
    {
        return true;
    }
    if (player_get_color(player) == BLACK && row + 1 <= board_get_row_num(board) / 2)
    {
        return true;
    }
    return false;
}

Player* board_get_active_player(const Board* board)
{
    verify_board(board);
    return *board->active_player;
}

Player* board_get_opponent(const Board* board)
{
    verify_board(board);
    return *board->active_player == board->player1 ? board->player2 : board->player1;
}

Player* board_get_player_white(const Board* board)
{
    verify_board(board);
    return board->player1;
}

Player* board_get_player_black(const Board* board)
{
    verify_board(board);
    return board->player2;
}

const char* board_get_log(const Board* board)
{
    return gamelog_get(board->log);
}


void verify_board_pos(const Board* board, int col, int row)
{
    verify(col < 0 || col >= board->col_num, "Invalid column");
    verify(row < 0 || row >= board->row_num, "Invalid row");
}

void verify_board(const Board* board)
{
    verify(board == NULL, "Board does not exist");
}
