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

const int   MAX_POLYGON = 10; // clipped (new) polygon size

struct point2D { float x, y; };

//bool inside(point2D &p, point2D &p1, point2D &p2);
bool inside(float p[2], float p1[2], float p2[2]);
bool inside_acc(point2D& p, point2D& p1, point2D& p2);
//point2D intersection(point2D &cp1, point2D &cp2, point2D &s, point2D &e);
point2D intersection(float cp1[2], float cp2[2], float s[2], float e[2]);
void SutherlandHodgman(const point2D(&subjectPolygon)[6], const int& subjectPolygonSize, const point2D(&clipPolygon)[4],
                        const int& clipPolygonSize, point2D(&newPolygon)[MAX_POLYGON], int& newPolygonSize);
vector<point2D> Vec2D_2_Vecpoiont2D(vector<vector<float>> vec2d, bool round);
void PrintVec2Ds(vector<point2D> vecpoint2D, string name);

#endif // POLYCLIP