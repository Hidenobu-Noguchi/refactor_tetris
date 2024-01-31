#include "tetris.h"
#include "game.h"

int main() {
	t_game	game;

	game_init(&game);
	game_loop(&game);
	game_end(&game);
	return 0;
}

#ifdef LEAKS
#include <stdlib.h>

__attribute__((destructor)) void destructor(void)
{
	system("leaks -q tetris");
}
#endif
