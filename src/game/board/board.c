#include "../../../include/game/board/board.h"

void init_board()
{
	int i;

	for (i = 0; i < 64; ++i) {
		color[i] = init_color[i];
		piece[i] = init_piece[i];
	}
}

