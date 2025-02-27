#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"

using namespace std;

vector<vector<float>> GetIdentity(int s) {
	vector<vector<float>> out(s, vector<float>(s, 0));
	for (int i = 0; i < s;  i++)	out[i][i] = 1;

	return out;
}

vector<vector<float>> M_Cat(const vector<vector<float>> &a, const vector<vector<float>> &b, int dim) {
	int ar = a.size(), br = b.size(), ac = a[0].size(), bc = b[0].size();
	int outr, outc;
	if (dim == 0) {
		outr = ar + br; outc = ac;
	}
	else {
		outr = ar, outc = ac + bc;
	}
	vector<vector<float>> out(outr, vector<float>(outc, 0));
	if (dim == 0) {
		for (int i = 0; i < ar; i++)
			for (int j = 0; j < outc; j++)
				out[i][j] = a[i][j];
		for (int i = 0; i < br; i++)
			for (int j = 0; j < outc; j++)
				out[i + ar][j] = b[i][j];
	}
	else {
		for (int i = 0; i < outr; i++)
			for (int j = 0; j < ac; j++)
				out[i][j] = a[i][j];
		for (int i = 0; i < outr; i++)
			for (int j = 0; j < bc; j++)
				out[i][j + ac] = b[i][j];
	}

	return out;
}

vector<vector<float>> ZeroInitMat(int r, int c) {
	vector<vector<float>> out(r, vector<float>(c, 0));

	return out;
}

vector<vector<float>> OneInitMat(int r, int c) {
	vector<vector<float>> out(r, vector<float>(c, 1));

	return out;
}

vector<vector<float>> MS_Mul(const vector<vector<float>> &a, float s) {
	int mr = a.size(), mc = a[0].size();
	vector<vector<float>> out(mr, vector<float>(mc, 0));
	for (int i = 0; i < mr; i++)
		for (int j = 0; j < mc; j++)
			out[i][j] = a[i][j] * s;

	return out;
}

void MS_Mul(vector<vector<float>>& a, float s) {
	int mr = a.size(), mc = a[0].size();
	for (int i = 0; i < mr; i++)
		for (int j = 0; j < mc; j++)
			a[i][j] *= s;
}

vector<vector<float>> MS_Add(const vector<vector<float>> &a, float s) {
	int mr = a.size(), mc = a[0].size();
	vector<vector<float>> out(mr, vector<float>(mc, 0));
	for (int i = 0; i < mr; i++)
		for (int j = 0; j < mc; j++)
			out[i][j] = a[i][j] + s;

	return out;
}

vector<float> VV_Sub(const vector<float> &a, const vector<float> &b) {
	vector<float> out(a.size());
	for (int i = 0; i < a.size(); i++) {
		out[i] = a[i] - b[i];
	}

	return out;
}

vector<float> VV_Add(const vector<float> &a, const vector<float> &b) {
	vector<float> out(a.size());
	for (int i = 0; i < a.size(); i++) {
		out[i] = a[i] + b[i];
	}

	return out;
}

vector<float> VS_Mul(vector<float> a, float s) {
	for (int i = 0; i < a.size(); i++) a[i] *= s;

	return a;
}

vector<vector<float>> MM_Mul(const vector<vector<float>> &a, const vector<vector<float>> &b) {
	int m1r = a.size(), m1c = a[0].size(), m2c = b[0].size();
	vector<vector<float>> out(m1r, vector<float>(m2c, 0));
	for (int i = 0; i < m2c; i++) {
		for (int j = 0; j < m1r; j++) {
			float tmp = 0;
			for (int k = 0; k < m1c; k++) {
				tmp += a[j][k] * b[k][i];
			}
			out[j][i] = tmp;
		}
	}

	return out;
}

vector<float> MV_Mul(const vector<vector<float>> &a, const vector<float> &b) {
	int m1r = a.size(), br = b.size();
	vector<float> out(m1r, 0);
	for (int i = 0; i < m1r; i++) {
		for (int j = 0; j < br; j++) {
			out[i] += a[i][j] * b[j];
		}
	}

	return out;
}

void MV_Mul(const vector<vector<float>>& a, const vector<float>& b, vector<float> &out) {
	int m1r = a.size(), br = b.size();
	float tmp;
	for (int i = 0; i < m1r; i++) {
		tmp = 0;
		for (int j = 0; j < br; j++) {
			tmp += a[i][j] * b[j];
		}
		out[i] = tmp;
	}
}

void MM_Mul(const vector<vector<float>> &a, const vector<vector<float>> &b, vector<vector<float>> &out) {
	int m1r = a.size(), m1c = a[0].size(), m2c = b[0].size();
	//vector<vector<float>> out(m1r, vector<float>(m2c, 0));
	float tmp;
	for (int i = 0; i < m2c; i++) {
		for (int j = 0; j < m1r; j++) {
			tmp = 0;
			for (int k = 0; k < m1c; k++) {
				tmp += a[j][k] * b[k][i];
			}
			out[j][i] = tmp;
		}
	}
	//return out;
}

void PrintMat(vector<vector<float>> a, string name) {
	cout << name << " Size (" << a.size() << ", " << a[0].size() << ")" << " = " << endl;
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a[0].size(); j++) {
			cout << " " << a[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

void PrintMati(vector<vector<int>> a, string name) {
	cout << name << " Size (" << a.size() << ", " << a[0].size() << ")" << " = " << endl;
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a[0].size(); j++) {
			cout << " " << a[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

void PrintVec(vector<float> a, string name) {
	cout << name << " Size (" << a.size() << ")" << " = " << endl;
	for (int i = 0; i < a.size(); i++) {
		cout << " " << a[i];
	}
	cout << endl;
}

void PrintVeci(vector<int> a, string name) {
	cout << name << " Size (" << a.size() << ")" << " = " << endl;
	for (int i = 0; i < a.size(); i++) {
		cout << " " << a[i];
	}
	cout << endl;
}

float Dot(const vector<float> &a, const vector<float> &b) {
	int len = a.size();
	float out = 0;

	for (int i = 0; i < len; i++)
		out += a[i] * b[i];

	return out;
}

vector<vector<float>> M_Transpose(const vector<vector<float>> &mat) {
	int outr = mat[0].size(), outc = mat.size();
	vector<vector<float>> out(outr, vector<float>(outc, 0));
	for (int i = 0; i < outr; i++)
		for (int j = 0; j < outc; j++)
			out[i][j] = mat[j][i];

	return out;
}

vector<vector<float>> SubM(const vector<vector<float>> &mat, int sr, int er, int sc, int ec) {
	vector<vector<float>> out(er - sr, vector<float>(ec - sc, 0));
	for (int i = sr; i < er; i++)
		for (int j = sc; j < ec; j++)
			out[i - sr][j - sc] = mat[i][j];

	return out;
}

vector<vector<float>> Unsqueeze(const vector<float> &vec, int dim) {
	int len = vec.size();
	int mr, mc;
	if (dim == 0) {
		mr = 1;
		mc = len;
	}
	else {
		mr = len;
		mc = 1;
	}
	vector<vector<float>> out(mr, vector<float>(mc, 0));

	for (int i = 0; i < mr; i++)
		for (int j = 0; j < mc; j++)
			out[i][j] = vec[i > j ? i : j];

	return out;
}

vector<float> Squeeze(const vector<vector<float>> &mat, int dim) {
	int mr = mat.size(), mc = mat[0].size();
	int len;
	if (dim == 0) {
		len = mc;
	}
	else {
		len = mr;
	}
	vector<float> out(len, 0);
	if (dim == 0) {
		for (int i = 0; i < len; i++)
			out[i] = mat[0][i];
	}
	else {
		for (int i = 0; i < len; i++)
			out[i] = mat[i][0];
	}

	return out;
}

vector<float> Normalize(vector<float> vec) { // 1 x 3 vector
	//cout << "!!!Yeh Normal " << PTs.size() << " = " << endl;
	int len = vec.size();
	float n_sum = 0;
	for (int i = 0; i < len; i++)
		n_sum += vec[i] * vec[i];
	n_sum = sqrt(n_sum);
	for (int i = 0; i < len; i++)
		vec[i] = vec[i] / n_sum;

	return vec;
}

void BlockAssign(vector<vector<float>> &a, const vector<vector<float>> &b, int sr, int er, int sc, int ec) {
	for (int i = sr; i < er; i++) {
		for (int j = sc; j < ec; j++) {
			int bi = i - sr;
			int bj = j - sc;
			//cout << "bi , bj= " << bi << " " << bj << endl;
			a[i][j] = b[bi][bj];
		}
	}
}

//vector<vector<float>> MInv2by2(const vector<vector<float>> &a) {
//	float coef = 1. / (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
//	vector<vector<float>> out = { {a[1][1], -a[0][1]}, {-a[1][0], a[0][0]} };
//
//	return MS_Mul(out, coef);
//}

void MInv2by2(vector<vector<float>>& a) {
	float coef = 1. / (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
	float tmp;
	tmp = coef * a[0][0];
	a[0][0] = coef * a[1][1];
	a[1][1] = tmp;
	a[0][1] *= -coef;
	a[1][0] *= -coef;
	//a[0][0] = coef * a[1][1];
	//a[0][1] = coef * -a[0][1];
	//a[1][0] = coef * -a[1][0];
	//a[1][1] = coef * a[0][0];
	//a = { {coef * a[1][1], coef * -a[0][1]}, { coef * -a[1][0], coef * a[0][0]} };
}

void Clear1DVector(vector<float> &vec) {
	vec.clear();
	vec.shrink_to_fit();
}

void Clear2DVector(vector<vector<float>>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		Clear1DVector(vec[i]);
	}
	vec.clear();
	vec.shrink_to_fit();
}

void Clear3DVector(vector<vector<vector<float>>>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		Clear2DVector(vec[i]);
	}
	vec.clear();
	vec.shrink_to_fit();
}

void Clear4DVector(vector<vector<vector<vector<float>>>>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		Clear3DVector(vec[i]);
	}
	vec.clear();
	vec.shrink_to_fit();
}