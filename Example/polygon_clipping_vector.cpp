#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "linalg.h"
#include "geometry.h"
#include "polygon_clipping_vector.h"


// check if a point is on the LEFT side of an edge
//bool inside(point2D p, point2D p1, point2D p2)
bool inside(vector<float> p, vector<float> p1, vector<float> p2)
{
    return (p2[1] - p1[1]) * p[0] + (p1[0] - p2[0]) * p[1] + (p2[0] * p1[1] - p1[0] * p2[1]) < 0;
}

// calculate intersection point
vector<float> intersection(vector<float> cp1, vector<float> cp2, vector<float> s, vector<float> e)
{
    vector<float> dc = { cp1[0] - cp2[0], cp1[1] - cp2[1] };
    vector<float> dp = { s[0] - e[0], s[1] - e[1] };

    float n1 = cp1[0] * cp2[1] - cp1[1] * cp2[0];
    float n2 = s[0] * e[1] - s[1] * e[0];
    float n3 = 1.0 / (dc[0] * dp[1] - dc[1] * dp[0]);

    return { (n1 * dp[0] - n2 * dc[0]) * n3, (n1 * dp[1] - n2 * dc[1]) * n3 };
}

// Sutherland-Hodgman clipping
//void SutherlandHodgman(vector<vector<float>> subjectPolygon, int& subjectPolygonSize, vector<vector<float>> clipPolygon, int& clipPolygonSize, vector<float>(&newPolygon)[MAX_POLYGON], int& newPolygonSize)
void SutherlandHodgman(vector<vector<float>> subjectPolygon, int& subjectPolygonSize, vector<vector<float>> clipPolygon, int& clipPolygonSize, vector<vector<float>> &newPolygon, int& newPolygonSize)
{
    vector<float> cp1, cp2, s, e, inputPolygon[MAX_POLYGON];
    //vector<vector<float>> inputPolygon(MAX_POLYGON, vector<float>(2));

    // copy subject polygon to new polygon and set its size
    for (int i = 0; i < subjectPolygonSize; i++){
        newPolygon[i][0] = subjectPolygon[i][0];
        newPolygon[i][1] = subjectPolygon[i][1];
    }
    newPolygonSize = subjectPolygonSize;

    for (int j = 0; j < clipPolygonSize; j++)
    {
        // copy new polygon to input polygon & set counter to 0
        //for (int k = 0; k < newPolygonSize; k++) { inputPolygon[k][0] = newPolygon[k][0]; inputPolygon[k][1] = newPolygon[k][1]; }
        for (int k = 0; k < newPolygonSize; k++) { inputPolygon[k] = newPolygon[k]; }
        int counter = 0;

        // get clipping polygon edge
        cp1 = clipPolygon[j];
        cp2 = clipPolygon[(j + 1) % clipPolygonSize];

        for (int i = 0; i < newPolygonSize; i++)
        {
            // get subject polygon edge
            s = inputPolygon[i];
            e = inputPolygon[(i + 1) % newPolygonSize];

            // Case 1: Both vertices are inside:
            // Only the second vertex is added to the output list
            if (inside(s, cp1, cp2) && inside(e, cp1, cp2))
                newPolygon[counter++] = e;

            // Case 2: First vertex is outside while second one is inside:
            // Both the point of intersection of the edge with the clip boundary
            // and the second vertex are added to the output list
            else if (!inside(s, cp1, cp2) && inside(e, cp1, cp2))
            {
                newPolygon[counter++] = intersection(cp1, cp2, s, e);
                newPolygon[counter++] = e;
            }

            // Case 3: First vertex is inside while second one is outside:
            // Only the point of intersection of the edge with the clip boundary
            // is added to the output list
            else if (inside(s, cp1, cp2) && !inside(e, cp1, cp2))
                newPolygon[counter++] = intersection(cp1, cp2, s, e);

            // Case 4: Both vertices are outside
            else if (!inside(s, cp1, cp2) && !inside(e, cp1, cp2))
            {
                // No vertices are added to the output list
            }
        }
        // set new polygon size
        newPolygonSize = counter;
    }
}
/*
vector<point2D> Vec2D_2_Vecpoiont2D(vector<vector<float>> vec2d, bool round) {
    vector<point2D> vecpoint2D;
    for (int i = 0; i < vec2d.size(); i++) {
        point2D tmp;
        if (round)
            tmp = { (float)((int)(vec2d[i][0] + 0.5)), (float)((int)(vec2d[i][1] + 0.5)) };
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
*/