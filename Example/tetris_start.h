#pragma once
#pragma once
#ifndef __TETRIS_START_H__
#define __TETRIS_START_H__

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

struct Tetris_START {
	vector<int> ids;// = { 0 }
	vector<vector<float>> ObjTF = GetIdentity(4); // 3 x 4 // R | T in world Coordinate
	vector<vector<float>> CubeLocs; // N x 3
	vector<float> center = { 0, 0, 0 }; // init
	vector<PrimCube>* Base_Cbs;
	//vector<PrimCube>* START_Cbs;
	//float z_dis = 1.15;
	//float xy_dis = 1.15;
	//vector<vector<float>> base_loc = { {  +xy_dis, +xy_dis, -z_dis * 3 },   // 4 x 3 
	//									{ +xy_dis, +xy_dis, -z_dis * 2 },
	//									{ +xy_dis, +xy_dis, -z_dis * 1 },   // 6 Center
	//									{ 0		 , +xy_dis, -z_dis * 1 } }; // 3
	/*vector<vector<float>> base_loc = { {  0, 0, -z_dis * 2 },   // 4 x 3
										{ 0, 0, -z_dis * 1 },
										{ 0, 0, 0 },   // 6 Center
										{ -xy_dis , 0, 0 } }; // 3*/
	vector<vector<float>> base_loc = { { 0, 0, 0 } }; // 3

	int T_num = base_loc.size();
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
	void Reset(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	Tetris_START(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	~Tetris_START() {};
};

#endif // __TETRIS_START_H__