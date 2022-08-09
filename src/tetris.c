// tetris.h
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define HEIGHT 20
#define WIDTH  15
#define TETRIS 4
#define N_SHAPE	7

typedef	unsigned char	t_field[HEIGHT][WIDTH];
typedef	unsigned char	t_field_row[WIDTH];

typedef	unsigned char	t_mino_array[TETRIS][TETRIS];

typedef struct {
	t_mino_array	array;
	int			size;
} t_mino_shape;

typedef struct {
	int	x;
	int	y;
} t_point;

typedef struct {
	t_mino_shape	shape;
	t_point			pos;
} t_tetrimino;

typedef struct {
	t_tetrimino current;
	t_field		field;
	int			score;
	bool		on;
	suseconds_t timer;
	suseconds_t timer_decrease;
	suseconds_t last_updated;
} t_game;


const t_mino_shape	k_shapes[N_SHAPE]= {
	{
		.array = {
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 3,
	},
	{
		.array = {
			{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 2,
	},
	{
		.array = {
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
		},
		.size = 4,
	},
};


t_tetrimino tetrimino_random(){
	t_tetrimino new_mino;

	new_mino.shape = (k_shapes[rand() % N_SHAPE]);
	new_mino.pos.x = rand() % (WIDTH - new_mino.shape.size + 1);
	new_mino.pos.y = 0;
	return new_mino;
}

bool tetrimino_is_valid_place(t_game *game, t_tetrimino mino){
	for (int j = 0; j < mino.shape.size; j++) {
		for (int i = 0; i < mino.shape.size; i++){
			if (!mino.shape.array[j][i]){
				continue;
			}
			if (mino.pos.x + i < 0
					|| WIDTH <= mino.pos.x + i
					|| HEIGHT <= mino.pos.y + j
					|| game->field[mino.pos.y + j][mino.pos.x + i])
			{
				return false;
			}
		}
	}
	return true;
}

void print_board(t_game *game, t_tetrimino mino){
	clear();
	t_field	buffer = {};
	for (int j = 0; j < mino.shape.size; j++){
		for (int i = 0; i < mino.shape.size; i++){
			if (mino.shape.array[j][i])
				buffer[mino.pos.y + j][mino.pos.x + i] = mino.shape.array[j][i];
		}
	}
	printw("%*c42 Tetris\n", WIDTH-10, ' ');
	for (int j = 0; j < HEIGHT; j++){
		for (int i = 0; i < WIDTH; i++){
			printw("%c ", (game->field[j][i] || buffer[j][i])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}

/*
 * get_current_time.c
 */
suseconds_t	get_current_time(){
	struct timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000000 + now.tv_usec);
}

bool	is_time_to_update(t_game *game){
	return (get_current_time() - game->last_updated) > game->timer;
}

/*
 * print_gameover.c
 *
 * */
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

bool	is_row_filled(const t_field_row row){
	for (int i = 0; i < WIDTH; i++) {
		if (!row[i]){
			return false;
		}
	}
	return true;
}

void	put_to_field(t_game *game, const t_tetrimino mino){
	for (int j = 0; j < mino.shape.size; j++){
		for (int i = 0; i < mino.shape.size; i++){
			if (mino.shape.array[j][i]){
				game->field[mino.pos.y + j][mino.pos.x + i] = mino.shape.array[j][i];
			}
		}
	}
}

void	clear_field_row(t_game *game, int y){
	for (int j = y; j >= 1; j--)
		memcpy(game->field[j], game->field[j - 1], sizeof(game->field[j]));
	memset(game->field[0], 0, sizeof(game->field[0]));
}

int	handle_lines(t_game *game, const t_tetrimino mino){
	put_to_field(game, mino);
	int	count = 0;
	for (int y = 0; y < HEIGHT; y++){
		if (is_row_filled(game->field[y])){
			clear_field_row(game, y);
			count++;
			game->timer -= game->timer_decrease--;
		}
	}
	return count;
}

t_point	point_add(const t_point p1, const t_point p2){
	return (t_point){p1.x + p2.x, p1.y + p2.y};
}

bool	try_move(t_game *game, t_tetrimino mino, t_point dir){
	mino.pos = point_add(mino.pos, dir);
	if (!tetrimino_is_valid_place(game, mino))
		return false;
	game->current = mino;
	return true;
}

t_mino_shape	shape_rotate(t_mino_shape shape){
	t_mino_shape	result = shape;
	for (int j = 0; j < shape.size; j++){
		for (int i = 0; i < shape.size; i++){
			result.array[j][i] = shape.array[shape.size - 1 - i][j];
		}
	}
	return result;
}

bool	try_rotate(t_game *game, t_tetrimino mino){
	mino.shape = shape_rotate(mino.shape);
	if (!tetrimino_is_valid_place(game, mino))
		return false;
	game->current = mino;
	return true;
}

void	drop_mino(t_game *game){
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

bool	key_handle(t_game *game, int key){
	const t_point	left = {-1, 0};
	const t_point	down = {0, 1};
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
		case ERR:
		default:
			return false;
	}
	print_board(game, game->current);
	return true;
}

/*
 * init.c
 */
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

/*
 * game_loop.c
 * gome_loop()
 * tetrimino_random()
 * print_board()
 * key_handle()
 * is_time_to_update()
 * drop_mino()
 */
void	game_loop(t_game *game) {
	game->current = tetrimino_random();
	print_board(game, game->current);
	while(game->on){
		key_handle(game, getch());
		if (is_time_to_update(game)) {
			drop_mino(game);
		}
	}
}

// tetris.c
int main() {
	t_game	game;

	init(&game);
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
