#ifndef __TETRIS_H__
#define __TETRIS_H__

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

struct Tetris {
	vector<int> ids;// = { 0, 1, 2, 3 }; // {0 ,1 ,2 ,3}
	vector<vector<float>> ObjTF = GetIdentity(4);; // 3 x 4 // R | T in world Coordinate
	vector<vector<float>> CubeLocs; // N x 3
	vector<float> center = {0, 0, 0}; // init
	vector<vector<float>> base_loc = {  { -C2CDistance, -C2CDistance, + C2CDistance }, // 9 x 3 
										{   0    , -C2CDistance, +C2CDistance },
										{ +C2CDistance, -C2CDistance, +C2CDistance },
										{ -C2CDistance, 0	  , +C2CDistance },
										{ 	0	 , 0      , +C2CDistance },
										{ +C2CDistance, 0      , +C2CDistance },
										{ -C2CDistance, +C2CDistance, +C2CDistance },
										{   0    , +C2CDistance, +C2CDistance },
										{ +C2CDistance, +C2CDistance, +C2CDistance }  };

	int T_num = base_loc.size();
	//void Collect_Loc(const vector<int>& drop_loc);
	void Collect_Ids(const vector<int>& drop_ids);
	void SetCenter(vector<float> T_center);
	void SetAmbColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetCubeId(vector<int> cube_ids);
	void SetAmbCoef(float coef, vector<PrimCube>& Cbs);
	void SetLambColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetLambCoef(float coef, vector<PrimCube>& Cbs);
	void AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs);
	void MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs);
	void RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs);
	//void SetLocation(vector<vector<float>> loc, vector<PrimCube>& Cbs);
	void SetLocation(vector<float> T_center, vector<PrimCube>& Cbs);
	void SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs);
	void Find_Hint_Height(vector<PrimCube>& Cbs, vector<vector<float>>& hint_location, int range);
	vector<vector<float>> Find_All_Height(vector<PrimCube>& Cbs, int range);
	vector<vector<float>> GetBaseLocation(vector<PrimCube>& Cbs);
	vector<vector<float>> GetBaseLocsInWC(vector<PrimCube>& Cbs);
	vector<vector<int>> GetBaseEachLayerIds(vector<PrimCube>& cubes);
	int GetDeleteLayerIds(vector<PrimCube>& cubes, vector<int> &one_layer);
	void CubeHShift(vector<vector<int>>& layer_ids, int idx, vector<PrimCube>& cubes);
	void CubeErase(vector<int> delete_layer_vec, vector<PrimCube>& cubes, Tetris& Base);
	void CheckDelete(vector<PrimCube>& cubes, Tetris& Base, int& Game_Score);
	void Reset(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	Tetris(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	~Tetris() {};
};

#endif // __TETRIS_H__