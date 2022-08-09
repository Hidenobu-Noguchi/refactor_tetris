#include <stdbool.h>
#include <sys/time.h>
#include "game.h"

suseconds_t	get_current_time(){
	struct timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000000 + now.tv_usec);
}

bool	is_time_to_update(t_game *game){
	return (get_current_time() - game->last_updated) > game->timer;
}

