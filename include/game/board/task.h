#pragma once

#include "include/helper_functions/helper_functions.h"
#include "include/helper_functions/vector.h"
#include "include/game/board/board.h"

typedef enum Task 
{
    TASK_ADD_PAWN,

    TASK_COUNT
} Task;

Vector* task_list_create();
void task_list_destroy(Vector* task_list);
void add_task(Vector* task_list, Task task);
bool has_task(const Vector* task_list);
Task task_get_first(const Vector* task_list);
void task_complete_first(Vector* task_list);

bool task_is_valid_tile__addPawn(Board* board, int col, int row);
int task__addPawn(Board* board, int col, int row);
