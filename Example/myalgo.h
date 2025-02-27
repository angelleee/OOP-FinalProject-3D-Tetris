#pragma once
#ifndef __MYALGO_H__
#define __MYALGO_H__
#include<stdio.h>
#include<stdlib.h>
#include <vector>

using namespace std;
void IntInit1D(int* Arr, int Size);
void Sort_Idx_Descend(float* DisArr, int* DisIdx, int CbsSize);
void Sort_Idx_Descend(vector<float>& DisArr, vector<int>& DisIdx);

#endif