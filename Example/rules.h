#ifndef __RULES_H__
#define __RULES_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "geometry.h"
#include "model.h"
#include "linalg.h"
#include "render.h"
#include "myalgo.h"
#include "polygon_clipping.h"
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "tetris.h"
#include "tetris_L.h"
#include "tetris_all.h"
#include "tetris_T.h"
#include "cube_hint.h"

using namespace std;

Tetris_ALL add_new_tetris(int tetris_type, vector<PrimCube>& cubes, Tetris& Base);
bool IsTtInsideBase(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base);
void RenewHintLoc(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base, int collect_or_not, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location);
void Check_Deadline_Block(vector<PrimCube>& cubes, vector<vector<float>>& Base_Now_Height);
void Game_Over_Or_Not(bool& Game_over, int Game_Score, vector<vector<float>>& Base_Now_Height);
//bool IsCollide(vector<PrimCube>& cubes, Tetris_ALL& Tt, Tetris& Base);

#endif // __RULES_H__

#pragma once
