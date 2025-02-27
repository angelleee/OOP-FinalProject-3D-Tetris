#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"
#include "tetris_T.h"

using namespace std;

Tetris_T::Tetris_T(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
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

void Tetris_T::SetCenter(vector<float> T_center) {
	center = T_center;
}

void Tetris_T::SetCubeId(vector<int> cube_ids) {
	ids = cube_ids;
}

void Tetris_T::SetAmbColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetAmbColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		//Cbs[ids[i]].SetAmbColor(color);
		Cbs[ids[i]].SetAmbColor(tetris_amb);
	}
	//printf("SetAmbColor\n");
}

void Tetris_T::SetLambColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetLambColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		//Cbs[ids[i]].SetLambColor(color);
		Cbs[ids[i]].SetLambColor(tetris_lamb);
	}
	//printf("SetLambColor\n");
}

void Tetris_T::SetLambCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetLambCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambCoef(coef);
	}
	//printf("SetLambCoef\n");
}

void Tetris_T::SetAmbCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetAmbCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbCoef(coef);
	}
	//printf("SetAmbCoef\n");
}

void Tetris_T::AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs) {
	//printf("AlterSize\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].AlterSize(sx, sy, sz);
	}
	//printf("AlterSize\n");
}

void Tetris_T::SetLocation(vector<float> T_center, vector<PrimCube>& Cbs) {
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

void Tetris_T::SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // Tetris ObjTF
	for (int i = 0; i < base_loc.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
	}
}

void Tetris_T::MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs) {
	ObjTF[3][0] += move_vec[0];
	ObjTF[3][1] += move_vec[1];
	ObjTF[3][2] += move_vec[2];
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].MoveCube(move_vec);
	}
}

void Tetris_T::RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs) {
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