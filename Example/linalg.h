#ifndef __LINALG_H__
#define __LINALG_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

vector<vector<float>> GetIdentity(int s);
vector<vector<float>> M_Cat(const vector<vector<float>> &a, const vector<vector<float>>& b, int dim);
vector<vector<float>> ZeroInitMat(int r, int c);
vector<vector<float>> OneInitMat(int r, int c);
vector<vector<float>> MS_Mul(const vector<vector<float>> &a, float s);
void MS_Mul(vector<vector<float>>& a, float s);
vector<vector<float>> MS_Add(const vector<vector<float>> &a, float s);
vector<float> VV_Sub(const vector<float> &a, const vector<float> &b);
vector<float> VV_Add(const vector<float> &a, const vector<float> &b);
vector<float> VS_Mul(vector<float> a, float s);
vector<vector<float>> MM_Mul(const vector<vector<float>> &a, const vector<vector<float>> &b);
void MM_Mul(const vector<vector<float>>& a, const vector<vector<float>> &b, vector<vector<float>> &out);
vector<float> MV_Mul(const vector<vector<float>> &a, const vector<float> &b);
void MV_Mul(const vector<vector<float>>& a, const vector<float>& b, vector<float>& out);
void PrintMat(vector<vector<float>> a, string name);
void PrintMati(vector<vector<int>> a, string name);
void PrintVec(vector<float> a, string name);
void PrintVeci(vector<int> a, string name);
float Dot(const vector<float>& a, const vector<float>& b);
vector<vector<float>> M_Transpose(const vector<vector<float>> &mat);
vector<vector<float>> SubM(const vector<vector<float>> &mat, int sr, int er, int sc, int ec);
vector<vector<float>> Unsqueeze(const vector<float> &vec, int dim);
vector<float> Squeeze(const vector<vector<float>> &mat, int dim);
vector<float> Normalize(vector<float> vec);
void BlockAssign(vector<vector<float>> &a, const vector<vector<float>> &b, int sr, int er, int sc, int ec);
//vector<vector<float>> MInv2by2(const vector<vector<float>> &a);
void MInv2by2(vector<vector<float>>& a);
void Clear1DVector(vector<float>& vec);
void Clear2DVector(vector<vector<float>>& vec);
void Clear3DVector(vector<vector<vector<float>>>& vec);
void Clear4DVector(vector<vector<vector<vector<float>>>>& vec);


typedef float mat33[3][3];
typedef float mat34[3][4];
typedef float mat22[2][2];
typedef float mat23[2][3];


#endif // __LINALG_H__

