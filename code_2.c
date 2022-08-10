		if (hasToUpdate()) {
			t_mino temp_mino = create_mino(current_mino);
			switch('s'){
				case 's':
					temp_mino.row++;
					if(FunctionCP(temp_mino))
						current_mino.row++;
					else {
						int i, j;
						for(i = 0; i < current_mino.width ;i++){
							for(j = 0; j < current_mino.width ; j++){
								if(current_mino.shape[i][j])
									game_field[current_mino.row+i][current_mino.col+j] = current_mino.shape[i][j];
							}
						}
						int n, m, sum, count=0;
						for(n=0;n< ROW ;n++){
							sum = 0;
							for(m=0;m< COLUMN ;m++) {
								sum+= game_field[n][m];
							}
							if(sum== COLUMN){
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
						get_score += 100*count;
						t_mino new_shape = create_mino(mino_set[rand()%7]);
						new_shape.col = rand()%(COLUMN -new_shape.width+1);
						new_shape.row = 0;
						delete_mino(current_mino);
						current_mino = new_shape;
						if(!FunctionCP(current_mino)){
							is_running = FALSE;
						}
					}
					break;
				case 'd':
					temp_mino.col++;
					if(FunctionCP(temp_mino))
						current_mino.col++;
					break;
				case 'a':
					temp_mino.col--;
					if(FunctionCP(temp_mino))
						current_mino.col--;
					break;
				case 'w':
					FunctionRS(temp_mino);
					if(FunctionCP(temp_mino))
						FunctionRS(current_mino);
					break;
			}
			delete_mino(temp_mino);
			FunctionPT();
			gettimeofday(&before_now, NULL);
		}
	}

