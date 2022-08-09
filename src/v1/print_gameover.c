#include "tetris.h"

void	print_gameover(t_game *game){
	endwin();
	int j, i;
	for (j = 0; j < HEIGHT; j++){
		for (i = 0; i < WIDTH; i++){
			printf("%c ", game->field[j][i] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", game->score);
}

