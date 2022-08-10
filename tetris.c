#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define ROW 20 // height of game table ROW
#define COLUMN 15 // width of game table CLUMN
#define	MINO_TYPES 7
#define	SQUARE_SIZE 4
#define TRUE 1
#define FALSE 0


char		game_field[ROW][COLUMN] = {0};
int			get_score = 0;
char		now_game = TRUE;
suseconds_t	timer = 400000; // suseconds_t is signed int. express the time. micro seconds
int			decrease = 1000;

typedef struct {
	char	shape[SQUARE_SIZE][SQUARE_SIZE];
	int		mino_size;
} t_mino;

const t_mino	mino_set[MINO_TYPES] = {

}
/*
typedef struct {
    char **shape;
    int width, row, col;
} t_mino;

t_mino current;

// blocks
const t_mino minos[MINO_TYPES]= {
	// . # # | # . .
	// # # . | # # .
	// . . . | . # .
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	// # # . | . # .
	// . # # | # # .
	// . . . | # . .
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	// . # . |
	// # # # |
	// . . . |
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	// . . #
	// # # #
	// . . .
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	// # . . 
	// # # #
	// . . .
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	// # #
	// # #
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	// . . . .
	// # # # #
	// . . . .
	// . . . .
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};
*/


t_mino create_mino(t_mino mino){
	t_mino new_mino = mino;
	new_mino.shape = (char**)malloc(new_mino.width*sizeof(char*));
    int row, column;
    for(row = 0; row < new_mino.width; row++){
		new_mino.shape[row] = (char*)malloc(new_mino.width*sizeof(char));
		for(column = 0; column < new_mino.width; column++) {
			new_mino.shape[row][column] = mino.shape[row][column];
		}
    }
    return new_mino;
}

void delete_mino(t_mino mino){
    int row;
    for(row = 0; row < mino.width; row++){
		free(mino.shape[row]);
    }
    free(mino.shape);
}

// Check Position (.) Check Point
int FunctionCP(t_mino mino){
	char **shape = mino.shape;
	int i, j;
	for(i = 0; i < mino.width;i++) {
		for(j = 0; j < mino.width ;j++){
			if((mino.col+j < 0 || mino.col+j >= COLUMN || mino.row+i >= ROW)){
				if(shape[i][j])
					return FALSE;
				
			}
			else if(game_field[mino.row+i][mino.col+j] && shape[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

// Remove Sharp (#) or shape
void FunctionRS(t_mino shape){
	t_mino temp = create_mino(shape);
	int i, j, k, width;
	width = shape.width;
	for(i = 0; i < width ; i++){
		for(j = 0, k = width-1; j < width ; j++, k--){
				shape.shape[i][j] = temp.shape[k][i];
		}
	}
	delete_mino(temp);
}

// print table
void FunctionPT(){
	char Buffer[ROW][COLUMN] = {0};
	int i, j;
	// add mino
	for(i = 0; i < current.width ;i++){
		for(j = 0; j < current.width ; j++){
			if(current.shape[i][j])
				Buffer[current.row+i][current.col+j] = current.shape[i][j];
		}
	}
	// clear the terminal screen
	clear();
	// print title
	for(i=0; i< COLUMN -9; i++)
		printw(" ");
	printw("42 Tetris\n");
	// print table
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (game_field[i][j] + Buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	// print score
	printw("\nScore: %d\n", get_score);
}

struct timeval before_now, now;
/*
 * struct timeval {
 * 					long	tv_sec; // second
 * 					long	tv_usec; // micro second
 * }
*/
int hasToUpdate(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > timer;
}

void set_timeout(int time) {
	time = 1;
	timeout(1);
}

int main() {
	/*
	 * void	srand(unsigned int seed);
	 * create random number seed
	 * time_t	time(time_t* timer);
	 * timer is output destination
	 */
    srand(time(0));
    get_score = 0;
    int c;
	/*
	 * WINDOW	initscr(void);
	 * screen initialization and manipulation routines
	 */
    initscr();
	/*
	 * int	gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz);
	 * the function can get the time as well as a timezone.
	 */
	gettimeofday(&before_now, NULL);
	set_timeout(1);

	// Create new_block
	t_mino new_shape = create_mino(minos[rand()%7]); // minos[0 ~ 6];
    new_shape.col = rand()%(COLUMN-new_shape.width+1);
    new_shape.row = 0;
    delete_mino(current);
	current = new_shape;
	if(!FunctionCP(current)){
		now_game = FALSE;
	}

	// print table
    FunctionPT();

	// now_game == TRUE == 1
	while(now_game){
		/*
		 * int	getch();
		 * get character from curses terminal keyboad
		 */
		if ((c = getch()) != ERR) {
			// Create block
			t_mino temp = create_mino(current);
			switch(c){
				case 's':
					temp.row++;  //move down
					if(FunctionCP(temp))
						current.row++;
					else {
						int i, j;
						for(i = 0; i < current.width ;i++){
							for(j = 0; j < current.width ; j++){
								if(current.shape[i][j])
									game_field[current.row+i][current.col+j] = current.shape[i][j];
							}
						}
						// erase # line
						int n, m, sum, count=0;
						for(n=0;n< ROW ;n++){
							sum = 0;
							for(m=0;m< COLUMN;m++) {
								sum+= game_field[n][m];
							}
							if(sum== COLUMN){
								count++;
								int l, k;
								for(k = n;k >=1;k--)
									for(l=0;l< COLUMN ;l++)
										game_field[k][l]= game_field[k-1][l];
								for(l=0;l< COLUMN ;l++)
									game_field[k][l]=0;
								timer-=decrease--;
							}
						}

						// count point
						get_score += 100*count;

						// Create new block
						t_mino new_shape = create_mino(minos[rand()%7]);
						new_shape.col = rand()%(COLUMN -new_shape.width+1);
						new_shape.row = 0;
						delete_mino(current);
						current = new_shape;
						if(!FunctionCP(current)){
							now_game = FALSE;
						}
					}
					break;

				case 'd':
					temp.col++;
					if(FunctionCP(temp))
						current.col++;
					break;
				case 'a':
					temp.col--;
					if(FunctionCP(temp))
						current.col--;
					break;
				case 'w':
					FunctionRS(temp);
					if(FunctionCP(temp))
						FunctionRS(current);
					break;
			}
			// Delete Block (temp)
			delete_mino(temp);
			// Print Table
			FunctionPT();
		}
		gettimeofday(&now, NULL);
		if (hasToUpdate()) {
			t_mino temp = create_mino(current);
			// patter of push 's'
			switch('s'){
				case 's':
					temp.row++;
					if(FunctionCP(temp))
						current.row++;
					else {
						int i, j;
						for(i = 0; i < current.width ;i++){
							for(j = 0; j < current.width ; j++){
								if(current.shape[i][j])
									game_field[current.row+i][current.col+j] = current.shape[i][j];
							}
						}
						int n, m, sum, count=0;
						for(n=0;n< ROW ;n++){
							sum = 0;
							for(m=0;m< COLUMN ;m++) {
								sum+= game_field[n][m];
							}
							if(sum== COLUMN ){
								count++;
								int l, k;
								for(k = n;k >=1;k--)
									for(l=0;l< COLUMN ;l++)
										game_field[k][l] = game_field[k-1][l];
								for(l=0;l< COLUMN ;l++)
									game_field[k][l]=0;
								timer-=decrease--;
							}
						}

						// count point
						get_score += 100*count;

						t_mino new_shape = create_mino(minos[rand()%7]);
						new_shape.col = rand()%(COLUMN -new_shape.width+1);
						new_shape.row = 0;
						delete_mino(current);
						current = new_shape;
						if(!FunctionCP(current)){
							now_game = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if(FunctionCP(temp))
						current.col++;
					break;
				case 'a':
					temp.col--;
					if(FunctionCP(temp))
						current.col--;
					break;
				case 'w':
					FunctionRS(temp);
					if(FunctionCP(temp))
						FunctionRS(current);
					break;
			}
			delete_mino(temp);
			FunctionPT();
			gettimeofday(&before_now, NULL);
		}
	}
	// Game Over
	delete_mino(current);
	endwin();
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printf("%c ", game_field[i][j] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", get_score);
    return 0;
}
