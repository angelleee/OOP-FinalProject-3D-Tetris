#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "linalg.h"
#include "geometry.h"
#include "polygon_clipping.h"



// check if a point is on the LEFT side of an edge
bool inside_acc(point2D &p, point2D &p1, point2D &p2)
{
    if (p1.y == p2.y) {
        if (p1.x > p2.x) {
            return p.y > p1.y;
        }
        else {
            return p.y < p1.y;
        }
    }
    else {
        if (p1.y > p2.y) {
            return p.x > p1.x;
        }
        else {
            return p.x < p1.x;
        }
    }
    
}

//bool inside(point2D& p, point2D& p1, point2D& p2) {
bool inside(float fp[2], float fp1[2], float fp2[2]) {
    /*float fp[2] = {p.x, p.y};
    float fp1[2] = {p1.x, p1.y};
    float fp2[2] = {p2.x, p2.y};*/
    return (fp2[1] - fp1[1]) * fp[0] + (fp1[0] - fp2[0]) * fp[1] + (fp2[0] * fp1[1] - fp1[0] * fp2[1]) < 0;
    //return (p2.y - p1.y) * p.x + (p1.x - p2.x) * p.y + (p2.x * p1.y - p1.x * p2.y) < 0;
}
// calculate intersection point
//point2D intersection(point2D &cp1, point2D &cp2, point2D &s, point2D &e)
point2D intersection(float fcp1[2], float fcp2[2], float fs[2], float fe[2])
{
    /*float fcp1[2] = { cp1.x, cp1.y};
    float fcp2[2] = { cp2.x, cp2.y };
    float fs[2] = { s.x, s.y};
    float fe[2] = { e.x, e.y };*/
    float dc[2] = { fcp1[0] - fcp2[0], fcp1[1] - fcp2[1]};
    float dp[2] = { fs[0] - fe[0], fs[1] - fe[1] };

    //point2D dc = { cp1.x - cp2.x, cp1.y - cp2.y };
    //point2D dp = { s.x - e.x, s.y - e.y };

    float n1 = fcp1[0] * fcp2[1] - fcp1[1] * fcp2[0];
    float n2 = fs[0] * fe[1] - fs[1] * fe[0];
    float n3 = 1.0 / (dc[0] * dp[1] - dc[1] * dp[0]);
    /*float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
    float n2 = s.x * e.y - s.y * e.x;
    float n3 = 1.0 / (dc.x * dp.y - dc.y * dp.x);*/

    //return { (n1 * dp.x - n2 * dc.x) * n3, (n1 * dp.y - n2 * dc.y) * n3 };
    return { (n1 * dp[0] - n2 * dc[0]) * n3, (n1 * dp[1] - n2 * dc[1]) * n3 };
}

// Sutherland-Hodgman clipping
void SutherlandHodgman(const point2D (&subjectPolygon)[6], const int& subjectPolygonSize, const point2D (&clipPolygon)[4], 
                        const int& clipPolygonSize, point2D(&newPolygon)[MAX_POLYGON], int& newPolygonSize)
{
    point2D inputPolygon[MAX_POLYGON]; // cp1, cp2, s, e, 
    float cp1[2] = { 0 };
    float cp2[2] = { 0 };
    float s[2] = { 0 };
    float e[2] = { 0 };
    bool sin, ein;
    // copy subject polygon to new polygon and set its size
    for (int i = 0; i < subjectPolygonSize; i++)
        newPolygon[i] = subjectPolygon[i];

    newPolygonSize = subjectPolygonSize;

    for (int j = 0; j < clipPolygonSize; j++)
    {
        // copy new polygon to input polygon & set counter to 0
        for (int k = 0; k < newPolygonSize; k++) { inputPolygon[k] = newPolygon[k]; }
        int counter = 0;

        // get clipping polygon edge
        /*cp1 = clipPolygon[j];
        cp2 = clipPolygon[(j + 1) % clipPolygonSize];*/
        cp1[0] = clipPolygon[j].x;
        cp1[1] = clipPolygon[j].y;
        cp2[0] = clipPolygon[(j + 1) % clipPolygonSize].x;
        cp2[1] = clipPolygon[(j + 1) % clipPolygonSize].y;

        for (int i = 0; i < newPolygonSize; i++)
        {
            // get subject polygon edge
            /*s = inputPolygon[i];
            e = inputPolygon[(i + 1) % newPolygonSize];*/
            s[0] = inputPolygon[i].x;
            s[1] = inputPolygon[i].y;
            e[0] = inputPolygon[(i + 1) % newPolygonSize].x;
            e[1] = inputPolygon[(i + 1) % newPolygonSize].y;
            //sin = inside(s, cp1, cp2);
            //ein = inside(e, cp1, cp2);
            sin = (cp2[1] - cp1[1]) * s[0] + (cp1[0] - cp2[0]) * s[1] + (cp2[0] * cp1[1] - cp1[0] * cp2[1]) < 0;
            ein = (cp2[1] - cp1[1]) * e[0] + (cp1[0] - cp2[0]) * e[1] + (cp2[0] * cp1[1] - cp1[0] * cp2[1]) < 0;
            // Case 1: Both vertices are inside:
            // Only the second vertex is added to the output list
            if (sin && ein) {
                //newPolygon[counter++] = e;
                newPolygon[counter].x = e[0];
                newPolygon[counter].y = e[1];
                counter++;
            }
                

            // Case 2: First vertex is outside while second one is inside:
            // Both the point of intersection of the edge with the clip boundary
            // and the second vertex are added to the output list
            else if (!sin && ein)
            {
                /*newPolygon[counter++] = intersection(cp1, cp2, s, e);
                newPolygon[counter++] = e;*/
                newPolygon[counter] = intersection(cp1, cp2, s, e);
                counter++;
                newPolygon[counter].x = e[0];
                newPolygon[counter].y = e[1];
                counter++;
            }

            // Case 3: First vertex is inside while second one is outside:
            // Only the point of intersection of the edge with the clip boundary
            // is added to the output list
            else if (sin && !ein)
                newPolygon[counter++] = intersection(cp1, cp2, s, e);

            // Case 4: Both vertices are outside
            else if (!sin && !ein)
            {
                // No vertices are added to the output list
            }
        }
        // set new polygon size
        newPolygonSize = counter;
    }
}

vector<point2D> Vec2D_2_Vecpoiont2D(vector<vector<float>> vec2d, bool round) {
    vector<point2D> vecpoint2D;
    for (int i = 0; i < vec2d.size(); i++) {
        point2D tmp;
        if (round)
            tmp = { (float) ((int) (vec2d[i][0] + 0.5)), (float) ((int) (vec2d[i][1] + 0.5)) };
        else {
            tmp = { (float)vec2d[i][0], (float)vec2d[i][1] };
        }
        vecpoint2D.push_back(tmp);
    }

    return vecpoint2D;
}

void PrintVec2Ds(vector<point2D> vecpoint2D, string name) {
    cout << name << " Size (" << vecpoint2D.size() << ", " << "2" << ")" << " = " << endl;
    for (int i = 0; i < vecpoint2D.size(); i++) {
        cout << vecpoint2D[i].x << " " << vecpoint2D[i].y << endl;
    }
    cout << endl;
}


//int main(int argc, char** argv)
//{
//    // subject polygon
//    vector<point2D> subjectPolygon, clipPolygon;
//    vector<vector<float>> aa = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
//    for (int i = 0; i < aa.size(); i++) {
//        point2D tmp = { aa[i][0], aa[i][1] };
//        subjectPolygon.push_back(tmp);
//    }
//    int subjectPolygonSize = subjectPolygon.size();
//
//    // clipping polygon
//    aa = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
//    for (int i = 0; i < aa.size(); i++) {
//        point2D tmp = { aa[i][0], aa[i][1] };
//        clipPolygon.push_back(tmp);
//    }
//    int clipPolygonSize = clipPolygon.size();
//
//    // define the new clipped polygon (empty)
//    int newPolygonSize = 0;
//    point2D newPolygon[N] = { 0 };
//
//    // apply clipping
//    SutherlandHodgman(subjectPolygon, subjectPolygonSize, clipPolygon, clipPolygonSize, newPolygon, newPolygonSize);
//
//    // print clipped polygon points
//    cout << "Clipped polygon points:" << endl;
//    cout << "N = " << newPolygonSize << endl;
//    for (int i = 0; i < newPolygonSize; i++)
//        cout << "(" << newPolygon[i].x << ", " << newPolygon[i].y << ")" << endl;
//
//    return 0;
//}
