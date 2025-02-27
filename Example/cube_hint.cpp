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
#include "tetris_all.h"



void Set_Hint_Location(vector<int>& now_T, vector<PrimCube>& cubes, vector<int>& hint_idx, int &now_id) {
	float loc_x, loc_y, loc_z;
	for (int i = 0; i < 9; i++) {
		hint_idx[i] = 0;
	}
	//int now_id = now_T[0];
	printf("Set Hint Loc: now_id = %d\n", now_id);
	for (int i = now_id; i < cubes.size(); i++) {
		loc_x = cubes[i].ObjTF[0][3];
		loc_y = cubes[i].ObjTF[1][3];
		loc_z = cubes[i].ObjTF[2][3];
		//printf("Now x=%f, y=%f, z=%f\n", loc_x, loc_y, loc_z);
		if (abs(loc_y - (float)1.2) < 0.1) {
			if (abs(loc_x - (float)1.2) < 0.1) {
				if (hint_idx[0] == 0) {
					//printf("I am 0\n");
					hint_idx[0] = 1;
				}
			}
			else if (abs(loc_x - (float)0.0) < 0.1) {
				if (hint_idx[1] == 0) {
					//printf("I am 1\n");
					hint_idx[1] = 1;
				}
			}
			else if (abs(loc_x - (-(float)1.2)) < 0.1) {
				if (hint_idx[2] == 0) {
					//printf("I am 2\n");
					hint_idx[2] = 1;
				}
			}
		}
		else if (abs(loc_y - (float)0.0) < 0.1) {
			if (abs(loc_x - (float)1.2) < 0.1) {
				if (hint_idx[3] == 0) {
					//printf("I am 3\n");
					hint_idx[3] = 1;
				}
			}
			else if (abs(loc_x - (float)0.0) < 0.1) {
				if (hint_idx[4] == 0) {
					//printf("I am 4\n");
					hint_idx[4] = 1;
				}
			}
			else if (abs(loc_x - (-(float)1.2)) < 0.1) {
				if (hint_idx[5] == 0) {
					//printf("I am 5\n");
					hint_idx[5] = 1;
				}
			}
		}
		else if (abs(loc_y - (-(float)1.2)) < 0.1) {
			if (abs(loc_x - (float)1.2) < 0.1) {
				if (hint_idx[6] == 0) {
					//printf("I am 6\n");
					hint_idx[6] = 1;
				}
			}
			else if (abs(loc_x - (float)0.0) < 0.1) {
				if (hint_idx[7] == 0) {
					//printf("I am 7\n");
					hint_idx[7] = 1;
				}
			}
			else if (abs(loc_x - (-(float)1.2)) < 0.1) {
				if (hint_idx[8] == 0) {
					//printf("I am 8\n");
					hint_idx[8] = 1;
				}
			}
		}
	}
	/*printf("IDX: ");
	for (int i = 0; i < 9; i++) {
		printf("%d ", hint_idx[i]);
	}
	printf("\n");*/
}

void Find_Hint_Height(vector<int>& hint_idx, vector<int>& now_T, vector<PrimCube>& base_cubes, vector<vector<float>>& hint_location, vector<float>& hint_height, vector<vector<float>>& hint_verts, int &now_id) {
	float tmp_x, tmp_y, tmp_z;
	int max_idx;
	//int now_id = now_T[0];
	// init H
	for (int i = 0; i < 9; i++) {
		hint_height[i] = 1.2;
	}
	hint_verts.clear();
	for (int k = 0; k < 9; k++) {
		if (hint_idx[k] == 1) {
			float now_h = hint_height[k];
			//printf("Compare: x = %f, y = %f\n;", hint_location[k][0], hint_location[k][1]);
			max_idx = 0;
			for (int i = now_id - 1; i >= 0; i--) {
				//printf("idx: %d\n", i);
				tmp_x = base_cubes[i].ObjTF[0][3];
				tmp_y = base_cubes[i].ObjTF[1][3];
				//printf("Each cbs: tmp_x = %f, tmp_y = %f, tmp_z = %f\n", tmp_x, tmp_y, base_cubes[i].ObjTF[2][3]);
				//if (tmp_x == hint_location[k][0] && tmp_y == hint_location[k][1]) {
				if (abs(tmp_x - hint_location[k][0]) < 0.1 && abs(tmp_y - hint_location[k][1]) < 0.1) {
					tmp_z = base_cubes[i].ObjTF[2][3];
					//printf("QAQ   tmp_x = %f, tmp_y = %f, tmp_z = %f\n", tmp_x, tmp_y, tmp_z);
					if (tmp_z <= now_h) {
						hint_height[k] = tmp_z;
						now_h = tmp_z;
						//printf("Max Idx: %d\n", max_idx);
						max_idx = i;
					}
				}
			}
			//printf("Max Idx: %d\n", max_idx);
			hint_verts.push_back({ hint_location[k][0], hint_location[k][1], now_h , (float)max_idx });
		}
	}
	//printf("==========Find_Done!===========\n");
}

void printf_hint_info(vector<vector<float>>& hint_verts, vector<int>& hint_idx, vector<float>& hint_height) {
	for (int i = 0; i < hint_verts.size(); i++) {
		printf("hint_verts: v_x = %f, v_y = %f, v_z = %f\n", hint_verts[i][0], hint_verts[i][1], hint_verts[i][2]);
	}
	for (int i = 0; i < 9; i++) {
		printf("%d ", hint_idx[i]);
	}
	printf("\n");
	for (int i = 0; i < 9; i++) {
		if (hint_idx[i] == 1) {
			printf("loc idx: %d, max_h = %f\n", i, hint_height[i]);
		}
	}
}

int count_hint_num(vector<int>& hint_idx) {
	int c = 0;
	for (int i = 0; i < 9; i++) {
		if (hint_idx[i] == 1)
			c += 1;
	}
	return c;
}