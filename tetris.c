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

t_tetrimino g_current = {};
t_field	g_field = {};
int		g_score = 0;
bool	g_GameOn = true;
suseconds_t g_timer = 400000;
suseconds_t g_timer_decrease = 1000;

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

bool tetrimino_is_valid_place(t_tetrimino mino){
	for (int j = 0; j < mino.shape.size; j++) {
		for (int i = 0; i < mino.shape.size; i++){
			if (!mino.shape.array[j][i]){
				continue;
			}
			if (mino.pos.x + i < 0
				|| WIDTH <= mino.pos.x + i
				|| HEIGHT <= mino.pos.y + j
				|| g_field[mino.pos.y + j][mino.pos.x + i])
			{
				return false;
			}
		}
	}
	return true;
}

void print_board(t_tetrimino mino){
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
			printw("%c ", (g_field[j][i] || buffer[j][i])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", g_score);
}

bool	hasToUpdate(struct timeval past){
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - (suseconds_t)(past.tv_sec * 1000000 + past.tv_usec)) > g_timer;
}

void	init(){
	srand(time(NULL));
	initscr();
	timeout(1);
	g_score = 0;
	g_current = tetrimino_random();
}

void	print_gameover(){
	endwin();
	int j, i;
	for (j = 0; j < HEIGHT; j++){
		for (i = 0; i < WIDTH; i++){
			printf("%c ", g_field[j][i] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", g_score);
}

bool	is_row_filled(const t_field_row row){
	for (int i = 0; i < WIDTH; i++) {
		if (!row[i]){
			return false;
		}
	}
	return true;
}

void	update_field(const t_tetrimino mino){
	for (int j = 0; j < mino.shape.size; j++){
		for (int i = 0; i < mino.shape.size; i++){
			if (mino.shape.array[j][i]){
				g_field[mino.pos.y + j][mino.pos.x + i] = mino.shape.array[j][i];
			}
		}
	}
}

void	clear_field_row(int y){
	for (int j = y; j >= 1; j--)
		memcpy(g_field[j], g_field[j - 1], sizeof(g_field[j]));
	memset(g_field[0], 0, sizeof(g_field[0]));
}

int	handle_lines(const t_tetrimino mino){
	update_field(mino);
	int	count = 0;
	for (int y = 0; y < HEIGHT; y++){
		if (is_row_filled(g_field[y])){
			clear_field_row(y);
			count++;
			g_timer -= g_timer_decrease--;
		}
	}
	return count;
}

t_point	point_add(t_point p1, t_point p2){
	return (t_point){p1.x + p2.x, p1.y + p2.y};
}

bool	try_move(t_tetrimino mino, t_point dir){
	mino.pos = point_add(mino.pos, dir);
	if (!tetrimino_is_valid_place(mino))
		return false;
	g_current = mino;
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

bool	try_rotate(t_tetrimino mino){
	mino.shape = shape_rotate(mino.shape);
	if (!tetrimino_is_valid_place(mino))
		return false;
	g_current = mino;
	return true;
}

bool	key_handle(int key){
	const t_point	left = {-1, 0};
	const t_point	down = {0, 1};
	const t_point	right = {1, 0};

	switch(key){
		case 'w':
		case KEY_UP:
			try_rotate(g_current);
			break;
		case 'a':
		case KEY_LEFT:
			try_move(g_current, left);
			break;
		case 's':
		case KEY_DOWN:
			try_move(g_current, down);
			break;
		case 'd':
		case KEY_RIGHT:
			try_move(g_current, right);
			break;
		case ERR:
		default:
			return false;
	}
	print_board(g_current);
	return true;
}

int main() {
	init();

	struct timeval past;
	gettimeofday(&past, NULL);

	print_board(g_current);

	while(g_GameOn){
		key_handle(getch());
		if (hasToUpdate(past)) {
			const t_point	down = {0, 1};
			if (!try_move(g_current, down)){
				g_score += 100 * handle_lines(g_current);

				g_current = tetrimino_random();

				if (!tetrimino_is_valid_place(g_current)){
					g_GameOn = false;
				}
			}

			print_board(g_current);
			gettimeofday(&past, NULL);
		}
	}
	print_gameover();
	return 0;
}

#ifdef LEAKS
__attribute__((destructor)) void destructor(void)
{
	system("leaks -q tetris");
}
#endif
