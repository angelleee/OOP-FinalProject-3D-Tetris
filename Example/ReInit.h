#ifndef __REINIT_H__
#define __REINIT_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "geometry.h"
#include "linalg.h"
#include "model.h"
#include "tetris.h"
#include "rules.h"

using namespace std;

void ReInitGame(bool& Game_over, int& ReStart, int& Game_Score, float& tt, float& CR, int& collect_or_not, float& Cube_Lowest, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris_ALL& Base_deadline, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location);

#endif // __REINIT_H__