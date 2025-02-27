#ifndef __RENDER_H__
#define __RENDER_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "geometry.h"
#include "model.h"
#include "linalg.h"
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"

using namespace std;

void Plot_Cubes(SDL_Renderer* renderer, vector<PrimCube> &Cbs, vector<vector<float>> CamP, vector<vector<float>> CamK, vector<float> LightRay);
void Plot_Cubes_Menu(SDL_Renderer* renderer, vector<PrimCube>& Cbs, vector<vector<float>> CamP, vector<vector<float>> CamK, vector<float> LightRay);
void Record_Shadow(vector<PrimCube>& Cbs, vector<vector<float>> LightP, vector<vector<float>> LightK, float shadow_pool[120][6][2]);
void Record_Shadow_Menu(vector<PrimCube>& Cbs, vector<vector<float>> LightP, vector<vector<float>> LightK, float shadow_pool[120][6][2]);
bool IsBFace(const vector<float> &CamRay, const vector<float> &normal);
bool InFrustumZ(const vector<float> &vertex, float z_min, float z_max);

#endif // __RENDER_H__

#pragma once
