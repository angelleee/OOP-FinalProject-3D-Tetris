#ifndef __CUBE_HINT_H__
#define __CUBE_HINT_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "linalg.h"
#include "model.h"
#include "tetris.h"
#include "tetris_all.h"

using namespace std;

vector<vector<float>> Find_Hint_Location(vector<PrimCube>& cubes, Tetris_ALL Tt);
void Set_Hint_Location(vector<int>& now_T, vector<PrimCube>& cubes, vector<int>& hint_idx, int &now_id);
void Find_Hint_Height(vector<int>& hint_idx, vector<int>& now_T, vector<PrimCube>& base_cubes, vector<vector<float>>& hint_location, vector<float>& hint_height, vector<vector<float>>& hint_verts, int& now_id);
void printf_hint_info(vector<vector<float>>& hint_verts, vector<int>& hint_idx, vector<float>& hint_height);
int count_hint_num(vector<int>& hint_idx);

#endif // __CUBE_HINT_H__

#pragma once