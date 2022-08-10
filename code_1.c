void	execute_command(char c)
{
	t_mino temp_mino = create_mino(current_mino);
	switch(c){
		case 's':
			temp_mino.pos.y++;
			if(check_mino_position(temp_mino))
				current_mino.pos.y++;
			else {
				for(int i = 0; i < current_mino.shape.size ;i++) {
					for(int j = 0; j < current_mino.shape.size ; j++) {
						if(current_mino.shape[i][j])
							game_field[current_mino.pos.y+i][current_mino.col+j] = current_mino.shape[i][j];
						}
					}
					int sum, count=0;
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
								game_field[k][l]=0;
							timer -= decrease--;
						}
					}
					get_score += 100 * count;
					new_mino = create_mino(mino_set[rand() % 7]);
					new_mino.pos.x = rand() % (COLUMN - new_mino.shape.size + 1);
					new_mino.pos.y = 0;
					current_mino = new_mino;
					if(!check_mino_position(current_mino)) {
						is_running = FALSE;
				}
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
			rotate_mino(temp_mino);
			if(check_mino_position(temp_mino))
				rotate_mino(current_mino);
			break;
		}
		print_table();
	}
}

if ((c = getch()) != ERR) {
		gettimeofday(&now, NULL);

