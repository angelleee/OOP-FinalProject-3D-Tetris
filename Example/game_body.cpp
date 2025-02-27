#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "linalg.h"
#include "model.h"
#include "tetris.h"
#include "geometry.h"
#include "render.h"
#include "polygon_clipping.h"
#include "cube_hint.h"
#include "rules.h"

void GameMainBody(bool& Game_over, int& Game_Score, int& collect_or_not, float& Cube_Lowest, Tetris& Base, Tetris_ALL& Tt, vector<PrimCube>& cubes, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location) {
	// Fall to Base and Become to Base
	if (collect_or_not == 0) {
		Cube_Lowest = Tt.GetLowestHeight(cubes);
		for (int j = 0; j < Base_Now_Height.size(); j++) {
			if (Base_Now_Height[j][2] - 1.6 < Cube_Lowest) { // if Tt is too low, become to base
				Base.Collect_Ids(Tt.ids);
				collect_or_not = 1;
				break;
			}
		}
	}
	Check_Deadline_Block(cubes, Base_Now_Height);

	for (int i = 0; i < hint_location.size(); i++) {
		if (abs(hint_location[i][3] - hint_location[i][2]) < 1.3) {
			if (collect_or_not == 0) {
				Base.Collect_Ids(Tt.ids);
			}
			else {
				collect_or_not = 0;
			}
			Game_Score += Tt.ids.size();

			Base.CheckDelete(cubes, Base, Game_Score);
			Base_Now_Height = Base.Find_All_Height(cubes, Base.ids.size());

			Game_Over_Or_Not(Game_over, Game_Score, Base_Now_Height);
			printf("Score: %d\n", Game_Score);
			if (Game_over == false) {
				int tetris_type = rand() % 9;
				Tt = add_new_tetris(tetris_type, cubes, Base);
				hint_location = Tt.GetHintLocation(cubes);
				Base.Find_Hint_Height(cubes, hint_location, Base.ids.size());
				//printf("Add One!\n");
			}
		}
	}
}

void Score2Text(int& Game_Score, char* score) {
	int s, count;
	//char score[10] = "\0";
	s = Game_Score;
	count = 0;
	while (s > 0)
	{
		s = s / 10;
		count++;
	}
	s = Game_Score;
	for (int i = (count - 1); i >= 0; i--)
	{
		score[i] = (s % 10) + 48;
		s /= 10;
	}
	score[count] = '\0';
	//return score;
}