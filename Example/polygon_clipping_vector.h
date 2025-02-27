#pragma once
#ifndef __POLYCLIP_H__
#define __POLYCLIP_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "linalg.h"
#include "geometry.h"

using namespace std;

const int   MAX_POLYGON = 20; // clipped (new) polygon size

//struct vector<float> { float x, y; };

bool inside(vector<float> p, vector<float> p1, vector<float> p2);
vector<float> intersection(vector<float> cp1, vector<float> cp2, vector<float> s, vector<float> e);
//void SutherlandHodgman(vector<vector<float>> subjectPolygon, int& subjectPolygonSize, vector<vector<float>> clipPolygon, int& clipPolygonSize, vector<float>(&newPolygon)[MAX_POLYGON], int& newPolygonSize);
void SutherlandHodgman(vector<vector<float>> subjectPolygon, int& subjectPolygonSize, vector<vector<float>> clipPolygon, int& clipPolygonSize, vector<vector<float>>& newPolygon, int& newPolygonSize);
//vector<vector<float>> Vec2D_2_Vecpoiont2D(vector<vector<float>> vec2d, bool round);
//void PrintVec2Ds(vector<vector<float>> vecpoint2D, string name);

#endif // POLYCLIP