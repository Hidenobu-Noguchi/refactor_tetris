#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include "tetris.h"
#include "game.h"

void	game_init(t_game *game){
	srand(get_current_time().tv_usec);
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

static void	drop_mino(t_game *game){
	const t_point	down = {0, 1};
	if (!try_move(game, game->current, down)){
		game->score += 100 * handle_lines(game, game->current);

		game->current = tetrimino_random();
		if (!tetrimino_is_valid_place(game, game->current)){
			game->on = false;
		}
	}
	print_board(game, game->current);
	game->last_updated = get_current_time();
}

static bool	key_handle(t_game *game, int key){
	const t_point	left = {-1, 0};
	const t_point	right = {1, 0};

	switch(key){
		case 'w':
			try_rotate(game, game->current);
			break;
		case 'a':
			try_move(game, game->current, left);
			break;
		case 's':
			drop_mino(game);
			break;
		case 'd':
			try_move(game, game->current, right);
			break;
		default:
			return false;
	}
	print_board(game, game->current);
	return true;
}

void	game_loop(t_game *game){
	game->current = tetrimino_random();
	print_board(game, game->current);
	while(game->on){
		key_handle(game, getch());
		if (is_time_to_update(game)) {
			drop_mino(game);
		}
	}
}
