#include "include/game/board/task.h"
#include "game/board/piece.h"
#include "helper_functions/error_handling.h"


Vector* task_list_create()
{
    Vector* task_list = vector_create(default_ops());
    verify(task_list == NULL, "Task list could not be created: malloc");
    return task_list;
}
void task_list_destroy(Vector* task_list)
{
    verify(task_list == NULL, "Task list does not exist");
    vector_destroy(task_list);
}

void add_task(Vector* task_list, Task task)
{
    verify(task_list == NULL, "Task list does not exist");
    verify(task < 0 || task >= TASK_COUNT, "Invalid Task");

    Task* task_to_add = SDL_malloc(sizeof(Task));
    verify(task_to_add == NULL, "Task could not be added: malloc");
    *task_to_add = task;
    vector_add(task_list, task_to_add);
}
bool has_task(const Vector* task_list)
{
    verify(task_list == NULL, "Task list does not exist");

    return vector_get_count(task_list) > 0;
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



bool task_is_valid_tile__addPawn(Board* board, int col, int row)
{
    Player* player = board_get_active_player(board);

    if (!board_can_add_piece_at(board, col, row))
    {
        return false;
    }
    if (!is_player_side_of_board(board, player, row))
    {
        return 0;
    }
    return true;
}
int task__addPawn(Board* board, int col, int row)
{
    Player* player = board_get_active_player(board);
    Color player_color = player_get_color(player);

    if (!task_is_valid_tile__addPawn(board, col, row))
    {
        return 0;
    }
    Piece* piece = piece_create(PAWN, player_color);
    piece_set_moved(piece);
    board_add_piece_at(board, piece, col, row);
    return 1;
}
