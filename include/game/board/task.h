// tasks for the game
// responsible for managing interactions between the tree and the board
#pragma once

#include "include/helper_functions/helper_functions.h"
#include "include/helper_functions/vector.h"
#include "include/game/board/board.h"
#include "ui_elements/window.h"

typedef enum Task 
{
    TASK_ADD_PAWN,
    TASK_EXPAND_BOARD,
    TASK_ADD_LANCE,
    TASK_SACRIFICE,

    TASK_COUNT
} Task;

Vector* task_list_create();
void task_list_destroy(Vector* task_list);
void add_task(Vector* task_list, Task task);
bool has_task(const Vector* task_list);
Task task_get_first(const Vector* task_list);
void task_complete_first(Vector* task_list);

// task add pawn
bool task_is_valid_tile__addPawn(Board* board, int col, int row);
int task__addPawn(Board* board, int col, int row);
// task expand board
int task__expandBoard(Board* board, Window* window);
bool task_is_valid_tile__expandBoard(Board* board);
// task add lance
bool task_is_valid_tile__addLance(Board* board, int col, int row);
int task__addLance(Board* board, int col, int row);
// task sacrifice
int task__sacrifice(Board* board, int col, int row);
bool task_is_valid_tile__sacrifice(Board* board, int col, int row);
