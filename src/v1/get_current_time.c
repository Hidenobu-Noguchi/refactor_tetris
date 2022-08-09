#include "tetris.h"

suseconds_t	get_current_time(){
	struct timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000000 + now.tv_usec);
}

