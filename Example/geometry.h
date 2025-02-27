#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "SDL2_gfxPrimitives.h" # Get the value of M_PI

using namespace std;

const int WIDTH = 600;
const int HEIGHT = 600;

vector<vector<float>> inK(float FoV, float scale);
vector<vector<float>> Euler2Mat(vector<float> cam_ori);
vector<vector<float>> RT_M(vector<float> cam_ori, vector<float> cam_loc);
vector<vector<float>> NormByScalar(vector<vector<float>> PTs, float scalar);
void NormByScalar(const vector<vector<float>>& PTs, float scalar, vector<vector<float>>& OutPTs);
vector<vector<float>> Transform_PTs(vector<vector<float>> PTs, vector<vector<float>> CamP);
void Transform_PTs(const vector<vector<float>>& PTs, const vector<vector<float>>& CamP, vector<vector<float>>& TFPTs);
vector<vector<float>> Project_PTs(vector<vector<float>> TfPTs, vector<vector<float>> CamK, float scale);
void Project_PTs(const vector<vector<float>>& TfPTs, const vector<vector<float>>& CamK, float scale, vector<vector<float>>& ImgPTs);
vector<float> Calculate_Depth(vector<vector<float>> PTs, vector<vector<float>> CamP);
void GetMatCarte2Oblique(const vector<float> &origin, const vector<float> &xpt, const vector<float> &ypt, vector<vector<float>> &Aff);
void GetMatOblique2Carte(const vector<float>& origin, const vector<float>& xpt, const vector<float>& ypt, vector<vector<float>>& Aff);

#endif // __GEOMETRY_H__

