#ifndef GAME_H
# define GAME_H

#include <sys/time.h>
#include <stdbool.h>
#include "tetrimino.h"

typedef struct timeval	t_timeval;

typedef struct s_game	t_game;

struct s_game {
	t_tetrimino current;
	t_field		field;
	int			score;
	bool		on;
	suseconds_t timer;
	suseconds_t timer_decrease;
	t_timeval	last_updated;
};

#endif
