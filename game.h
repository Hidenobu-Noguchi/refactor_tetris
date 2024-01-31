#ifndef GAME_H
# define GAME_H

#include <sys/time.h>
#include <stdbool.h>
#include "tetrimino.h"

#define DEFAULT_INTERVAL 1000000

#define CHAR_EMPTY	'.'
#define CHAR_FULL	'#'

typedef struct timeval	t_timeval;
typedef struct s_game	t_game;

struct s_game {
	t_tetrimino current;
	t_field		field;
	int			score;
	bool		is_on;
	suseconds_t interval;
	t_timeval	last_updated;
};

#endif
