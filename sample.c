#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define ROW 20
#define COLUMN 15
#define	MINO_TYPES 7
#define	SQUARE_SIZE 4
#define TRUE 1
#define FALSE 0

char			game_field[ROW][COLUMN] = {0};
int				get_score = 0;
char			is_running = TRUE;
suseconds_t		interval = 400000;
int				up_speed = 1000;
struct timeval	starting_time;
struct timeval 	present_time;

typedef unsigned char	t_mino_array[SQUARE_SIZE][SQUARE_SIZE];

typedef struct s_mino_shape	t_mino_shape;
typedef struct s_position	t_position;
typedef struct s_mino		t_mino;

struct s_mino_shape {
	t_mino_array	array;
	int				size;
};

struct s_position {
	int	x;
	int	y;
};

struct s_mino {
	t_mino_shape	shape;
	t_position		pos;
};

t_mino	current_mino;

const t_mino_shape	mino_set[MINO_TYPES] = {
	{
		.array = {
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 3,
	},
	{
		.array = {
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 3,
	},
	{
		.array = {
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 3,
	},
	{
		.array = {
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 3,
	},
	{
		.array = {
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 3,
	},
	{
		.array = {
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 2,
	},
	{
		.array = {
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		.size = 4,
	},
};

t_mino	create_mino(void) {
	t_mino			new_mino;
	t_mino_shape	new_shape = mino_set[rand() % 7];
	new_mino.shape = new_shape;
	new_mino.pos.x = rand() % (COLUMN - (new_mino.shape.size + 1));
	new_mino.pos.y = 0;
	return (new_mino);
}

static void	add_if_exist_mino(char (*mino_pos)[15], int row, int col)
{
	if(current_mino.shape.array[row][col])
		mino_pos[current_mino.pos.y + row][current_mino.pos.x + col] = current_mino.shape.array[row][col];
}

static void	add_current_mino_pos(char (*mino_pos)[15])
{
	for(int row = 0; row < current_mino.shape.size ;row++) {
		for(int col = 0; col < current_mino.shape.size ; col++) {
			add_if_exist_mino(mino_pos, row, col);
		}
	}
}

static void	print_title()
{
	for(int i = 0; i < COLUMN - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
}

static void	print_game_table(char (*mino_pos)[15])
{
	for(int i = 0; i < ROW ; i++){
		for(int j = 0; j < COLUMN ; j++){
			printw("%c ", (game_field[i][j] + mino_pos[i][j]) ? '#' : '.');
		}
		printw("\n");
	}
}

static void	print_score()
{
	printw("\nScore: %d\n", get_score);
}

void	print_table()
{
	char current_mino_pos[ROW][COLUMN] = {0};
	add_current_mino_pos(current_mino_pos);
	clear();
	print_title();
	print_game_table(current_mino_pos);
	print_score();
}

int check_mino_position(t_mino mino) {
	for(int i = 0; i < mino.shape.size; i++) {
		for(int j = 0; j < mino.shape.size; j++) {
			int	x = mino.pos.x + j;
			int	y = mino.pos.y + i;

			if (y < 0 || ROW <= y || x < 0 || COLUMN <= x)
			{
				if (mino.shape.array[i][j])
					return FALSE;
			}
			else if (game_field[y][x] && mino.shape.array[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

void	rotate_mino(t_mino *mino) {
	t_mino_shape	before_shape = mino->shape;
	int 			width = mino->shape.size;
	int				k;

	for(int i = 0; i < width; i++){
		for(int j = 0, k = width - 1; j < width; j++, k--){
				mino->shape.array[i][j] = before_shape.array[k][i];
		}
	}
}

// void	execute_command(void)
void	execute_command(int c)
{
	t_mino	new_mino;
	t_mino	temp_mino = current_mino;

	switch(c){
		case 's':
			temp_mino.pos.y++;
			if(check_mino_position(temp_mino))
				current_mino.pos.y++;
			else {
				for(int i = 0; i < current_mino.shape.size ;i++) {
					for(int j = 0; j < current_mino.shape.size ; j++) {
						if(current_mino.shape.array[i][j])
							game_field[current_mino.pos.y + i][current_mino.pos.x + j] = current_mino.shape.array[i][j];
						}
					}
					int sum, count = 0;
					for(int n = 0; n < ROW; n++) {
						sum = 0;
						for(int m = 0; m < COLUMN; m++) {
							sum += game_field[n][m];
						}
						if(sum == COLUMN) {
							count++;
							int k;
							for(k = n; k >= 1; k--)
								for(int l = 0; l < COLUMN; l++)
									game_field[k][l] = game_field[k - 1][l];
							for(int l = 0; l < COLUMN ; l++)
								game_field[k][l] = 0;
							interval -= up_speed;
							up_speed--;
						}
					}
					get_score += 100 * count;
					new_mino = create_mino();
					new_mino.pos.x = rand() % (COLUMN - new_mino.shape.size + 1);
					new_mino.pos.y = 0;
					current_mino = new_mino;
					if(!check_mino_position(current_mino))
						is_running = FALSE;
			}
			break;

		case 'd':
			temp_mino.pos.x++;
			if(check_mino_position(temp_mino))
				current_mino.pos.x++;
			break;

		case 'a':
			temp_mino.pos.x--;
			if(check_mino_position(temp_mino))
				current_mino.pos.x--;
			break;

		case 'w':
			rotate_mino(&temp_mino);
			if(check_mino_position(temp_mino))
				rotate_mino(&current_mino);
			break;
	}
	print_table();
}

int is_time_to_update(){
	return ((suseconds_t)((present_time.tv_sec * 1000000) + present_time.tv_usec) - (suseconds_t)((starting_time.tv_sec * 1000000) + starting_time.tv_usec)) > timer;
}

void	running_game()
{
	int		c;

	while(is_running) {
		if ((c = getch()) != ERR)
			execute_command(c);
		gettimeofday(&present_time, NULL);
		if (is_time_to_update())
		{
			execute_command('s');
			gettimeofday(&starting_time, NULL);
		}
	}
}

void	initialize_game() {
	t_mino	new_mino = create_mino();
   
    initscr();
	gettimeofday(&starting_time, NULL);
	timeout(1);
	srand((unsigned int)starting_time.tv_usec);

	get_score = 0;
	current_mino = new_mino;

	if(!check_mino_position(current_mino))
		is_running = FALSE;
	print_table();
}

int main() {

	initialize_game();
	running_game();

	// Game Over
	endwin();
	for(int i = 0; i < ROW ;i++) {
		for(int j = 0; j < COLUMN ; j++) {
			printf("%c ", game_field[i][j] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", get_score);
    return 0;
}
