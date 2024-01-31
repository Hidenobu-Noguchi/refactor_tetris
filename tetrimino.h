#ifndef TETRIMINO_H
# define TETRIMINO_H

#define HEIGHT 20
#define WIDTH  10
#define TETRIS 4
#define N_SHAPE	7

typedef unsigned char	t_block;

typedef	t_block	t_field[HEIGHT][WIDTH];
typedef	t_block	t_field_row[WIDTH];
typedef	t_block	t_mino_array[TETRIS][TETRIS];

typedef struct s_tetrimino	t_tetrimino;
typedef struct s_point		t_point;
typedef struct s_mino_shape	t_mino_shape;

struct s_mino_shape {
	t_mino_array	array;
	int				size;
};

struct s_point {
	int	x;
	int	y;
};

struct s_tetrimino {
	t_mino_shape	shape;
	t_point			pos;
};

#endif
