#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"
#include "tetris_menu.h"

using namespace std;

Tetris_Menu::Tetris_Menu(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
	SetCenter(T_center);
	SetCubeId(cube_ids);
	SetLocation(T_center, Cbs);
	SetOrientation(ori, Cbs);
	SetLambColor(lamb_color, Cbs);
	SetLambCoef(lamb_coef, Cbs);
	SetAmbColor(amb_color, Cbs);
	SetAmbCoef(amb_coef, Cbs);
	AlterSize(size[0], size[1], size[2], Cbs);
}

void Tetris_Menu::Reset(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
	SetCenter(T_center);
	SetCubeId(cube_ids);
	SetLocation(T_center, Cbs);
	SetOrientation(ori, Cbs);
	SetLambColor(lamb_color, Cbs);
	SetLambCoef(lamb_coef, Cbs);
	SetAmbColor(amb_color, Cbs);
	SetAmbCoef(amb_coef, Cbs);
	AlterSize(size[0], size[1], size[2], Cbs);
}

void Tetris_Menu::Collect_Ids(const vector<int>& drop_ids) {
	for (int i = 0; i < drop_ids.size(); i++) {
		ids.push_back(drop_ids[i]);
	}
}

void Tetris_Menu::SetCenter(vector<float> T_center) {
	center = T_center;
}

void Tetris_Menu::SetCubeId(vector<int> cube_ids) {
	ids = cube_ids;
}

void Tetris_Menu::SetAmbColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetAmbColor\n");
	Cbs[0].SetAmbColor(color);
	/*for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbColor(color);
	}*/

	for (int i = 1; i < 10; i++) { //T 9
		Cbs[ids[i]].SetAmbColor({ 90 , 0, 0 });
	}
	for (int i = 10; i < 23; i++) { //E 13
		Cbs[ids[i]].SetAmbColor({ 100 , 150, 0 });
	}
	for (int i = 23; i < 32; i++) { //T 9
		Cbs[ids[i]].SetAmbColor({ 90 , 0, 0 });
	}
	for (int i = 32; i < 44; i++) { //R 12
		Cbs[ids[i]].SetAmbColor({ 100 , 90, 0 });
	}
	for (int i = 44; i < 54; i++) { //I 10
		Cbs[ids[i]].SetAmbColor({ 220 , 110, 0 });
	}
	for (int i = 54; i < 67; i++) { //S 13
		Cbs[ids[i]].SetAmbColor({ 0 , 66, 66 });
	}
	
	for (int i = 67; i < base_loc.size(); i++) { //S 13
		Cbs[ids[i]].SetAmbColor({ 0 , 0, (float)255 - 50*(i-67) });
	}

	//printf("SetAmbColor\n");
}

void Tetris_Menu::SetLambColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetLambColor\n");
	Cbs[0].SetLambColor(color);
	for (int i = 1; i < 10; i++) { //T 9
		Cbs[ids[i]].SetLambColor({255 , 0, 0});
	}
	for (int i = 10; i < 23; i++) { //E 13
		Cbs[ids[i]].SetLambColor({ 181 , 230, 29 });
	}
	for (int i = 23; i < 32; i++) { //T 9
		Cbs[ids[i]].SetLambColor({ 255 , 0, 0 });
	}
	for (int i = 32; i < 44; i++) { //R 12
		Cbs[ids[i]].SetLambColor({ 255 , 242, 0 });
	}
	for (int i = 44; i < 54; i++) { //I 10
		Cbs[ids[i]].SetLambColor({ 254 , 145, 30 }); //{ 254 , 125, 39 }
	}
	for (int i = 54; i < base_loc.size(); i++) { //S 13
		Cbs[ids[i]].SetLambColor({ 0 , 255, 255 });
	}
	//printf("SetLambColor\n");
}

void Tetris_Menu::SetLambCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetLambCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambCoef(coef);
	}
	//printf("SetLambCoef\n");
}

void Tetris_Menu::SetAmbCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetAmbCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbCoef(coef);
	}
	//printf("SetAmbCoef\n");
}

void Tetris_Menu::AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs) {
	//printf("AlterSize\n");
	sx = 0.5, sy = 0.5, sz = 0.5;
	Cbs[0].AlterSize(5, 5, 5);
	for (int i = 1; i < base_loc.size(); i++) {
		Cbs[ids[i]].AlterSize(sx, sy, sz);
	}
	//printf("AlterSize\n");
}

void Tetris_Menu::SetLocation(vector<float> T_center, vector<PrimCube>& Cbs) {
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

void Tetris_Menu::SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // Tetris ObjTF
	for (int i = 0; i < base_loc.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
	}
}

void Tetris_Menu::MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs) {
	ObjTF[3][0] += move_vec[0];
	ObjTF[3][1] += move_vec[1];
	ObjTF[3][2] += move_vec[2];
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].MoveCube(move_vec);
	}
}

void Tetris_Menu::RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat_ori = Euler2Mat(euler_ang);
	vector<vector<float>> RotMat = MM_Mul(RotMat_ori, SubM(ObjTF, 0, 3, 0, 3));

	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // 80 + 10 = 90
	vector<float> tmp_T;

	for (int i = 0; i < ids.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3); // 80 + 10 = 90
		//tmp_T = MV_Mul(SubM(ObjTF, 0, 3, 0, 3), base_loc[i]); // 
		Cbs[ids[i]].ObjTF[0][3] = Cbs[ids[i]].ObjTF[0][3] - ObjTF[0][3];
		Cbs[ids[i]].ObjTF[1][3] = Cbs[ids[i]].ObjTF[1][3] - ObjTF[1][3];
		Cbs[ids[i]].ObjTF[2][3] = Cbs[ids[i]].ObjTF[2][3] - ObjTF[2][3];
		tmp_T = MV_Mul(RotMat_ori, { Cbs[ids[i]].ObjTF[0][3], Cbs[ids[i]].ObjTF[1][3], Cbs[ids[i]].ObjTF[2][3] }); // T_ori 80 + 10 R_ori
		//tmp_T = MV_Mul(SubM(ObjTF, 0, 3, 0, 3), { Cbs[ids[i]].ObjTF[0][3], Cbs[ids[i]].ObjTF[1][3], Cbs[ids[i]].ObjTF[2][3] });
		tmp_T[0] += ObjTF[0][3];
		tmp_T[1] += ObjTF[1][3];
		tmp_T[2] += ObjTF[2][3];
		Cbs[ids[i]].SetLocation(tmp_T);
	}
}