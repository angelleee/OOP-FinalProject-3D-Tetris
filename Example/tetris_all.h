#pragma once
#ifndef __TETRIS_Z_H__
#define __TETRIS_Z_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"

#define TtInitHeight -18
using namespace std;

int NumCubeInTt(int TtType);

struct Tetris_ALL {
	vector<int> ids;// = { 0, 1, 2, 3 }; // {0 ,1 ,2 ,3}
	int type;
	int floor_num;
	int cube_num;
	vector<vector<float>> ObjTF = GetIdentity(4); // 3 x 4 // R | T in world Coordinate
	vector<vector<float>> CubeLocs; // N x 3
	vector<float> center = { 0, 0, 0 }; // init
	//vector<PrimCube>* Base_Cbs;

	vector<vector<float>> base_loc;
	vector<float> tetris_lamb;
	vector<float> tetris_amb;
	
	void SetCubeNum(int cube_type);
	void SetShape(int cube_type);
	void SetFloorNum(int cube_type);
	void SetCenter(vector<float> T_center);
	void SetAmbColor(vector<float> color, vector<PrimCube>& Cbs, int cube_type);
	void SetCubeId(vector<int> cube_ids);
	void SetAmbCoef(float coef, vector<PrimCube>& Cbs);
	void SetLambColor(vector<float> color, vector<PrimCube>& Cbs, int cube_type);
	void SetLambCoef(float coef, vector<PrimCube>& Cbs);
	void AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs);
	//void MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs);
	void MoveTetris(vector<float> T_center, vector<PrimCube>& Cbs);
	void RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs);
	//void SetLocation(vector<vector<float>> loc, vector<PrimCube>& Cbs);
	void SetLocation(vector<float> T_center, vector<PrimCube>& Cbs);
	void SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs);
	vector<vector<float>> GetHintLocation(vector<PrimCube>& Cbs);
	vector<vector<float>> GetTtObjTF(vector<PrimCube>& Cbs);
	void Reset(int cube_type, vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	float GetLowestHeight(vector<PrimCube>& Cbs);
	Tetris_ALL(int cube_type, vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	Tetris_ALL();
	
	~Tetris_ALL() {};
};

#endif // __TETRIS_Z_H__
