#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"
#include "tetris.h"

using namespace std;

Tetris::Tetris(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
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

void Tetris::Reset(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs) {
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

void Tetris::Collect_Ids(const vector<int> & drop_ids) {
	for (int i = 0; i < drop_ids.size(); i++) {
		ids.push_back(drop_ids[i]);
	}
}

void Tetris::SetCenter(vector<float> T_center) {
	center = T_center;
}

void Tetris::SetCubeId(vector<int> cube_ids) {
	ids = cube_ids;
}

void Tetris::SetAmbColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetAmbColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbColor(color);
	}
	//printf("SetAmbColor\n");
}

void Tetris::SetLambColor(vector<float> color, vector<PrimCube>& Cbs) {
	//printf("SetLambColor\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambColor(color);
	}
	//printf("SetLambColor\n");
}

void Tetris::SetLambCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetLambCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetLambCoef(coef);
	}
	//printf("SetLambCoef\n");
}

void Tetris::SetAmbCoef(float coef, vector<PrimCube>& Cbs) {
	//printf("SetAmbCoef\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].SetAmbCoef(coef);
	}
	//printf("SetAmbCoef\n");
}

void Tetris::AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs) {
	//printf("AlterSize\n");
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].AlterSize(sx, sy, sz);
	}
	//printf("AlterSize\n");
}

void Tetris::SetLocation(vector<float> T_center, vector<PrimCube>& Cbs) {
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

void Tetris::SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs) {
	vector<vector<float>> RotMat = Euler2Mat(euler_ang);
	BlockAssign(ObjTF, RotMat, 0, 3, 0, 3); // Tetris ObjTF
	for (int i = 0; i < base_loc.size(); i++) {
		BlockAssign(Cbs[ids[i]].ObjTF, RotMat, 0, 3, 0, 3);
	}
}

void Tetris::MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs) {
	ObjTF[3][0] += move_vec[0];
	ObjTF[3][1] += move_vec[1];
	ObjTF[3][2] += move_vec[2];
	for (int i = 0; i < base_loc.size(); i++) {
		Cbs[ids[i]].MoveCube(move_vec);
	}
}

void Tetris::RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs) {
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

vector<vector<float>> Tetris::GetBaseLocsInWC(vector<PrimCube>& Cbs) {
	vector<vector<float>> BaseLocsInWC;
	for (int i = 0; i < base_loc.size(); i++) {
		BaseLocsInWC.push_back({ Cbs[ids[i]].ObjTF[0][3], Cbs[ids[i]].ObjTF[1][3], Cbs[ids[i]].ObjTF[2][3] });
	}

	return BaseLocsInWC;
}

void Tetris::Find_Hint_Height(vector<PrimCube>& cubes, vector<vector<float>>& hint_location, int range) {
	vector<int> HintCubeIds(hint_location.size(), -1);
	//PrintMat(hint_location, "Hint Location = ");
	for (int i = 0; i < hint_location.size(); i++) {
		//for (int j = 0; j < ids.size(); j++) {
		for (int j = 0; j < range; j++) {
			if (j < 9 || j > 17) {
				if (abs(cubes[ids[j]].ObjTF[0][3] - hint_location[i][0]) < 0.1 && abs(cubes[ids[j]].ObjTF[1][3] - hint_location[i][1]) < 0.1) {
					if (hint_location[i].size() == 3) {
						hint_location[i].push_back(cubes[ids[j]].ObjTF[2][3]);
						HintCubeIds[i] = ids[j];
					}
					else if (hint_location[i][3] > cubes[ids[j]].ObjTF[2][3]) {
						hint_location[i][3] = cubes[ids[j]].ObjTF[2][3];
						HintCubeIds[i] = ids[j];
					}
				}
			}
		}
	}
	
	for (int i = 0; i < ids.size(); i++)
		cubes[ids[i]].hint = false;
	for (int i = 0; i < HintCubeIds.size(); i++) {
		//cout << HintCubeIds[i] << endl;
		if (HintCubeIds[i] >= 0)
			cubes[HintCubeIds[i]].hint = true;
	}
}

vector<vector<float>> Tetris::GetBaseLocation(vector<PrimCube>& cubes) {
	vector<vector<float>> BaseLocs;
	for (int i = 0; i < 9; i++) {
		BaseLocs.push_back({ cubes[ids[i]].ObjTF[0][3] , cubes[ids[i]].ObjTF[1][3] });
	}

	return BaseLocs;
}

vector<vector<float>> Tetris::Find_All_Height(vector<PrimCube>& cubes, int range) {
	// Find Base xy location
	vector<vector<float>> BaseLocs = GetBaseLocation(cubes);
	//for (int i = 0; i < base_loc.size(); i++) { // base_loc.size() = 9
	//	BaseLocs.push_back({ cubes[ids[i]].ObjTF[0][3] , cubes[ids[i]].ObjTF[1][3] });
	//}
	bool repeat;
	vector<int> HintCubeIds(BaseLocs.size(), -1); // BaseLocs.size = 9
	//PrintMat(hint_location, "Hint Location = ");
	for (int i = 0; i < BaseLocs.size(); i++) {
		//for (int j = 0; j < ids.size(); j++) {
		for (int j = 0; j < range; j++) {
			if (j < 9 || j > 17) {
				if (abs(cubes[ids[j]].ObjTF[0][3] - BaseLocs[i][0]) < 0.1 && abs(cubes[ids[j]].ObjTF[1][3] - BaseLocs[i][1]) < 0.1) {
					if (BaseLocs[i].size() == 2) {
						BaseLocs[i].push_back(cubes[ids[j]].ObjTF[2][3]);
						HintCubeIds[i] = ids[j];
					}
					else if (BaseLocs[i][2] > cubes[ids[j]].ObjTF[2][3]) {
						BaseLocs[i][2] = cubes[ids[j]].ObjTF[2][3];
						HintCubeIds[i] = ids[j];
					}
				}
			}
		}
	}
	return BaseLocs;
}

vector<vector<int>> Tetris::GetBaseEachLayerIds(vector<PrimCube>& cubes) {
	//vector<vector<float>> BaseLocs = GetBaseLocation(cubes);
	vector<vector<int>> All_Layer_Ids(14, vector<int>(1, -1));
	//vector<int> Each_Layer_Ids;
	int each_h;
	/*printf("ids shape: %d\n", ids.size());
	for (int i = 0; i < ids.size(); i++) {
		printf("%d ", i);
	}
	printf("\n");*/
	for (int i = 18; i < ids.size(); i++) {
		//printf("each_h: %f\n", cubes[ids[i]].ObjTF[2][3]);
		each_h = (int)((cubes[ids[i]].ObjTF[2][3] - 0.1)/ -1.2);
		//printf("each_h idx: %d\n", each_h);
		All_Layer_Ids[each_h].push_back(ids[i]);
	}
	
	return All_Layer_Ids;
}

int Tetris::GetDeleteLayerIds(vector<PrimCube>& cubes, vector<int> &one_layer) {
	vector<vector<int>> All_Layer_Ids = GetBaseEachLayerIds(cubes);
	for (int i = 0; i < All_Layer_Ids.size(); i++) {
		if (All_Layer_Ids[i].size() == 10) {
			one_layer = All_Layer_Ids[i];
			return i;
		}
	}
	one_layer = {};
	return -1;
}

void Tetris::CubeHShift(vector<vector<int>>& layer_ids, int idx, vector<PrimCube>& cubes) {
	float each_drop = 1.2;
	int if_drop = 1;
	if (if_drop == 1){
		for (int i = idx+1; i < layer_ids.size(); i++) {
			if (layer_ids[i].size() > 1) {
				for (int j = 1; j < layer_ids[i].size(); j++) {
					//cubes[layer_ids[i][j]].ObjTF[2][3] += 1.2; // Can call .MoveCube
					//each_drop += 0.2;
					cubes[layer_ids[i][j]].MoveCube({0, 0, each_drop});
					/*if (each_drop == 1.2) {
						if_drop = 0;
					}*/
				}
			}
		}
	}
}

void Tetris::CubeErase(vector<int> delete_layer_vec, vector<PrimCube>& cubes, Tetris& Base) {
	for (int i = 1; i < delete_layer_vec.size(); i++) { // 1 ~ 9
		for (int j = 0; j < Base.ids.size(); ) {
			if (delete_layer_vec[i] == Base.ids[j]) {
				Base.ids.erase(Base.ids.begin() + j); // erase idx in Base.ids
				cubes.erase(cubes.begin() + j);       // erase cube 
				//printf("erase %d, cube_size = %d\n", delete_layer_vec[i], cubes.size());
			}
			else {
				j++;
			}
		}
	}
	cubes.shrink_to_fit();
	Base.ids.shrink_to_fit();
	for (int i = 0; i < Base.ids.size(); i++) {
		Base.ids[i] = i;
	}
	/*printf("%d %d After idx: ", cubes.size(), Base.ids.size());
	for (int i = 0; i < cubes.size(); i++) {
		printf("%d ", Base.ids[i]);
	}
	printf("\n");*/
}

void Tetris::CheckDelete(vector<PrimCube>& cubes, Tetris& Base, int & Game_Score) {
	vector<vector<int>> layer_ids = Base.GetBaseEachLayerIds(cubes);
	/*for (int i = 0; i < layer_ids.size(); i++) {
		printf("layer %d: ", i);
		for (int j = 0; j < layer_ids[i].size(); j++) {
			printf("%d ", layer_ids[i][j]);
		}
		printf("\n");
	}*/
	vector<int> delete_layer_vec;
	int delete_layer_idx;
	delete_layer_idx = GetDeleteLayerIds(cubes, delete_layer_vec);
	while (delete_layer_idx >= 0) {
		CubeHShift(layer_ids, delete_layer_idx, cubes);
		CubeErase(delete_layer_vec, cubes, Base);
		layer_ids = Base.GetBaseEachLayerIds(cubes);
		delete_layer_idx = GetDeleteLayerIds(cubes, delete_layer_vec);
		Game_Score += 10;
	}
		
	/*for (int i = 0; i < layer_ids.size(); i++) {
		if (layer_ids[i].size() == 10) {
			CubeHShift(layer_ids, i, cubes);
			CubeErase(layer_ids, i, cubes, Base);
			layer_ids = Base.GetBaseEachLayerIds(cubes);
		}
	}*/
}