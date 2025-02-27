#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"
#include "menu_tetris.h"

using namespace std;
#define CC_Len 0.7

vector<vector<int>> L_ids_all = { {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2, 3}, {0, 1, 2} };

int NumCubeInTt(int TtType) {
	int NumCubeLst[10] = { 4, 4, 4, 3, 3, 4, 3, 4, 3, 9 };

	return NumCubeLst[TtType];
}


Tetris_MENU::Tetris_MENU(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
	SetCenter(T_center);
	SetCubeId(cube_ids);
	SetOrientation(ori, Cbs);
	SetLocation(T_center, Cbs);
	SetLambColor(lamb_color, Cbs);
	SetLambCoef(lamb_coef, Cbs);
	SetAmbColor(amb_color, Cbs);
	SetAmbCoef(amb_coef, Cbs);
	AlterSize(size[0], size[1], size[2], Cbs);
	//SetCubeInsideId(cube_ids);
	//MoveTetris(T_center, Cbs); // Callable
	//RotateTetris(ori, Cbs);    // Callable
}

void Tetris_MENU::Reset(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
	SetCenter(T_center);
	SetCubeId(cube_ids);
	SetOrientation(ori, Cbs);
	SetLocation(T_center, Cbs);
	SetLambColor(lamb_color, Cbs);
	SetLambCoef(lamb_coef, Cbs);
	SetAmbColor(amb_color, Cbs);
	SetAmbCoef(amb_coef, Cbs);
	AlterSize(size[0], size[1], size[2], Cbs);
}

void Tetris_MENU::SetCubeNum(int cube_type) {
	vector<int> cube_num_lst = { 4, 4, 4, 3, 3, 4, 3, 4, 3, 9, 25*5 };
	cube_num = cube_num_lst[cube_type];
}

void Tetris_MENU::SetShape(int cube_type) {

	vector<vector<float>> base_loc_L = { { 0, 0, 0 }, // 6 Center
										 {  0, 0, -CC_Len * 2 },   // 4 x 3 
										 { 0, 0, -CC_Len * 1 },
										 { -CC_Len , 0, 0 } };
	vector<vector<float>> base_loc_Z = { { 0, 0, 0 },
										 {  -CC_Len, 0, -CC_Len * 1 },
										 { 0, 0, -CC_Len * 1 },
										 { -CC_Len , 0, -CC_Len * 2 } };
	vector<vector<float>> base_loc_T = { { 0, 0, 0 },
										 {  0, 0, -CC_Len * 2 },
										 { 0, 0, -CC_Len * 1 },
										 { -CC_Len , 0, -CC_Len * 1 } };
	vector<vector<float>> base_loc_I = { { 0, 0, 0 },
										 {  0, 0, -CC_Len * 2 },
										 { 0, 0, -CC_Len * 1 } };
	vector<vector<float>> base_loc_lv = { { 0, 0, 0 },
										  { -CC_Len, 0, 0 },
										  { 0, 0, -CC_Len * 1 } };
	vector<vector<float>> base_loc_O = { { 0, 0, 0 },
										 { -CC_Len, 0, -CC_Len * 1 },
										 { -CC_Len, 0, 0 },
										 { 0, 0, -CC_Len * 1 } };
	vector<vector<float>> base_loc_rv = { { 0, 0, 0 },
										  { -CC_Len, 0, -CC_Len * 1 },
										  { 0, 0, -CC_Len * 1 } };
	vector<vector<float>> base_loc_lr = { { 0, 0, 0 }, // 6 Center
										 {  0, 0, -CC_Len * 2 },   // 4 x 3 
										 { 0, 0, -CC_Len * 1 },
										 { -CC_Len , 0, -CC_Len * 2 } };
	vector<vector<float>> base_loc_i2 = { { 0, 0, 0 },
										 {  0, 0, -CC_Len * 2 },
										 { 0, 0, -CC_Len * 1 } };
	vector<vector<float>> base_cube_loc = { { -CC_Len, -CC_Len, 0 }, // 9 x 3 
											{   0    , -CC_Len, 0 },
											{ +CC_Len, -CC_Len, 0 },
											{ -CC_Len, 0	  , 0 },
											{ 	0	 , 0      , 0 },
											{ +CC_Len, 0      , 0 },
											{ -CC_Len, +CC_Len, 0 },
											{   0    , +CC_Len, 0 },
											{ +CC_Len, +CC_Len, 0 }
	};
	vector<vector<float>> each_layer = { { -2 * CC_Len, -2 * CC_Len, 0 }, { -1 * CC_Len, -2 * CC_Len, 0 }, { 0, -2 * CC_Len, 0 }, { +1 * CC_Len, -2 * CC_Len, 0 }, { +2 * CC_Len, -2 * CC_Len, 0 },
										 { -2 * CC_Len, -1 * CC_Len, 0 }, { -1 * CC_Len, -1 * CC_Len, 0 }, { 0, -1 * CC_Len, 0 }, { +1 * CC_Len, -1 * CC_Len, 0 }, { +2 * CC_Len, -1 * CC_Len, 0 },
										 { -2 * CC_Len, 0, 0 }, { -1 * CC_Len, 0, 0 }, { 0, 0, 0 }, { +1 * CC_Len, 0, 0 }, { +2 * CC_Len, 0, 0 },
										 { -2 * CC_Len, +1 * CC_Len, 0 }, { -1 * CC_Len, +1 * CC_Len, 0 }, { 0, +1 * CC_Len, 0 }, { +1 * CC_Len, +1 * CC_Len, 0 }, { +2 * CC_Len, +1 * CC_Len, 0 },
										 { -2 * CC_Len, +2 * CC_Len, 0 }, { -1 * CC_Len, +2 * CC_Len, 0 }, { 0, +2 * CC_Len, 0 }, { +1 * CC_Len, +2 * CC_Len, 0 }, { +2 * CC_Len, +2 * CC_Len, 0 }
	};// 25 x 3
	vector<vector<float>> combine;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 25; j++) {
			combine.push_back(each_layer[j]);
			each_layer[j][2] -= CC_Len;
		}
	}
	vector<vector<vector<float>>> base_lst = { base_loc_L, base_loc_Z, base_loc_T, base_loc_I, base_loc_lv, base_loc_O, base_loc_rv, base_loc_lr, base_loc_i2, base_cube_loc, each_layer };
	//base_loc = base_lst[cube_type];
	base_loc = combine;
}



void Tetris_MENU::SetCenter(vector<float> T_center) {
	center = T_center;
}

void Tetris_MENU::SetCubeId(vector<int> cube_ids) {
	ids = cube_ids;
}

void Tetris_MENU::SetAmbColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetAmbColor\n");
	vector<float> tetris_amb_l = { 0, 49, 83 };
	vector<float> tetris_amb_z = { 131, 0, 40 };
	vector<float> tetris_amb_t = { 190, 103, 0 };
	vector<float> tetris_amb_i = { 53, 74, 0 };
	vector<float> tetris_amb_lv = { 127 / 2., 218 / 2., 251 / 2. };
	vector<float> tetris_amb_o = { 255 / 2., 214 / 2., 90 / 2. };
	vector<float> tetris_amb_rv = { 128 / 2., 74 / 2., 150 / 2. };
	vector<float> tetris_amb_lr = { 150, 60, 30 };
	vector<float> tetris_amb_i2 = { 90, 18, 18 };
	vector<float> tetris_cube_amb = { 190, 0, 0 };
	vector<float> menu_cube_amb = {52, 86, 110};
	vector<vector<float>> tetris_amb_lst = { tetris_amb_l, tetris_amb_z, tetris_amb_t, tetris_amb_i, tetris_amb_lv, tetris_amb_o, tetris_amb_rv, tetris_amb_lr, tetris_amb_i2, tetris_cube_amb, menu_cube_amb };

	for (int i = 0; i < base_loc.size(); i++) {
		//Cbs[ids[i]].SetAmbColor(color);
		tetris_amb = tetris_amb_lst[10];
		Cbs[ids[i]].SetAmbColor(tetris_amb);
	}
	//printf("SetAmbColor\n");
}

void Tetris_MENU::SetLambColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetLambColor\n");
	vector<float> tetris_lamb_l = { 47, 82, 143 };
	vector<float> tetris_lamb_z = { 201, 69, 108 };
	vector<float> tetris_lamb_t = { 248, 157, 40 };
	vector<float> tetris_lamb_i = { 83, 124, 10 };
	vector<float> tetris_lamb_lv = { 127, 218, 251 };
	vector<float> tetris_lamb_o = { 255, 214, 90 };
	vector<float> tetris_lamb_rv = { 128, 74, 150 };
	vector<float> tetris_lamb_lr = { 227, 100, 74 };
	vector<float> tetris_lamb_i2 = { 139, 58, 58 };
	vector<float> base_cube_lam = { 255, 0, 0 };//{ 248, 157, 40 };
	vector<float> menu_cube_lam = {112, 146, 190};
	vector<vector<float>> tetris_lamb_lst = { tetris_lamb_l, tetris_lamb_z, tetris_lamb_t, tetris_lamb_i, tetris_lamb_lv, tetris_lamb_o, tetris_lamb_rv,  tetris_lamb_lr, tetris_lamb_i2, base_cube_lam, menu_cube_lam };

	for (int i = 0; i < base_loc.size(); i++) {
		//Cbs[ids[i]].SetLambColor(color);
		tetris_lamb = tetris_lamb_lst[10];
		Cbs[ids[i]].SetLambColor(tetris_lamb);
	}
	//printf("SetLambColor\n");
}

void Tetris_MENU::SetLambCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetLambCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambCoef(coef);
	}
	//printf("SetLambCoef\n");
}

void Tetris_MENU::SetAmbCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetAmbCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbCoef(coef);
	}
	//printf("SetAmbCoef\n");
}

void Tetris_MENU::AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs) {
	//printf("AlterSize\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].AlterSize(sx, sy, sz);
	}
	//printf("AlterSize\n");
}

void Tetris_MENU::SetLocation(vector<float> T_center, vector<PrimCube>& Cbs) {
	//printf("SetLocation1\n");
	ObjTF[3][0] = T_center[0];
	ObjTF[3][1] = T_center[1];
	ObjTF[3][2] = T_center[2];
	vector<float> tmp_T;
	for (int i = 0; i < base_loc.size(); i++) {
		tmp_T = MV_Mul(SubM(ObjTF, 0, 3, 0, 3), base_loc[i]);
		tmp_T[0] += T_center[0];
		tmp_T[1] += T_center[1];
		tmp_T[2] += T_center[2];
		Cbs[ids[i]].SetLocation(tmp_T);
	}
	//printf("SetLocation2\n");
}

void Tetris_MENU::SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // Tetris ObjTF
	for (int i = 0; i < base_loc.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
	}
}

void Tetris_MENU::MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs) {
	ObjTF[3][0] += move_vec[0];
	ObjTF[3][1] += move_vec[1];
	ObjTF[3][2] += move_vec[2];
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].MoveCube(move_vec);
	}
}

void Tetris_MENU::RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	RotMat = MM_Mul(RotMat, SubM(ObjTF, 0, 3, 0, 3));
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3);
	vector<float> tmp_T;
	for (int i = 0; i < base_loc.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
		tmp_T = MV_Mul(SubM(ObjTF, 0, 3, 0, 3), base_loc[i]);
		tmp_T[0] += ObjTF[3][0];
		tmp_T[1] += ObjTF[3][1];
		tmp_T[2] += ObjTF[3][2];
		Cbs[ids[i]].SetLocation(tmp_T);
	}
}

vector<vector<float>> Tetris_MENU::GetTtObjTF(vector<PrimCube>& cubes) {
	vector<vector<float>> Each_ObjTF;
	for (int i = 0; i < base_loc.size(); i++) {
		Each_ObjTF.push_back({ cubes[i].ObjTF[0][3], cubes[i].ObjTF[1][3], cubes[i].ObjTF[2][3] });
	}
	return Each_ObjTF;
}