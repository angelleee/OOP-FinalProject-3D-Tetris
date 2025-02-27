#include<stdio.h>
#include <vector>
#include"myalgo.h"

using namespace std;

void IntInit1D(int* Arr, int Size) {
	// Initial Idx
	for (int i = 0; i < Size; i++) {
		Arr[i] = i;
	}
}

void Sort_Idx_Descend(float *DisArr, int *DisIdx, int CbsSize){
	IntInit1D(DisIdx, CbsSize);
	// Descending Sort (Large to small)
	for (int i = 0; i < CbsSize - 1; i++) {
		for (int j = i + 1; j < CbsSize; j++) {
			if (DisArr[i] < DisArr[j]) {
				float tmp = DisArr[i];
				DisArr[i] = DisArr[j];
				DisArr[j] = tmp;
				int tmp_idx = DisIdx[i];
				DisIdx[i] = DisIdx[j];
				DisIdx[j] = tmp_idx;
			}
		}
	}
	//return DisIdx;
}


void Sort_Idx_Descend(vector<float> &DisArr, vector<int> &DisIdx) {
	int CbsSize = DisArr.size();
	// Descending Sort (Large to small)
	for (int i = 0; i < CbsSize - 1; i++) {
		for (int j = i + 1; j < CbsSize; j++) {
			if (DisArr[i] < DisArr[j]) {
				float tmp = DisArr[i];
				DisArr[i] = DisArr[j];
				DisArr[j] = tmp;
				int tmp_idx = DisIdx[i];
				DisIdx[i] = DisIdx[j];
				DisIdx[j] = tmp_idx;
			}
		}
	}
}