#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "tetrimino.h"

static bool	is_row_filled(const t_field_row row) {
	for (int i = 0; i < WIDTH; i++) {
		if (!row[i]) {
			return false;
		}
	}
	return true;
}

static void	clear_field_row(t_game *game, int y) {
	for (int j = y; j < HEIGHT - 1; j++) {
		memcpy(game->field[j], game->field[j + 1], sizeof(game->field[j]));
	}
	memset(game->field[HEIGHT - 1], 0, sizeof(game->field[0]));
}

static void	put_tetrimino_to_field(t_game *game, const t_tetrimino mino) {
	for (int j = 0; j < mino.shape.size; j++) {
		for (int i = 0; i < mino.shape.size; i++) {
			if (mino.shape.array[j][i]) {
				game->field[mino.pos.y + j][mino.pos.x + i] = mino.shape.array[j][i];
			}
		}
	}
}

int	drop_lines(t_game *game, const t_tetrimino mino) {
	put_tetrimino_to_field(game, mino);
	int	count = 0;
	for (int y = HEIGHT; y >= 0; y--) {
		if (is_row_filled(game->field[y])) {
			clear_field_row(game, y);
			count++;
			int level = game->score / 100;
			game->interval = pow(0.8 - level*0.007, level) * 1000000;
		}
	}
	return count;
}
