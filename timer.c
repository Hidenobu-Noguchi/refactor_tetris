#include <stdbool.h>
#include <sys/time.h>
#include "game.h"

static suseconds_t	difftime_usec(t_timeval t1, t_timeval t0) {
	return (t1.tv_sec - t0.tv_sec) * 1000000L + (t1.tv_usec - t0.tv_usec);
}

t_timeval	get_current_time() {
	t_timeval	now;
	gettimeofday(&now, NULL);
	return now;
}

bool	is_time_to_update(t_game *game) {
	return (difftime_usec(get_current_time(), game->last_updated) > game->interval);
}
