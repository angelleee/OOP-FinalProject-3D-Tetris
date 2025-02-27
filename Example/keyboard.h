#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "geometry.h"
#include "linalg.h"

using namespace std;

void LightAuto(float& LRx, float& LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float>& LightLoc, vector<float>& LightOri, vector<vector<float>>& LightP, vector<vector<float>>& LightOriMat, vector<float>& LightRay);
void LightControl(int& Light_x_up, int& Light_x_do, int& Light_y_up, int& Light_y_do, int& Light_z_up, int& Light_z_do, float& LRx, float& LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float>& LightLoc, vector<float>& LightOri, vector<vector<float>>& LightP, vector<vector<float>>& LightOriMat, vector<float>& LightRay);
void LightReset(float& LRx, float& LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float>& LightLoc, vector<float>& LightOri, vector<vector<float>>& LightP, vector<vector<float>>& LightOriMat, vector<float>& LightRay);
void Base_Rotate(int& Left_R, int& Right_R, float& CR, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not);
void Tt_Rotate(int& xLeft, int& xRight, float& CR, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not, bool & cannot_sound);
void Tt_Auto_Up_Down(float& Cube_Lowest, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not, int& Game_Score);
void Tt_Move_Up_Down(int& yClose, int& yFront, float& Cube_Lowest, float& tt, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not);
void Tt_Move_UDLR(int& Tt_X_B, int& Tt_X_F, int& Tt_Y_B, int& Tt_Y_F, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not);

#endif // __KEYBOARD_H__