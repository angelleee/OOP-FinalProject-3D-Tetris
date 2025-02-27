#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "geometry.h"
#include "linalg.h"
#include "model.h"
#include "tetris.h"
#include "rules.h"

using namespace std;

void ReInitGame(bool & Game_over, int & ReStart, int& Game_Score, float& tt, float& CR, int &collect_or_not, float & Cube_Lowest, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris_ALL &Base_deadline, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location) {
	printf("Restart!\n");
	Game_Score = 0;
	tt = 0;
	CR = 0;
	collect_or_not = 0;

	cubes.clear();
	cubes.shrink_to_fit();
	Base_Now_Height.clear();
	Base_Now_Height.shrink_to_fit();

	for (int i = 0; i < 9; i++) {
		PrimCube base_cube(i, { -1.2, -1.2, 1.3 }, { 0, 0, 0 }, { 1.1, 1.1, 1.1 }, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1);
		cubes.push_back(base_cube);
	}

	// Set Base Attribute
	Base.Reset({ 0, 1, 2, 3, 4, 5, 6, 7, 8 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1, cubes);
	// Set Deadline Block
	Base_deadline = add_new_tetris(9, cubes, Base);
	Base.Collect_Ids(Base_deadline.ids);

	// Set Random Init T_Loc
	int tetris_type = rand() % 9;
	//printf("T Type: %d\n", tetris_type);
	Tt = add_new_tetris(tetris_type, cubes, Base);

	hint_location = Tt.GetHintLocation(cubes);
	Base.Find_Hint_Height(cubes, hint_location, Base.ids.size());
	Base_Now_Height = Base.Find_All_Height(cubes, Base.ids.size());
	Cube_Lowest = Tt.GetLowestHeight(cubes);

	Game_over = false;
	ReStart = 0;
}