#ifndef TETRIS_H
# define TETRIS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define HEIGHT 20
#define WIDTH  15
#define TETRIS 4
#define N_SHAPE	7

typedef	unsigned char	t_field[HEIGHT][WIDTH];
typedef	unsigned char	t_field_row[WIDTH];

typedef	unsigned char	t_mino_array[TETRIS][TETRIS];

typedef struct {
	t_mino_array	array;
	int			size;
} t_mino_shape;

typedef struct {
	int	x;
	int	y;
} t_point;

typedef struct {
	t_mino_shape	shape;
	t_point			pos;
} t_tetrimino;

typedef struct {
	t_tetrimino current;
	t_field		field;
	int			score;
	bool		on;
	suseconds_t timer;
	suseconds_t timer_decrease;
	suseconds_t last_updated;
} t_game;


const t_mino_shape	k_shapes[N_SHAPE]= {
	{
		.array = {
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 2,
	},
	{
		.array = {
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 4,
	},
};

suseconds_t	get_current_time();
void		init(t_game *game);
void		game_loop(t_game *game);
void		print_gameover(t_game *game);

#endif
