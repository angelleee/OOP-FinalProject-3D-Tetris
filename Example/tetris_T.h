#pragma once
#ifndef __TETRIS_T_H__
#define __TETRIS_T_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"

using namespace std;

struct Tetris_T {
	vector<int> ids;// = { 0, 1, 2, 3 }; // {0 ,1 ,2 ,3}
	//vector<int> ids_in = vector<int>(4);
	vector<vector<float>> ObjTF = GetIdentity(4); // 3 x 4 // R | T in world Coordinate
	vector<vector<float>> CubeLocs; // N x 3
	vector<float> center = { 0, 0, 0 }; // init
	vector<PrimCube>* Base_Cbs;
	float z_dis = 1.2;
	float xy_dis = 1.2;
	vector<float> tetris_lamb = { 248, 157, 40 };
	vector<float> tetris_amb = { 190, 103, 0 };
	vector<vector<float>> base_loc = { {  0, 0, -z_dis * 2 },   // 4 x 3 
										{ 0, 0, -z_dis * 1 },
										{ 0, 0, 0 },   // 6 Center
										{ -xy_dis , 0, -z_dis * 1 } }; // 3
	int T_num = base_loc.size();
	int floor_num = 2;
	//void SetCubeInsideId(vector<int> cube_ids);
	//void SetCubes(vector<PrimCube>& Cbs);
	void SetCenter(vector<float> T_center);
	void SetAmbColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetCubeId(vector<int> cube_ids);
	void SetAmbCoef(float coef, vector<PrimCube>& Cbs);
	void SetLambColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetLambCoef(float coef, vector<PrimCube>& Cbs);
	void AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs);
	//void MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs);
	void MoveTetris(vector<float> T_center, vector<PrimCube>& Cbs);
	void RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs);
	//void SetLocation(vector<vector<float>> loc, vector<PrimCube>& Cbs);
	void SetLocation(vector<float> T_center, vector<PrimCube>& Cbs);
	void SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs);
	Tetris_T(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	~Tetris_T() {};
};

#endif // __TETRIS_T_H__

