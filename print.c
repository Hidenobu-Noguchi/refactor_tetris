#include <ncurses.h>
#include "game.h"
#include "tetrimino.h"
#include "tetris.h"

static void	fill_buffer(t_field buffer, const t_tetrimino mino) {
	for (int j = 0; j < mino.shape.size; j++) {
		for (int i = 0; i < mino.shape.size; i++) {
			int	x = mino.pos.x + i;
			int	y = mino.pos.y + j;
			if (mino.shape.array[j][i] && is_in_field(x, y)) {
				buffer[y][x] = mino.shape.array[j][i];
			}
		}
	}
}

void	print_board(const t_game *game, const t_tetrimino mino) {
	clear();

	t_field	buffer = {};
	fill_buffer(buffer, mino);

	printw("%*c42 Tetris\n", WIDTH / 2, ' ');
	for (int i = 0; i < WIDTH; i++) {
		printw("%c ", '_');
	}
	printw("\n");
	for (int j = HEIGHT - 1; j >= 0; j--) {
		for (int i = 0; i < WIDTH; i++) {
			printw("%c ", (game->field[j][i] || buffer[j][i]) ? CHAR_FULL : CHAR_EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}

void	print_gameover(const t_game *game) {
	for (int j = HEIGHT - 1; j >= 0; j--) {
		for (int i = 0; i < WIDTH; i++) {
			printf("%c ", game->field[j][i] ? CHAR_FULL : CHAR_EMPTY);
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", game->score);
}
