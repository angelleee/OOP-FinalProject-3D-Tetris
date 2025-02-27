#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "geometry.h"
#include "model.h"
#include "linalg.h"
#include "render.h"
#include "myalgo.h"
#include "polygon_clipping.h"
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "tetris_all.h"
#include "cube_hint.h"
#include "rules.h"


Tetris_ALL add_new_tetris(int tetris_type, vector<PrimCube> &cubes, Tetris &Base){
	int RandLocId, RandOriId, now_hint;
	//int R_init[4] = { 0, 90, 180, 270 };
	vector<int> NewIds = {};
	vector<float> C_len, Ori_Loc, Init_R;
	int NumNewCube = NumCubeInTt(tetris_type);
	for (int i = 0; i < NumNewCube; i++) {
		PrimCube L_cube(cubes.size(), { 0, 0, 1.15 }, { 0, 0, 0 }, { 1.1, 1.1, 1.1 }, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1);
		if (tetris_type == 9) {
			L_cube.deadline = false;
		}
		NewIds.push_back(cubes.size());
		cubes.push_back(L_cube);
	}
	//block_num += L_ids.size();
	//printf("Block Num: %d\n", block_num);
	// Set Random Init T_Loc
	RandOriId = rand() % 4;
	RandLocId = rand() % Base.base_loc.size();
	
	//printf("This is first setting!\n");
	//printf("x=%f, y=%f, R: %f\n", init_x[pick_x], init_y[pick_y], R_init[r]);
	//int cube_type, vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs
	if (tetris_type == 9) {
		C_len = { 1., 1., 0.06 };
		Ori_Loc = { 0, 0, -13.2 + 0.5 };
		Init_R = { 0, 0, 0 };
	}
	else {
		C_len = { 1., 1., 1. };
		Ori_Loc = { cubes[Base.ids[RandLocId]].ObjTF[0][3], cubes[Base.ids[RandLocId]].ObjTF[1][3], TtInitHeight };
		Init_R = { 0, 0, (float)RandOriId * 90 };
	}
	// Select Cube Type
	Tetris_ALL NewTt(tetris_type, NewIds, Ori_Loc, Init_R, C_len, { 0, 0, 0 }, 1, { 0, 0, 0 }, 1, cubes);
	
	//Set_Hint_Location(NewTt.ids, cubes, hint_idx);
	//now_hint = count_hint_num(hint_idx);
	if (tetris_type != 9){
		while (!IsTtInsideBase(cubes, NewTt, Base)) { // make sure init is in base
			//printf("This is Re setting!\n");
			//printf("now_hint=%d, floor_numt=%d\n", now_hint, Cube_type.floor_num);
			//for (int i = 0; i < 9; i++) {
			//	printf("%d ", hint_idx[i]);
			//}
			//NewTt.~Tetris_ALL();
			RandOriId = rand() % 4;
			RandLocId = rand() % Base.base_loc.size();
			//printf("This is Re setting!\n");
			//printf("x=%f, y=%f, R: %f\n", init_x[pick_x], init_y[pick_y], R_init[r]);
			NewTt.Reset(tetris_type, NewIds, { cubes[Base.ids[RandLocId]].ObjTF[0][3], cubes[Base.ids[RandLocId]].ObjTF[1][3], TtInitHeight },
				{ 0, 0, (float)RandOriId * 90 }, { 1., 1., 1. }, { 0, 0, 0 }, 1, { 0, 0, 0 }, 1, cubes);
		}
	}
	return NewTt;
}

bool IsTtInsideBase(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base) {
	vector<vector<float>> HintLocs, BaseLocs;
	HintLocs = Tt.GetHintLocation(cubes);
	BaseLocs = Base.GetBaseLocsInWC(cubes);
	bool IsHintInside;
	for (int i = 0; i < HintLocs.size(); i++) {
		IsHintInside = false;
		for (int j = 0; j < BaseLocs.size(); j++) {
			if (abs(HintLocs[i][0] - BaseLocs[j][0]) < 0.1 && abs(HintLocs[i][1] - BaseLocs[j][1]) < 0.1) {
				IsHintInside = true;
			}
		}
		if (IsHintInside == false) {
			return false;
		}
	}
	return true;
}

void RenewHintLoc(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base, int collect_or_not, vector<vector<float>> &Base_Now_Height, vector<vector<float>> &hint_location) {
	hint_location = Tt.GetHintLocation(cubes);
	if (collect_or_not == 0) {
		Base.Find_Hint_Height(cubes, hint_location, Base.ids.size());
		Base_Now_Height = Base.Find_All_Height(cubes, Base.ids.size());
	}
	else {
		Base.Find_Hint_Height(cubes, hint_location, Base.ids.size() - Tt.ids.size());
		Base_Now_Height = Base.Find_All_Height(cubes, Base.ids.size() - Tt.ids.size());
	}
}

void Check_Deadline_Block(vector<PrimCube>& cubes, vector<vector<float>>& Base_Now_Height) {
	for (int k = 9; k <= 17; k++) {
		cubes[k].deadline = false;
	}
	for (int j = 0; j < Base_Now_Height.size(); j++) {
		//if (Base_Now_Height[j][2] < -10.7) {
		//if (Base_Now_Height[j][2] < -9.5) {
		if (Base_Now_Height[j][2] < -8.3) {
			//printf("idx %d, x = %f, y = %f, z = %f\n", j, Base_Now_Height[j][0], Base_Now_Height[j][1], Base_Now_Height[j][2]);
			for (int k = 9; k <= 17; k++) {
				if (abs(Base_Now_Height[j][0] - cubes[k].ObjTF[0][3]) < 0.1 && abs(Base_Now_Height[j][1] - cubes[k].ObjTF[1][3]) < 0.1) {
					cubes[k].deadline = true;
				}
			}
		}
	}
	
}

void Game_Over_Or_Not(bool &Game_over, int Game_Score, vector<vector<float>>& Base_Now_Height) {
	for (int i = 0; i < Base_Now_Height.size(); i++) {
		if (Base_Now_Height[i][2] < -13.0) {
			Game_over = true;
		}
	}
	if (Game_over == true) {
		printf("Game Over !!!\n");
		printf("Final Score: %d!\n", Game_Score);
		//Game_over = false;
	}
}



//int GetCollideType(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base) {
//	int collide = 0;
//	float NeighborDis = C2CDistance + 0.1;
//	float MatchTolerance = 0.1;
//	float z_dis, y_dis, x_dis;
//	for (int i = 0; i < Tt.ids.size(); i++) {
//		cubes[Tt.ids[i]];
//		for (int j = 0; j < Base.ids.size(); j++) {
//			z_dis = abs(cubes[Tt.ids[i]].ObjTF[2][3] - cubes[Base.ids[i]].ObjTF[2][3]);
//			if (z_dis < NeighborDis) {
//				y_dis = abs(cubes[Tt.ids[i]].ObjTF[1][3] - cubes[Base.ids[i]].ObjTF[1][3]);
//				if (y_dis < NeighborDis) {
//					x_dis = abs(cubes[Tt.ids[i]].ObjTF[0][3] - cubes[Base.ids[i]].ObjTF[0][3]);
//					if (x_dis < NeighborDis) {
//						if (z_dis < MatchTolerance) {
//							if x_dis < NeighborDis&& y_dis < NeighborDis
//						}
//					}
//				}
//			}
//		}
//	}
//}