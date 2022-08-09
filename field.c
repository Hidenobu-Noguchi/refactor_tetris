#include <stdbool.h>
#include <string.h>
#include "game.h"

static void	clear_field_row(t_game *game, int y){
	for (int j = y; j >= 1; j--)
		memcpy(game->field[j], game->field[j - 1], sizeof(game->field[j]));
	memset(game->field[0], 0, sizeof(game->field[0]));
}

static bool	is_row_filled(const t_field_row row){
	for (int i = 0; i < WIDTH; i++) {
		if (!row[i]){
			return false;
		}
	}
	return true;
}

static void	tetrimino_put_to_field(t_game *game, const t_tetrimino mino){
	for (int j = 0; j < mino.shape.size; j++){
		for (int i = 0; i < mino.shape.size; i++){
			if (mino.shape.array[j][i]){
				game->field[mino.pos.y + j][mino.pos.x + i] = mino.shape.array[j][i];
			}
		}
	}
}

int	handle_lines(t_game *game, const t_tetrimino mino){
	tetrimino_put_to_field(game, mino);
	int	count = 0;
	for (int y = 0; y < HEIGHT; y++){
		if (is_row_filled(game->field[y])){
			clear_field_row(game, y);
			count++;
			game->timer -= game->timer_decrease--;
		}
	}
	return count;
}
