#include "tetris.h"

void	init(t_game *game){
	srand(time(NULL));
	initscr();
	timeout(1);

	*game = (t_game){
		.current = {},
		.field = {},
		.score = 0,
		.on = true,
		.timer = 400000,
		.timer_decrease = 1000,
		.last_updated = get_current_time(),
	};
}

