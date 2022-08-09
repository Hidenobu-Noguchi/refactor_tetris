#include "game.h"

int main() {
	t_game	game;

	game_init(&game);
	game_loop(&game);
	print_gameover(&game);
	return 0;
}

#ifdef LEAKS
__attribute__((destructor)) void destructor(void)
{
	system("leaks -q tetris");
}
#endif
