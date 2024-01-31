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

	printw("%*c42 Tetris\n\n", WIDTH - 4, ' ');
	for (int j = HEIGHT - 1; j >= 0; j--) {
		printw(" |");
		for (int i = 0; i < WIDTH; i++) {
			printw(" %c", (game->field[j][i] || buffer[j][i]) ? CHAR_FULL : CHAR_EMPTY);
		}
		printw(" |\n");
	}
	printw("\n Score: %d\n Interval: %d\n", game->score, game->interval);
}

void	print_gameover(const t_game *game) {
	printf("\033[2J");
	printf("%*c42 Tetris\n\n", WIDTH - 4, ' ');
	for (int j = HEIGHT - 1; j >= 0; j--) {
		printf(" |");
		for (int i = 0; i < WIDTH; i++) {
			printf(" %c", game->field[j][i] ? CHAR_FULL : CHAR_EMPTY);
		}
		printf(" |\n");
	}
	printf("\n Game over!\n");
	printf("\n Score: %d\n", game->score);
}
