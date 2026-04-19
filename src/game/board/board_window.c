#include "../../../include/game/board/board_window.h"

Window* board_window_create(Board* board)
{
    Window* window = window_create(double x, double y, double width, double height, SDL_Texture *background_texture);


    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {

        }
    }
}
