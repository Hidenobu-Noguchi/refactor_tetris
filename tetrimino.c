#include <stdlib.h>
#include <stdbool.h>
#include "tetrimino.h"
#include "game.h"
#include "tetris.h"

static const t_mino_shape	k_shapes[N_SHAPE]= {
	{ // Z
		.array = {
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{ // S
		.array = {
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{ // T
		.array = {
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{ // L
		.array = {
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{ // J
		.array = {
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{ // O 
		.array = {
			{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 2,
	},
	{ // I
		.array = {
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 4,
	},
};

t_tetrimino new_tetrimino() {
	t_tetrimino mino;

	mino.shape = k_shapes[rand() % N_SHAPE];
	mino.pos.x = (WIDTH - mino.shape.size) / 2;
	mino.pos.y = HEIGHT - 1;
	return mino;
}

bool	is_in_field(int x, int y) {
	return (0 <= x && x < WIDTH && 0 <= y && y < HEIGHT + BUFZONE);
}

bool is_tetrimino_valid_place(const t_game *game, const t_tetrimino mino) {
	for (int j = 0; j < mino.shape.size; j++) {
		for (int i = 0; i < mino.shape.size; i++) {
			int	x = mino.pos.x + i;
			int	y = mino.pos.y + j;
			if (mino.shape.array[j][i] && (!is_in_field(x, y) || game->field[y][x])) {
				return false;
			}
		}
	}
	return true;
}

static t_point	point_add(const t_point p1, const t_point p2) {
	return (t_point){p1.x + p2.x, p1.y + p2.y};
}

bool	try_move(t_game *game, t_tetrimino mino, const t_point dir) {
	mino.pos = point_add(mino.pos, dir);
	if (!is_tetrimino_valid_place(game, mino))
		return false;
	game->current = mino;
	return true;
}

static t_mino_shape	rotate_shape(const t_mino_shape shape, int dir) {
	t_mino_shape	result = shape;

	for (int j = 0; j < shape.size; j++) {
		for (int i = 0; i < shape.size; i++) {
			if (dir > 0)
				result.array[j][i] = shape.array[i][shape.size - 1 - j];
			else if (dir < 0)
				result.array[j][i] = shape.array[shape.size - 1 - i][j];
		}
	}
	return result;
}

bool	try_rotate(t_game *game, t_tetrimino mino) {
	const int clockwise = 1;

	mino.shape = rotate_shape(mino.shape, clockwise);
	if (!is_tetrimino_valid_place(game, mino))
		return false;
	game->current = mino;
	return true;
}

/*
before:
A B
D C

after:
D A
C B
*/
