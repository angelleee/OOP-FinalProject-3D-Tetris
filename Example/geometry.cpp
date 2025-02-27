#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "geometry.h"
#include "linalg.h"

using namespace std;


vector<vector<float>> inK(float FoV, float scale) {
	float length = WIDTH / 2;
	float Fov_rad = FoV / 180. * M_PI;
	float f = length * 1 / tan(0.5 * Fov_rad) * scale;
	float cx = (length * 2 - 1) / 2.0;
	float cy = cx;
	vector<vector<float>> K(3, vector<float>(3, 0));
	K[0][0] = f; K[0][2] = cx; K[1][1] = f; K[1][2] = cy; K[2][2] = 1;
	//float** K;
	//K = new float* [3];
	//for (int i = 0; i < 3; i++) {
	//	K[i] = new float[3];
	//}
	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		K[i][j] = 0;
	//	}
	//}

	//float K[3][3] = { {f, 0, cx}, {0, f, cy}, {0, 0, 1} };
	return K;
}

vector<vector<float>> Euler2Mat(vector<float> cam_ori) {
	vector<vector<float>> R(3, vector<float>(3, 0));
	// Convert to radius
	float radx = cam_ori[0] / 180. * M_PI;
	float rady = cam_ori[1] / 180. * M_PI;
	float radz = cam_ori[2] / 180. * M_PI;
	R[0][0] = (cos(rady) * cos(radz)) - (sin(radx) * sin(rady) * sin(radz));
	R[0][1] = -1 * cos(radx) * sin(radz);
	R[0][2] = (sin(rady) * cos(radz)) + (sin(radx) * cos(rady) * sin(radz));

	R[1][0] = (cos(rady) * sin(radz)) + (sin(radx) * sin(rady) * cos(radz));
	R[1][1] = cos(radx) * cos(radz);
	R[1][2] = (sin(rady) * sin(radz)) - (sin(radx) * cos(rady) * cos(radz));

	R[2][0] = -1 * cos(radx) * sin(rady);
	R[2][1] = sin(radx);
	R[2][2] = cos(radx) * cos(rady);

	return R;
}


vector<vector<float>> RT_M(vector<float> cam_ori, vector<float> cam_loc) {
	vector<vector<float>> R(3, vector<float>(3, 0));
	vector<vector<float>> T(3, vector<float>(1, 0));
	vector<vector<float>> RT(3, vector<float>(4, 0));
	T[0][0] = cam_loc[0]; T[1][0] = cam_loc[1]; T[2][0] = cam_loc[2];
	R = Euler2Mat(cam_ori);
	R = M_Transpose(R);
	
	T = MS_Mul(MM_Mul(R, T), -1);

	BlockAssign(RT, R, 0, 3, 0, 3);
	BlockAssign(RT, T, 0, 3, 3, 4);

	return RT;
}

vector<vector<float>> NormByScalar(vector<vector<float>> PTs, float scalar) {
	for (int i = 0; i < PTs.size(); i++) {
		PTs[i][0] = PTs[i][0] / scalar; //PTs[i][2];
		PTs[i][1] = PTs[i][1] / scalar; //PTs[i][2];
		PTs[i][2] = 1; //PTs[i][2] / 5.; //PTs[i][2];
	}

	return PTs;
}

void NormByScalar(const vector<vector<float>> &PTs, float scalar, vector<vector<float>> &OutPTs) {
	for (int i = 0; i < PTs.size(); i++) {
		OutPTs[i][0] = PTs[i][0] / scalar; //PTs[i][2];
		OutPTs[i][1] = PTs[i][1] / scalar; //PTs[i][2];
	}
}

vector<vector<float>> Transform_PTs(vector<vector<float>> PTs, vector<vector<float>> CamP) {
	// PTS (#pts, 3)
	// CamP (3, 4)
	// CamK (3, 3)
	vector<vector<float>>HoPTs = M_Transpose(M_Cat(PTs, OneInitMat(PTs.size(), 1), 1));
	//PrintMat(HoPTs, "HoPTs");
	vector<vector<float>>TfPTs = M_Transpose(SubM(MM_Mul(CamP, HoPTs), 0, 3, 0, HoPTs[0].size()));

	return TfPTs;
}

void Transform_PTs(const vector<vector<float>> &PTs, const vector<vector<float>> &CamP, vector<vector<float>> &TFPTs) {
	// PTS (#pts, 3)
	// CamP (3, 4)
	// CamK (3, 3)
	int k;
	for(int i = 0; i < TFPTs.size(); i++){
		for (int j = 0; j < TFPTs[0].size(); j++) {
			TFPTs[i][j] = 0;
			for (k = 0; k < PTs[0].size(); k++) {
				TFPTs[i][j] += CamP[j][k] * PTs[i][k];
			}
			TFPTs[i][j] += CamP[j][k];
		}
	}
	//vector<vector<float>>HoPTs = M_Transpose(M_Cat(PTs, OneInitMat(PTs.size(), 1), 1));
	//PrintMat(HoPTs, "HoPTs");
	//vector<vector<float>>TfPTs = M_Transpose(SubM(MM_Mul(CamP, HoPTs), 0, 3, 0, HoPTs[0].size()));
}

vector<vector<float>> Project_PTs(vector<vector<float>> TfPTs, vector<vector<float>> CamK, float scale) {
	vector<vector<float>>ExPTs = M_Transpose(NormByScalar(TfPTs, scale));
	//vector<vector<float>>ExPTs = M_Transpose(M_Transpose(TfPTs));

	//PrintMat(ExPTs, "ExPTs");

	vector<vector<float>>InPTs = MM_Mul(CamK, ExPTs);
	//PrintMat(InPTs, "InPTs");

	vector<vector<float>>ImgPTs = M_Transpose(SubM(InPTs, 0, 2, 0, InPTs[0].size()));

	//PrintMat(ImgPTs, "ImgPTs");

	return ImgPTs;
}

void Project_PTs(const vector<vector<float>> &TfPTs, const vector<vector<float>> &CamK, float scale, vector<vector<float>> &ImgPTs) {
	NormByScalar(TfPTs, scale, ImgPTs);
	int k;
	vector<float> tmp(ImgPTs[0].size());
	for (int i = 0; i < ImgPTs.size(); i++) {
		for (int j = 0; j < ImgPTs[0].size(); j++) {
			tmp[j] = 0;
			for (k = 0; k < ImgPTs[0].size(); k++) {
				tmp[j] += CamK[j][k] * ImgPTs[i][k];
			}
			tmp[j] += CamK[j][k];
		}
		ImgPTs[i] = tmp;
	}
}

vector<float> Calculate_Depth(vector<vector<float>> PTs, vector<vector<float>> CamP) {
	// PTS (#pts, 3)
	// CamP (3, 4)
	// CamK (3, 3)
	vector<vector<float>>HoPTs = M_Transpose(M_Cat(PTs, OneInitMat(PTs.size(), 1), 1)); //  1 * 3 cat 1 * 1 (concate in col-basis 1) -> 1 x 4 , then T -> 4 x 1 [homogeneous point]
	//PrintMat(HoPTs, "HoPTs");
	vector<float>Depths = Squeeze(SubM(MM_Mul(CamP, HoPTs), 2, 3, 0, HoPTs[0].size()), 0); // CamP: 3 x 4, HoPTs: 4 x 1 -> 3 x 4 * 4 x 1 -> 3 x 1 -> select Row 2-3, Col 0-1 (z value) (1, 1) -> (1, )

	return Depths;
}

void GetMatCarte2Oblique(const vector<float> &origin, const vector<float> &xpt, const vector<float> &ypt, vector<vector<float>> &Aff) {
	//vector<float> xax = VV_Sub(xpt, origin);
	//vector<float> yax = VV_Sub(ypt, origin);
	Aff[0][0] = xpt[0] - origin[0];
	Aff[0][1] = ypt[0] - origin[0];
	Aff[1][0] = xpt[1] - origin[1];
	Aff[1][1] = ypt[1] - origin[1];
	//vector<vector<float>> Aff = { {xpt[0] - origin[0], ypt[0] - origin[0]}, {xpt[1] - origin[1], ypt[1] - origin[1]} }; // A
	MInv2by2(Aff); // A^-1
	Aff[0][2] = -(Aff[0][0] * origin[0] + Aff[0][1] * origin[1]);
	Aff[1][2] = -(Aff[1][0] * origin[0] + Aff[1][1] * origin[1]);
	//vector<float> Tr = MV_Mul(Aff, origin);
	//Tr = VS_Mul(Tr, (float)-1); // MM_Mul(MS_Mul(A, -1), Unsqueeze(origin, 1)); // -A^-1 * t
	/*Aff[0].push_back(Tr[0]);
	Aff[1].push_back(Tr[1]);*/
}

void GetMatOblique2Carte(const vector<float> &origin, const vector<float> &xpt, const vector<float> &ypt, vector<vector<float>> &Aff) {
	vector<float> xax = VV_Sub(xpt, origin);
	vector<float> yax = VV_Sub(ypt, origin);
	Aff[0][0] = xpt[0] - origin[0];
	Aff[0][1] = ypt[0] - origin[0];
	Aff[0][2] = origin[0];
	Aff[1][0] = xpt[1] - origin[1];
	Aff[1][1] = ypt[1] - origin[1];
	Aff[1][2] = origin[1];
	//vector<vector<float>> Affine = { {xax[0], yax[0], origin[0]}, {xax[1], yax[1], origin[1]} }; // A
}
