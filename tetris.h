#ifndef TETRIS_H
# define TETRIS_H

#include <sys/time.h>
#include <stdbool.h>

typedef struct s_game		t_game;
typedef struct s_tetrimino	t_tetrimino;
typedef struct s_point		t_point;

// game_loop.c
void	game_init(t_game *game);
void	game_loop(t_game *game);

//print.c
void	print_board(t_game *game, t_tetrimino mino);
void	print_gameover(t_game *game);

// field.c
int		handle_lines(t_game *game, const t_tetrimino mino);

// tetrimino.c
t_tetrimino tetrimino_random();
bool	try_move(t_game *game, t_tetrimino mino, t_point dir);
bool	try_rotate(t_game *game, t_tetrimino mino);
bool	tetrimino_is_valid_place(t_game *game, t_tetrimino mino);

//timer.c
struct timeval	get_current_time();
bool			is_time_to_update(t_game *game);

#endif
