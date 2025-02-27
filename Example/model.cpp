#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "model.h"
#include "geometry.h"
#include "linalg.h"
#include "render.h"

using namespace std;

PrimCube::PrimCube(int id, vector<float> loc, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef) { // init
	SetCubeId(id);
	SetLocation(loc);
	SetOrientation(ori);
	SetLambColor(lamb_color);
	SetLambCoef(lamb_coef);
	SetAmbColor(amb_color);
	SetAmbCoef(amb_coef);
	AlterSize(size[0], size[1], size[2]);
	GetVertxLoc();
}

void PrimCube::SetCubeId(int id) {
	cube_id = id;
}

void PrimCube::SetAmbColor(vector<float> color) {
	amb_color = color;
}

void PrimCube::SetAmbCoef(float coef) {
	amb_coef = coef;
}

void PrimCube::SetLambColor(vector<float> color) {
	lamb_color = color;
}

void PrimCube::SetLambCoef(float coef) {
	lamb_coef = coef;
}

void PrimCube::AlterSize(float sx, float sy, float sz) {
	for (int i = 0; i < verts.size(); i++) {
		verts[i][0] *= sx;
		verts[i][1] *= sy;
		verts[i][2] *= sz;
	}
}

void PrimCube::SetLocation(vector<float> loc) {
	ObjTF[0][3] = loc[0];
	ObjTF[1][3] = loc[1];
	ObjTF[2][3] = loc[2];
}

void PrimCube::SetOrientation(vector<float> euler_ang) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3);
}

void PrimCube::MoveCube(vector<float> move_vec) {
	ObjTF[0][3] += move_vec[0];
	ObjTF[1][3] += move_vec[1];
	ObjTF[2][3] += move_vec[2];
}

void PrimCube::RotateCube(vector<float> euler_ang) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	RotMat = MM_Mul(RotMat, SubM(ObjTF, 0, 3, 0, 3));
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3);
}

void PrimCube::SetDis2Light(float distance) {
	Dis2Light = distance;
}

void PrimCube::SetDis2Cam(float distance) {
	Dis2Cam = distance;
}

void PrimCube::ResetShadow() {  // 6 face, k shadow, e ploy_points, 2 xy_points
	Clear4DVector(tmp_shadow);
}

void PrimCube::GetVertxLoc() {
	for (int i = 0; i < verts.size(); i++) {
		new_verts.push_back({ verts[i][0] + ObjTF[0][3],  verts[i][1] + ObjTF[1][3], verts[i][2] + ObjTF[2][3]});
	}
}