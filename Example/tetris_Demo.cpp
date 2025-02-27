#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"
#include "tetris_Demo.h"

using namespace std;

Tetris_DEMO::Tetris_DEMO(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
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

void Tetris_DEMO::SetCenter(vector<float> T_center) {
	center = T_center;
}

void Tetris_DEMO::SetCubeId(vector<int> cube_ids) {
	ids = cube_ids;
}

void Tetris_DEMO::SetAmbColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetAmbColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbColor(color);
	}
	//printf("SetAmbColor\n");
}

void Tetris_DEMO::SetLambColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetLambColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		//Cbs[ids[i]].SetLambColor(color);
		Cbs[ids[i]].SetLambColor(demo_color[i]);
	}
	//printf("SetLambColor\n");
}

void Tetris_DEMO::SetLambCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetLambCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambCoef(coef);
	}
	//printf("SetLambCoef\n");
}

void Tetris_DEMO::SetAmbCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetAmbCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbCoef(coef);
	}
	//printf("SetAmbCoef\n");
}

void Tetris_DEMO::AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs) {
	//printf("AlterSize\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].AlterSize(sx, sy, sz);
	}
	//printf("AlterSize\n");
}

void Tetris_DEMO::SetLocation(vector<float> T_center, vector<PrimCube>& Cbs) {
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

void Tetris_DEMO::SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // Tetris ObjTF
	for (int i = 0; i < base_loc.size(); i++) {
		/*printf("R Size1: %d\n", RotMat.size());
		printf("R Size2: %d\n", RotMat[0].size());
		printf("Obj Size1: %d\n", Cbs[ids[i]].ObjTF.size());
		printf("Obj Size2: %d\n", Cbs[ids[i]].ObjTF[0].size());*/
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
	}
}

void Tetris_DEMO::MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs) {
	ObjTF[3][0] += move_vec[0];
	ObjTF[3][1] += move_vec[1];
	ObjTF[3][2] += move_vec[2];
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].MoveCube(move_vec);
	}
}

void Tetris_DEMO::RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs) {
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