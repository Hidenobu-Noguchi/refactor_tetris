#ifndef TETRIS_H
# define TETRIS_H

#include <sys/time.h>
#include <stdbool.h>

typedef struct s_game		t_game;
typedef struct s_tetrimino	t_tetrimino;
typedef struct s_point		t_point;
typedef struct timeval		t_timeval;

// game_loop.c
void	game_init(t_game *game);
void	game_loop(t_game *game);
void	game_end(const t_game *game);

// print.c
void	print_board(const t_game *game, const t_tetrimino mino);
void	print_gameover(const t_game *game);

// field.c
int		drop_lines(t_game *game, const t_tetrimino mino);

// tetrimino.c
t_tetrimino new_tetrimino();
bool	try_move(t_game *game, t_tetrimino mino, const t_point dir);
bool	try_rotate(t_game *game, t_tetrimino mino);
bool	is_in_field(int x, int y);
bool	is_tetrimino_valid_place(const t_game *game, const t_tetrimino mino);

// timer.c
t_timeval	get_current_time(void);
bool		is_time_to_update(const t_game *game);

#endif
