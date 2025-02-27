#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#define UNIT_CUBE_LEN 0.5
#define C2CDistance 1.2
using namespace std;

struct PrimCube {
	int cube_id;
	float Dis2Light;
	float Dis2Cam;
	vector<vector<float>> ObjTF = GetIdentity(4);
	bool hint = false;
	bool deadline = false;
	vector<float> lamb_color = { 152, 200, 204 };
	vector<float> amb_color = { 0, 51, 51 };
	float lamb_coef = 1.0;
	float amb_coef = 1.0;

	vector<vector<float>> verts = {
		{ -UNIT_CUBE_LEN, -UNIT_CUBE_LEN, UNIT_CUBE_LEN },
		{ UNIT_CUBE_LEN, -UNIT_CUBE_LEN, UNIT_CUBE_LEN },
		{ UNIT_CUBE_LEN, -UNIT_CUBE_LEN, -UNIT_CUBE_LEN },
		{ -UNIT_CUBE_LEN, -UNIT_CUBE_LEN, -UNIT_CUBE_LEN },
		{ -UNIT_CUBE_LEN, UNIT_CUBE_LEN, UNIT_CUBE_LEN },
		{ UNIT_CUBE_LEN, UNIT_CUBE_LEN, UNIT_CUBE_LEN },
		{ UNIT_CUBE_LEN, UNIT_CUBE_LEN, -UNIT_CUBE_LEN },
		{ -UNIT_CUBE_LEN, UNIT_CUBE_LEN, -UNIT_CUBE_LEN }
	}; // 8 vertices x 3 (x, y, z)

	vector<vector<float>> new_verts;
	//vector<vector<float>> verts_2d = ZeroInitMat(8, 2); // 8 vertices x 2 (x, y)

	//       -z
	//       ^
	//       |
	//       |
	//       3 - - - 2
	//      /|      /|
	//     / |     / |
	//    7 -|- - 6  |
	//    |  0 - -|- 1 - - > x
	//    | /|    | / 
	//    |/ |    |/
	//    4 -|- - 5
	//   /   |
	//  /    |
	// y     z        P.S. (0, 0) is at Cube Center

	vector<vector<int>> faces{ {0, 1, 2, 3},
							   {0, 4, 5, 1},
							   {0, 3, 7, 4},
							   {1, 5, 6, 2}, // 
							   {4, 7, 6, 5}, //
							   {2, 6, 7, 3} }; // 6 x 4 (Vertex ID)

	vector<vector<float>> normals{
		{ 0, -1,  0},
		{ 0,  0,  1},
		{-1,  0,  0},
		{ 1,  0,  0},
		{ 0,  1,  0},
		{ 0,  0, -1}
	}; // 6 x 3 (6 faces, [Nx, Ny, Nz])

	vector<vector<vector<vector<float>>>> tmp_shadow; // = { {}, {}, {}, {}, {}, {} }; // = 6 x N x K x 2 (6 faces, N(30) shadows, K-vertices polygon(10), [x, y])
	float tmp_shadow_static[6][30][10][2] = { 0 };
	int shadow_num[6] = {0}; // shadow_num, shadow_polygon_num
	int shadow_poly_num[6][30] = {0}; // save each polygon num
	void SetAmbColor(vector<float> color);
	void SetCubeId(int id);
	void SetAmbCoef(float coef);
	void SetLambColor(vector<float> color);
	void SetLambCoef(float coef);
	void AlterSize(float sx, float sy, float sz);
	void MoveCube(vector<float> move_vec);
	void RotateCube(vector<float> euler_ang);
	void SetLocation(vector<float> loc);
	void SetOrientation(vector<float> euler_ang);
	void SetDis2Light(float distance);
	void SetDis2Cam(float distance);
	void ResetShadow();
	void GetVertxLoc();
	PrimCube() {};
	PrimCube(int id, vector<float> loc, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef);
	~PrimCube() {};
};

#endif // __MODEL_H__