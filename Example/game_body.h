#ifndef __GAME_BODY_H__
#define __GAME_BODY_H__

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
#include "geometry.h"
#include "render.h"
#include "polygon_clipping.h"
#include "cube_hint.h"
#include "rules.h"

using namespace std;

void GameMainBody(bool& Game_over, int& Game_Score, int& collect_or_not, float& Cube_Lowest, Tetris& Base, Tetris_ALL& Tt, vector<PrimCube>& cubes, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location);
void Score2Text(int& Game_Score, char* score);

#endif // __GAME_BODY_H__