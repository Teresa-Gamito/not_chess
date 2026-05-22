#include "include/game/task.h"


Vector* task_list_create()
{
    Vector* task_list = vector_create();
    verify(task_list == NULL, "Task list could not be created: malloc");
    return task_list;
}
void task_list_destroy(Vector* task_list)
{
    verify_tasklist(task_list);

    vector_destroy(task_list);
}

void add_task(Vector* task_list, Task task)
{
    verify_tasklist(task_list);

    Task* task_to_add = SDL_malloc(sizeof(Task));
    verify(task_to_add == NULL, "Task could not be added: malloc");
    *task_to_add = task;
    vector_add(task_list, task_to_add);
}
bool has_task(const Vector* task_list)
{
    verify(task_list == NULL, "Task list does not exist");

    return vector_get_size(task_list) > 0;
}
Task task_get_first(const Vector* task_list)
{
    verify(task_list == NULL, "Task list does not exist");
    verify(!has_task(task_list), "Task list does not containt any task");

    return *(Task*)vector_get_at(task_list, 0);
}
void task_complete_first(Vector* task_list)
{
    verify(task_list == NULL, "Task list does not exist");
    verify(!has_task(task_list), "Task list does not containt any task");

    Task* task = vector_get_at(task_list, 0);
    vector_destroy_item(task_list, task);
}



bool task_is_valid_tile__addPiece(Board* board, Pos pos)
{
    Player* player = board_get_active_player(board);

    if (!board_can_add_piece_at(board, pos))
    {
        return false;
    }
    if (!is_player_side_of_board(board, player, pos.row))
    {
        return false;
    }
    return true;
}
int task__addPiece(Board* board, PieceType piece_type, Pos pos)
{
    Player* player = board_get_active_player(board);
    Color player_color = player_get_color(player);

    if (!task_is_valid_tile__addPiece(board, pos))
    {
        return 0;
    }
    Piece* piece = piece_create(piece_type, player_color);
    piece_set_moved(piece);
    board_add_piece_at(board, piece, pos);
    return 1;
}

bool task_is_valid_tile__expandBoard(Board* board)
{
    (void)board;
    return true;
}
int task__expandBoard(Board* board, Window* window)
{
    int old_col_num = board_get_col_num(board);
    board_expand(board);
    int new_col_num = board_get_col_num(board);
    // float new_scale = window_get_scale(window) * old_col_num / new_col_num;
    // window_set_scale(window, new_scale);
    return 1;
}

bool task_is_valid_tile__sacrifice(Board* board, Pos pos)
{
    Player* player = board_get_active_player(board);
    Color player_color = player_get_color(player);

    if (!board_has_piece_at(board, pos))
    {
        return false;
    }
    Piece* piece = board_get_piece_at(board, pos);
    Color piece_color = piece_get_color(piece);
    if (piece_color != player_color)
    {
        return false;
    }
    return true;
}
int task__sacrifice(Board* board, Pos pos)
{
    if (!task_is_valid_tile__sacrifice(board, pos))
    {
        return 0;
    }
    board_piece_remove(board, pos);
    return 1;
}

void verify_tasklist(const Vector* tasklist)
{
    verify(tasklist, "Tasklist does not exist");
}
