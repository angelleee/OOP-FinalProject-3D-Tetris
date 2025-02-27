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

void Plot_Cubes(SDL_Renderer* renderer, vector<PrimCube> &Cbs, vector<vector<float>> CamP, vector<vector<float>> CamK, vector<float> LightRay) { // drop fail QQ
	//cout << "Plot Cube" << endl;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	//SDL_RenderClear(renderer);
	vector<float> CamRay = { 0, 0, 1 };
	vector<vector<float>> CamOriMat = M_Transpose(SubM(CamP, 0, 3, 0, 3)); // R_Matrix 3 x 3, do R inverse (point to axis transformation) [Because R is z axis]
	CamRay = Squeeze(MM_Mul(CamOriMat, Unsqueeze(CamRay, 1)), 1); // (R_M) 3 x 3 * (CamRay) 3 x 1, World Coordinate x y z

	vector<float> Dis2Cam_Arr(Cbs.size());
	vector<int> Dis_Idx(Cbs.size());
	// Calculate Depth
	vector<float> ObjLoc(3);
	for (int i = 0; i < Cbs.size(); i++) {
		Dis_Idx[i] = i;
		for (int j = 0; j < ObjLoc.size(); j++) {
			ObjLoc[j] = Cbs[i].ObjTF[j][3];
		}
		Dis2Cam_Arr[i] = Dot(ObjLoc, CamRay);
	}
	Sort_Idx_Descend(Dis2Cam_Arr, Dis_Idx);
	vector<bool> infrustum(8, true);
	vector<vector<float>> verts_3d(8, vector<float>(3, 0)), verts_world2cam(8, vector<float>(3, 0));
	vector<vector<float>> verts_2d(8, vector<float>(2, 0));
	vector<vector<float>> matO2C(2, vector<float>(3, 0));
	vector<float> normal(3);
	vector<vector<float>> CubeOriMat(3, vector<float>(3, 0));
	Sint16 draw_sha_x[10] = { 0 };
	Sint16 draw_sha_y[10] = { 0 };
	Sint16 draw_x[4] = { 0 };
	Sint16 draw_y[4] = { 0 };
	for (int ii = 0; ii < Cbs.size(); ii++) {
		float DifMag = 0;
		int RR = 0, GG = 0, BB = 0, AA = 255;
		int oi = Dis_Idx[ii];
		int num_verts = 8; // Cbs[oi].verts.size();
		Transform_PTs(Cbs[oi].verts, Cbs[oi].ObjTF, verts_3d);
		Transform_PTs(verts_3d, CamP, verts_world2cam);
		
		for (int vi = 0; vi < num_verts; vi++) 
			infrustum[vi] = InFrustumZ(verts_world2cam[vi], 0.01, 100);
		bool CubeInFrustum = std::all_of(infrustum.begin(), infrustum.end(), [](bool flag) { return flag; });
		//cout << "AA" << endl;
		if (CubeInFrustum) {
			//cout << "BB" << endl;
			Project_PTs(verts_world2cam, CamK, 1.5, verts_2d);
			BlockAssign(CubeOriMat, Cbs[oi].ObjTF, 0, 3, 0, 3);
			for (int fi = 0; fi < Cbs[oi].faces.size(); fi++) {
				MV_Mul(CubeOriMat, Cbs[oi].normals[fi], normal); // 1 x 3 // normal only need rotation :)
				
				if (!IsBFace(CamRay, normal)) {
					GetMatOblique2Carte(verts_2d[Cbs[oi].faces[fi][0]], 
										verts_2d[Cbs[oi].faces[fi][1]], verts_2d[Cbs[oi].faces[fi][3]],
										matO2C);
					//Sint16 draw_x[4] = { 0 };
					//Sint16 draw_y[4] = { 0 };
					for (int vi = 0; vi < Cbs[oi].faces[fi].size(); vi++) {
						draw_x[vi] = (Sint16) (verts_2d[Cbs[oi].faces[fi][vi]][0] + 0.5);
						draw_y[vi] = (Sint16) (verts_2d[Cbs[oi].faces[fi][vi]][1] + 0.5);
					}
					DifMag = Dot(normal, LightRay); // 1 x 1 It is Cos_theta
					if (DifMag < 0)	DifMag = DifMag * (-1);
					else  DifMag = 0;
					// Add Ambient Reflection
					RR = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[0] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[0]);
					GG = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[1] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[1]);
					BB = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[2] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[2]);
					//if ((Cbs[oi].hint) && (fi == 5)) {
					//	RR = 255;
					//	GG = 0;
					//	BB = 0; //255, 233, 85
					//}
					if ((Cbs[oi].deadline)) {
						RR = 255; 
						GG = 0;
						BB = 0;
						AA = 160;
					}
					//cout << "ID = " << Cbs[oi].cube_id << ", Face = " << fi << ", (" << RR << ", " << GG << ", " << BB << ")" << endl;
					if (oi >= 9 && oi <= 17) {
						if (Cbs[oi].deadline==true){
							filledPolygonRGBA(renderer, draw_x, draw_y, 4, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), AA);
						}	
					}
					else {
						filledPolygonRGBA(renderer, draw_x, draw_y, 4, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), AA);
					}
					// Plot Shadow
					//if (Cbs[oi].tmp_shadow.size() > 0) { // 6 // tmp_shadow size = 6 x N(30 shadows) x K(10 shape in polygon) x 2
					if (Cbs[oi].shadow_num[fi] > 0) { // 6 // tmp_shadow size = 6 x N(30 shadows) x K(10 shape in polygon) x 2
						//if (Cbs[oi].tmp_shadow[fi].size() > 0) { // (Cbs[oi].tmp_shadow[fi].size() > 0) { // N
						if (oi < 9 || oi > 17){
							if (Cbs[oi].shadow_num[fi] > 0) {
								RR = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[0]);
								GG = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[1]);
								BB = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[2]);
								//int number_shadow = Cbs[oi].tmp_shadow[fi].size();
								int number_shadow = Cbs[oi].shadow_num[fi];
								for (int sha_i = 0; sha_i < number_shadow; sha_i++) {
									//int shadow_size = Cbs[oi].tmp_shadow[fi][sha_i].size();
									int shadow_size = Cbs[oi].shadow_poly_num[fi][sha_i];
									//Sint16* draw_sha_x = new Sint16[shadow_size]{ 0 };
									//Sint16* draw_sha_y = new Sint16[shadow_size]{ 0 };
									float tmp;
									for (int sha_vi = 0; sha_vi < shadow_size; sha_vi++) {
										//tmp = matO2C[0][0] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][0] + matO2C[0][1] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][1] + matO2C[0][2];
										tmp = matO2C[0][0] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][0] + matO2C[0][1] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][1] + matO2C[0][2];
										draw_sha_x[sha_vi] = (Sint16)(tmp + 0.5);
										//tmp = matO2C[1][0] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][0] + matO2C[1][1] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][1] + matO2C[1][2];
										tmp = matO2C[1][0] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][0] + matO2C[1][1] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][1] + matO2C[1][2];
										draw_sha_y[sha_vi] = (Sint16)(tmp + 0.5);
									}
									filledPolygonRGBA(renderer, draw_sha_x, draw_sha_y, shadow_size, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), 255);
									//delete[] draw_sha_x, draw_sha_y;
								}
							}
						}
					}
					if ((Cbs[oi].hint) && (fi == 5)) {
						RR = 255;
						GG = 0;
						BB = 0; //255, 233, 85
						filledPolygonRGBA(renderer, draw_x, draw_y, 4, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), AA);
					}
				}
			}
		}
		Cbs[oi].ResetShadow();
	}
}

void Record_Shadow(vector<PrimCube> &Cbs, vector<vector<float>> LightP, vector<vector<float>> LightK, float shadow_pool[120][6][2]) { // LightRay 
	vector<float> LightRay = { 0, 0, 1 };
	vector<vector<float>> LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3)); // R_Matrix 3 x 3, do R inverse (point to axis transformation) [Because R is z axis]
	LightRay = Squeeze(MM_Mul(LightOriMat, Unsqueeze(LightRay, 1)), 1); // (R_M) 3 x 3 * (CamRay) 3 x 1, World Coordinate x y z
	//float* Dis2Light_Arr = new float[Cbs.size()];
	//int* Dis_Idx = new int[Cbs.size()];
	vector<float> Dis2Light_Arr(Cbs.size());
	vector<int> Dis_Idx(Cbs.size());
	// Calculate Depth
	vector<float> ObjLoc(3);
	for (int i = 0; i < Cbs.size(); i++) {
		Dis_Idx[i] = i;
		for (int j = 0; j < ObjLoc.size(); j++) {
			ObjLoc[j] = Cbs[i].ObjTF[j][3];
		}
		Dis2Light_Arr[i] = Dot(ObjLoc, LightRay);
	}
	Sort_Idx_Descend(Dis2Light_Arr, Dis_Idx);

	//vector<vector<vector<float>>> shadow_pool; // N(cube numbers) * 3 (number of shadow), 4 (number of vertex), 2 (x, y coordinate) -> N x 6 x 2
	point2D clipPolygon[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };         // select 4 points from one of light-view face, now  face //
	//point2D subjectPolygon[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	point2D subjectPolygon[6]; // = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} };
	vector<vector<float>> matC2O(2, vector<float>(3, 0));
	vector<float> normal(3);
	vector<vector<float>> CubeOriMat(3, vector<float>(3, 0));
	vector<vector<float>> my_silhouette(4, vector<float>(2, 0)); // 4 x 2
	vector<vector<float>> verts_3d(8, vector<float>(3, 0)), verts_world2cam(8, vector<float>(3, 0));
	vector<vector<float>> verts_2d(8, vector<float>(2, 0));
	//vector<vector<float>> single_shadow;
	//vector<vector<vector<float>>> accum_silhouette; // N x K x 2
	vector<vector<float>> cube_silhouette(6, vector<float>(2, 0)); // 6 x 2
	//vector<vector<vector<float>>> accum_shadow; // size of tmp is N x K(polygon vertex) x 2 (x, y)
	vector<float> cube_once_3_face_idx;
	float matC2O_00, matC2O_01, matC2O_02, matC2O_10, matC2O_11, matC2O_12;
	int max_num = 0;
	int shadow_pool_count = 0;
	for (int ii = Cbs.size() - 1; ii >= 0; ii--) { // First from the smallest cubes (nearest cubes) // 3 * 3 * 11 * 6 * 30 * 10 * 2 * 4 (byte) => * 0.001(kb) => 1425.6(kb)
		int oi = Dis_Idx[ii];
		if (oi < 9 || oi > 17){
			int num_verts = Cbs[oi].verts.size(); // 8 vertex (each cube)
		
			Transform_PTs(Cbs[oi].verts, Cbs[oi].ObjTF, verts_3d);
			//verts_3d = Transform_PTs(Cbs[oi].verts, SubM(Cbs[oi].ObjTF, 0, 3, 0, 4)); //  8 x 3 operate with 3 x 4 -> 8 x 3 // convert cube vertex to world-coordinate
			//PrintMat(verts_3d, "verts_3d");
			Transform_PTs(verts_3d, LightP, verts_world2cam);
			Project_PTs(verts_world2cam, LightK, 1, verts_2d);   // convert to Light-view 2D position // 3D to 2D points
			BlockAssign(CubeOriMat, Cbs[oi].ObjTF, 0, 3, 0, 3);
			int face_to_light_idx = 0;
			int f_num = 0;
			//vector<int> f_idx;
			for (int fi = 0; fi < Cbs[oi].faces.size(); fi++) { // Cbs[oi].faces.size() is 6 // face
				MV_Mul(CubeOriMat, Cbs[oi].normals[fi], normal); // 1 x 3 // normal only need rotation :)
				for (int vi = 0; vi < Cbs[oi].faces[fi].size(); vi++) { // Cbs[oi].faces[fi].size() is 4 (one face has 4 points) // face's vertex
					my_silhouette[vi] = verts_2d[Cbs[oi].faces[fi][vi]]; // Light-view 2D position 4 x 2 
				}
				if (IsBFace(LightRay, normal)) { // clock-wise // if is Back Face , the shadow is itself // not necessary
					//accum_shadow.push_back({ {0, 0}, {0, 0}, {0, 0}, {0, 0} }); // other light project to this face // so accum has to draw itself
				}
				else { // face to light
					//f_num += 1;
					face_to_light_idx += fi;
					//f_idx.push_back(fi);
					GetMatCarte2Oblique(my_silhouette[0], my_silhouette[1], my_silhouette[3], matC2O); // 2 x 3
					//PrintMat(matC2O, "matC2O");
					//PrintMat({ {shadow_pool[sha_i][xx]}, {shadow_pool[sha_i][xx]}, {1} }, "matC2O");
					matC2O_00 = matC2O[0][0]; matC2O_01 = matC2O[0][1]; matC2O_02 = matC2O[0][2];
					matC2O_10 = matC2O[1][0]; matC2O_11 = matC2O[1][1]; matC2O_12 = matC2O[1][2];
					//reverse(my_silhouette.begin(), my_silhouette.end()); // counter clock-wise // due to format, we have to do counter clock-wise
					//printf("Cube %d, face %d, have %d shadow, shadow_pool shape: %d\n", oi, fi, accum_shadow.size(), shadow_pool.size());
					int counter_shadow_idx = 0;
					//for (int sha_i = 0; sha_i < shadow_pool.size(); sha_i++) { // Plot should from original point (reverse again pick idx 0) // Each shadow
					for (int sha_i = 0; sha_i < shadow_pool_count; sha_i++) {
						point2D newPolygon[MAX_POLYGON] = { 0 };
						int subjectPolygonSize = 6, clipPolygonSize = my_silhouette.size(), newPolygonSize = 0;
						//for (int xx = 0; xx < shadow_pool[sha_i].size(); xx++) {
						for (int xx = 0; xx < 6; xx++) {
							subjectPolygon[xx].x = matC2O_00 * shadow_pool[sha_i][xx][0] + matC2O_01 * shadow_pool[sha_i][xx][1] + matC2O_02;
							subjectPolygon[xx].y = matC2O_10 * shadow_pool[sha_i][xx][0] + matC2O_11 * shadow_pool[sha_i][xx][1] + matC2O_12;
						}
						//subjectPolygon = Vec2D_2_Vecpoiont2D(shadow_pool[sha_i], true); // select 4 points from one of light-view face, prev face
						//clipPolygon = Vec2D_2_Vecpoiont2D(my_silhouette, true);         // select 4 points from one of light-view face, now  face //
						//subjectPolygon = Vec2D_2_Vecpoiont2D(rect_shadow, false); // select 4 points from one of light-view face, prev face
						//for (int xx = 0; xx < rect_shadow.size(); xx++) {
						//	subjectPolygon[xx].x = rect_shadow[xx][0];
						//	subjectPolygon[xx].y = rect_shadow[xx][1];
						//}
						//clipPolygon = Vec2D_2_Vecpoiont2D({ {0, 0}, {1, 0}, {1, 1}, {0, 1} }, false);         // select 4 points from one of light-view face, now  face //
						SutherlandHodgman(subjectPolygon, subjectPolygonSize, clipPolygon, clipPolygonSize, newPolygon, newPolygonSize);
						//printf("newPolygonSize: %d\n", newPolygonSize);
						//vector<vector<float>> tmp;
						//if (newPolygonSize > 0)
						//	cout << "ID = " << Cbs[oi].cube_id << ", Face = " << fi << endl;
						//	PrintMat(matC2O, "matC2O");
						//	PrintMat(M_Transpose(rect_shadow), "subjectPolygon");
						//	PrintMat(M_Transpose({ {0, 0}, {1, 0}, {1, 1}, {0, 1} }), "clipPolygon");

						for (int xx = 0; xx < newPolygonSize; xx++) { // counter shadow idx += 1
							//tmp = { { newPolygon[xx].x }, {newPolygon[xx].y}, {1} };
							//cout << "inter : " << newPolygon[xx].x << ", " << newPolygon[xx].y << endl;
							//tmp = MM_Mul(matC2O, tmp);
							//cout << "  tmp : " << tmp[0][0] << ", " << tmp[1][0] << endl;
							//PrintMat(tmp, "tmp");
							//single_shadow.push_back({ min(max(newPolygon[xx].x, (float)0), (float)1), min(max(newPolygon[xx].y, (float)0), (float)1) });
							Cbs[oi].tmp_shadow_static[fi][counter_shadow_idx][xx][0] = min(max(newPolygon[xx].x, (float)0), (float)1);
							Cbs[oi].tmp_shadow_static[fi][counter_shadow_idx][xx][1] = min(max(newPolygon[xx].y, (float)0), (float)1);
							//cout << "  sha : " << min(max(tmp[0][0], (float)0), (float)1) << ", " << min(max(tmp[1][0], (float)0), (float)1) << endl;
						}
						if (newPolygonSize > 0){
							Cbs[oi].shadow_poly_num[fi][counter_shadow_idx] = newPolygonSize;
							counter_shadow_idx += 1;
						}
						// Concate Shadow and 
						//if (newPolygonSize)
						//	accum_shadow.push_back(single_shadow); // transfor to Square and then tansfer to camera-view
						//Clear2DVector(single_shadow);	
					}
					Cbs[oi].shadow_num[fi] = counter_shadow_idx;
					//printf("Cube %d, face %d, have %d shadow, polygon shape: %d\n", oi, fi, accum_shadow.size(), accum_shadow[1].size());
					//accum_silhouette.push_back(my_silhouette); // This face's silhouette project to other place // 3 x 4 x 2
				}
				/*printf("Cbs: %d, F num : %d, value: %d\n", oi, f_idx.size(), face_to_light_idx);
				for (int i = 0; i < f_idx.size(); i++) {
					printf("%d ", f_idx[i]);
				}
				printf("\n======\n");*/
				switch (face_to_light_idx) {

					case 7: // 0 2 5 {0, 1, 2, 6, 7, 4} in face idx
						cube_once_3_face_idx = { 0, 1, 2, 6, 7, 4 };
						break;
					case 8: // 0 3 5 {0, 3, 7, 6, 5, 1}
						cube_once_3_face_idx = { 0, 3, 7, 6, 5, 1 };
						break;
					case 11: // 2 4 5 {0, 3, 2, 6, 5, 4}
						cube_once_3_face_idx = { 0, 3, 2, 6, 5, 4 };
						break;
					case 12: // 3 4 5 {1, 5, 4, 7, 3, 2}
						cube_once_3_face_idx = { 1, 5, 4, 7, 3, 2 };
						break;
				}
				/*printf("cube_once_3_face_idx: %d\n", cube_once_3_face_idx.size());*/ 
				for (int vi = 0; vi < cube_once_3_face_idx.size(); vi++) { // Cbs[oi].faces[fi].size() is 4 (one face has 4 points) // face's vertex
					cube_silhouette[vi][0] = verts_2d[cube_once_3_face_idx[vi]][0]; // Light-view 2D position 6 x 2 
					cube_silhouette[vi][1] = verts_2d[cube_once_3_face_idx[vi]][1];
				}
				//printf("Cube %d, face %d, have %d shadow, shadow_pool shape: %d\n", oi, fi, accum_shadow.size(), shadow_pool.size());
				//Cbs[oi].tmp_shadow.push_back(accum_shadow);
				//Clear3DVector(accum_shadow);
			}
			//for (int xx = 0; xx < accum_silhouette.size(); xx++)	
			//	shadow_pool.push_back(accum_silhouette[xx]); // push_back 3 times, can only push back once // cube_silhouette <- 6 x 2
			if (face_to_light_idx != 0){
				for (int mf = 0; mf < 6; mf++) {
					shadow_pool[shadow_pool_count][mf][0] = cube_silhouette[mf][0];
					shadow_pool[shadow_pool_count][mf][1] = cube_silhouette[mf][1];
				}
				shadow_pool_count += 1;
				//shadow_pool.push_back(cube_silhouette);
			}
			//Clear3DVector(accum_silhouette); // 
			//printf("shadow_pool size: %d\n", shadow_pool.size());
			//printf("shadow_pool size: %d\n", shadow_pool_count);
		}
	}
	//Clear3DVector(shadow_pool);
}

bool IsBFace(const vector<float> &CamRay, const vector<float> &normal) {
	return (Dot(CamRay, normal) > 0.0); // if angle is too small, see it as back face
}

bool InFrustumZ(const vector<float> &vertex, float z_min, float z_max) {
	return (vertex[2] > z_min) && (vertex[2] < z_max);
}

void Each_Face_Sort(vector<float> & Dis2Cam_Arr_6_face, vector<int> & Dis_Idx_6_face, vector<PrimCube>& Cbs, vector<float>& CamRay) {
	Dis2Cam_Arr_6_face[0] = Dot({ Cbs[0].ObjTF[0][3] , Cbs[0].ObjTF[1][3] , Cbs[0].ObjTF[2][3] }, CamRay);
	Dis_Idx_6_face[0] = 0;
	vector<float> ObjLoc_6_face(3);
	for (int i = 1; i < 7; i++) {
		Dis_Idx_6_face[i] = i;
		for (int j = 0; j < ObjLoc_6_face.size(); j++) {
			ObjLoc_6_face[j] = Cbs[i + 66].ObjTF[j][3];
		}
		Dis2Cam_Arr_6_face[i] = Dot(ObjLoc_6_face, CamRay);
	}
	Sort_Idx_Descend(Dis2Cam_Arr_6_face, Dis_Idx_6_face);
}

void Each_Tetris_Sort(vector<int>& Dis_Idx_All, vector<int> & Dis_Idx_6_face, vector<PrimCube>& Cbs, vector<float> & CamRay) {
	vector<int> num = {1, 9, 13, 9, 12, 10, 13};
	vector<vector<int>> start_end = { {0, 0}, {1, 9}, {10, 22}, {23, 31}, {32, 43}, {44, 53}, {54, 66} };
	for (int i = 0; i < Dis_Idx_6_face.size(); i++) {
		if (Dis_Idx_6_face[i] == 0) {
			Dis_Idx_All.push_back(0);
		}
		else {
			vector<float> Dis2Cam_Arr_Each(num[Dis_Idx_6_face[i]]);
			vector<int> Dis_Idx_Each;
			for (int j = start_end[Dis_Idx_6_face[i]][0]; j <= start_end[Dis_Idx_6_face[i]][1]; j++) {
				Dis2Cam_Arr_Each[j - start_end[Dis_Idx_6_face[i]][0]] = Dot({ Cbs[j].ObjTF[0][3],  Cbs[j].ObjTF[1][3],  Cbs[j].ObjTF[2][3]}, CamRay);
				Dis_Idx_Each.push_back(j);
			}
			Sort_Idx_Descend(Dis2Cam_Arr_Each, Dis_Idx_Each);
			for (int k = 0; k < Dis_Idx_Each.size(); k++) {
				Dis_Idx_All.push_back(Dis_Idx_Each[k]);
			}
		}
	}
	/*printf("All size : %d\n", Dis_Idx_All.size());
	for (int i = 0; i < Dis_Idx_All.size(); i++) {
		printf("%d ", Dis_Idx_All[i]);
	}
	printf("\n========QAQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n");*/
}

void Plot_Cubes_Menu(SDL_Renderer* renderer, vector<PrimCube>& Cbs, vector<vector<float>> CamP, vector<vector<float>> CamK, vector<float> LightRay) { // drop fail QQ
	//cout << "Plot Cube" << endl;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	//SDL_RenderClear(renderer);
	vector<float> CamRay = { 0, 0, 1 };
	vector<vector<float>> CamOriMat = M_Transpose(SubM(CamP, 0, 3, 0, 3)); // R_Matrix 3 x 3, do R inverse (point to axis transformation) [Because R is z axis]
	CamRay = Squeeze(MM_Mul(CamOriMat, Unsqueeze(CamRay, 1)), 1); // (R_M) 3 x 3 * (CamRay) 3 x 1, World Coordinate x y z
	
	// OLD ORDERRR ========================>
	/*vector<float> Dis2Cam_Arr(Cbs.size());
	vector<int> Dis_Idx(Cbs.size());
	// Calculate Depth
	vector<float> ObjLoc(3);
	for (int i = 0; i < Cbs.size(); i++) {
		Dis_Idx[i] = i;
		for (int j = 0; j < ObjLoc.size(); j++) {
			ObjLoc[j] = Cbs[i].ObjTF[j][3];
		}
		if (i == 0) {
			for (int j = 0; j < Cbs[i].verts.size(); j++) {
				//printf("Big Cbs: x=%f, y=%f, z=%f\n", Cbs[i].verts[j][0] + ObjLoc[0], Cbs[i].verts[j][1] + ObjLoc[1], Cbs[i].verts[j][2] + ObjLoc[2]);
			}
		}
		//printf("OBBBB %d =>> x=%f, y=%f, z=%f\n",i,  ObjLoc[0], ObjLoc[1], ObjLoc[2]);
		Dis2Cam_Arr[i] = Dot(ObjLoc, CamRay);
	}
	Sort_Idx_Descend(Dis2Cam_Arr, Dis_Idx);*/

	// YEHHHHHH NEWWWW ORDERRR ========================>
	vector<float> Dis2Cam_Arr_6_face(7);
	vector<int> Dis_Idx_6_face(7);
	Each_Face_Sort(Dis2Cam_Arr_6_face, Dis_Idx_6_face, Cbs, CamRay);
	/*printf("\nIdx Order: ");
	for (int i = 0; i < 7; i++) {
		printf("%d ", Dis_Idx_6_face[i]);
	}
	printf("=======Face Idx Order Done===============\n");*/
	vector<int> Dis_Idx_new;
	Each_Tetris_Sort(Dis_Idx_new, Dis_Idx_6_face, Cbs, CamRay);
	// YEHHHHHHH NEWWWWW DONEEEEE ======================>

	vector<bool> infrustum(8, true);
	vector<vector<float>> verts_3d(8, vector<float>(3, 0)), verts_world2cam(8, vector<float>(3, 0));
	vector<vector<float>> verts_2d(8, vector<float>(2, 0));
	vector<vector<float>> matO2C(2, vector<float>(3, 0));
	vector<float> normal(3);
	vector<vector<float>> CubeOriMat(3, vector<float>(3, 0));
	Sint16 draw_sha_x[10] = { 0 };
	Sint16 draw_sha_y[10] = { 0 };
	Sint16 draw_x[4] = { 0 };
	Sint16 draw_y[4] = { 0 };
	
	for (int ii = 0; ii < Cbs.size()-6; ii++) {
		float DifMag = 0;
		int RR = 0, GG = 0, BB = 0, AA = 255;
		//int oi = Dis_Idx[ii]; 
		int oi = Dis_Idx_new[ii];
		int num_verts = 8; // Cbs[oi].verts.size();
		Transform_PTs(Cbs[oi].verts, Cbs[oi].ObjTF, verts_3d);
		Transform_PTs(verts_3d, CamP, verts_world2cam);

		for (int vi = 0; vi < num_verts; vi++)
			infrustum[vi] = InFrustumZ(verts_world2cam[vi], 0.01, 100);
		bool CubeInFrustum = std::all_of(infrustum.begin(), infrustum.end(), [](bool flag) { return flag; });
		//cout << "AA" << endl;
		if (CubeInFrustum) {
			//cout << "BB" << endl;
			Project_PTs(verts_world2cam, CamK, 1.5, verts_2d);
			BlockAssign(CubeOriMat, Cbs[oi].ObjTF, 0, 3, 0, 3);
			for (int fi = 0; fi < Cbs[oi].faces.size(); fi++) {
				MV_Mul(CubeOriMat, Cbs[oi].normals[fi], normal); // 1 x 3 // normal only need rotation :)
				if (!IsBFace(CamRay, normal)) {
					GetMatOblique2Carte(verts_2d[Cbs[oi].faces[fi][0]],
						verts_2d[Cbs[oi].faces[fi][1]], verts_2d[Cbs[oi].faces[fi][3]],
						matO2C);
					//Sint16 draw_x[4] = { 0 };
					//Sint16 draw_y[4] = { 0 };
					for (int vi = 0; vi < Cbs[oi].faces[fi].size(); vi++) {
						draw_x[vi] = (Sint16)(verts_2d[Cbs[oi].faces[fi][vi]][0] + 0.5);
						draw_y[vi] = (Sint16)(verts_2d[Cbs[oi].faces[fi][vi]][1] + 0.5);
					}
					DifMag = Dot(normal, LightRay); // 1 x 1 It is Cos_theta
					if (DifMag < 0)	DifMag = DifMag * (-1);
					else  DifMag = 0;
					// Add Ambient Reflection
					RR = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[0] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[0]);
					GG = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[1] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[1]);
					BB = (int)(Cbs[oi].lamb_coef * Cbs[oi].lamb_color[2] * DifMag + Cbs[oi].amb_coef * Cbs[oi].amb_color[2]);
					
					//cout << "ID = " << Cbs[oi].cube_id << ", Face = " << fi << ", (" << RR << ", " << GG << ", " << BB << ")" << endl;
					filledPolygonRGBA(renderer, draw_x, draw_y, 4, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), AA);
					// Plot Shadow
					//if (Cbs[oi].tmp_shadow.size() > 0) { // 6 // tmp_shadow size = 6 x N(30 shadows) x K(10 shape in polygon) x 2
					if (Cbs[oi].shadow_num[fi] > 0) { // 6 // tmp_shadow size = 6 x N(30 shadows) x K(10 shape in polygon) x 2
						//if (Cbs[oi].tmp_shadow[fi].size() > 0) { // (Cbs[oi].tmp_shadow[fi].size() > 0) { // N
						if (Cbs[oi].shadow_num[fi] > 0) {
							RR = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[0]);
							GG = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[1]);
							BB = (int)(Cbs[oi].amb_coef * Cbs[oi].amb_color[2]);
							//int number_shadow = Cbs[oi].tmp_shadow[fi].size();
							int number_shadow = Cbs[oi].shadow_num[fi];
							for (int sha_i = 0; sha_i < number_shadow; sha_i++) {
								//int shadow_size = Cbs[oi].tmp_shadow[fi][sha_i].size();
								int shadow_size = Cbs[oi].shadow_poly_num[fi][sha_i];
								//Sint16* draw_sha_x = new Sint16[shadow_size]{ 0 };
								//Sint16* draw_sha_y = new Sint16[shadow_size]{ 0 };
								float tmp;
								for (int sha_vi = 0; sha_vi < shadow_size; sha_vi++) {
									//tmp = matO2C[0][0] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][0] + matO2C[0][1] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][1] + matO2C[0][2];
									tmp = matO2C[0][0] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][0] + matO2C[0][1] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][1] + matO2C[0][2];
									draw_sha_x[sha_vi] = (Sint16)(tmp + 0.5);
									//tmp = matO2C[1][0] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][0] + matO2C[1][1] * Cbs[oi].tmp_shadow[fi][sha_i][sha_vi][1] + matO2C[1][2];
									tmp = matO2C[1][0] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][0] + matO2C[1][1] * Cbs[oi].tmp_shadow_static[fi][sha_i][sha_vi][1] + matO2C[1][2];
									draw_sha_y[sha_vi] = (Sint16)(tmp + 0.5);
								}
								filledPolygonRGBA(renderer, draw_sha_x, draw_sha_y, shadow_size, min(max(RR, 0), 255), min(max(GG, 0), 255), min(max(BB, 0), 255), 255);
								//delete[] draw_sha_x, draw_sha_y;
							}
						}
					}
				}
			}
		}
		Cbs[oi].ResetShadow();
	}
}

void Record_Shadow_Menu(vector<PrimCube>& Cbs, vector<vector<float>> LightP, vector<vector<float>> LightK, float shadow_pool[120][6][2]) { // LightRay 
	vector<float> LightRay = { 0, 0, 1 };
	vector<vector<float>> LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3)); // R_Matrix 3 x 3, do R inverse (point to axis transformation) [Because R is z axis]
	LightRay = Squeeze(MM_Mul(LightOriMat, Unsqueeze(LightRay, 1)), 1); // (R_M) 3 x 3 * (CamRay) 3 x 1, World Coordinate x y z
	
	// Old Order
	/*vector<float> Dis2Light_Arr(Cbs.size());
	vector<int> Dis_Idx(Cbs.size());
	// Calculate Depth
	vector<float> ObjLoc(3);
	for (int i = 0; i < Cbs.size(); i++) {
		Dis_Idx[i] = i;
		for (int j = 0; j < ObjLoc.size(); j++) {
			ObjLoc[j] = Cbs[i].ObjTF[j][3];
		}
		Dis2Light_Arr[i] = Dot(ObjLoc, LightRay);
	}
	Sort_Idx_Descend(Dis2Light_Arr, Dis_Idx);*/

	// YEHHHHHH NEWWWW ORDERRR ========================>
	vector<float> Dis2Cam_Arr_6_face(7);
	vector<int> Dis_Idx_6_face(7);
	Each_Face_Sort(Dis2Cam_Arr_6_face, Dis_Idx_6_face, Cbs, LightRay);
	vector<int> Dis_Idx_new;
	Each_Tetris_Sort(Dis_Idx_new, Dis_Idx_6_face, Cbs, LightRay);
	// YEHHHHHHH NEWWWWW DONEEEEE ======================>
	
	//vector<vector<vector<float>>> shadow_pool; // N(cube numbers) * 3 (number of shadow), 4 (number of vertex), 2 (x, y coordinate) -> N x 6 x 2
	point2D clipPolygon[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };         // select 4 points from one of light-view face, now  face //
	//point2D subjectPolygon[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	point2D subjectPolygon[6]; // = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} };
	vector<vector<float>> matC2O(2, vector<float>(3, 0));
	vector<float> normal(3);
	vector<vector<float>> CubeOriMat(3, vector<float>(3, 0));
	vector<vector<float>> my_silhouette(4, vector<float>(2, 0)); // 4 x 2
	vector<vector<float>> verts_3d(8, vector<float>(3, 0)), verts_world2cam(8, vector<float>(3, 0));
	vector<vector<float>> verts_2d(8, vector<float>(2, 0));
	//vector<vector<float>> single_shadow;
	//vector<vector<vector<float>>> accum_silhouette; // N x K x 2
	vector<vector<float>> cube_silhouette(6, vector<float>(2, 0)); // 6 x 2
	//vector<vector<vector<float>>> accum_shadow; // size of tmp is N x K(polygon vertex) x 2 (x, y)
	vector<float> cube_once_3_face_idx;
	float matC2O_00, matC2O_01, matC2O_02, matC2O_10, matC2O_11, matC2O_12;
	int max_num = 0;
	int shadow_pool_count = 0;
	
	for (int ii = Cbs.size()-6 - 1; ii >= 0; ii--) { // First from the smallest cubes (nearest cubes) // 3 * 3 * 11 * 6 * 30 * 10 * 2 * 4 (byte) => * 0.001(kb) => 1425.6(kb)
		//int oi = Dis_Idx[ii];
		int oi = Dis_Idx_new[ii];
		int num_verts = Cbs[oi].verts.size(); // 8 vertex (each cube)
		Transform_PTs(Cbs[oi].verts, Cbs[oi].ObjTF, verts_3d);
		Transform_PTs(verts_3d, LightP, verts_world2cam);
		Project_PTs(verts_world2cam, LightK, 1, verts_2d);   // convert to Light-view 2D position // 3D to 2D points
		BlockAssign(CubeOriMat, Cbs[oi].ObjTF, 0, 3, 0, 3);
		int face_to_light_idx = 0;
		int f_num = 0;
		//printf("\nface: ");	
		vector<int> ff;
		for (int fi = 0; fi < Cbs[oi].faces.size(); fi++) { // Cbs[oi].faces.size() is 6 // face
			//printf("Into new Face============>\n");
			MV_Mul(CubeOriMat, Cbs[oi].normals[fi], normal); // 1 x 3 // normal only need rotation :)
			for (int vi = 0; vi < Cbs[oi].faces[fi].size(); vi++) { // Cbs[oi].faces[fi].size() is 4 (one face has 4 points) // face's vertex
				my_silhouette[vi] = verts_2d[Cbs[oi].faces[fi][vi]]; // Light-view 2D position 4 x 2 
			}
			if (IsBFace(LightRay, normal)) { // clock-wise // if is Back Face , the shadow is itself // not necessary
				//accum_shadow.push_back({ {0, 0}, {0, 0}, {0, 0}, {0, 0} }); // other light project to this face // so accum has to draw itself
			}
			else { // face to light
				face_to_light_idx += fi;
				//printf("%d ", fi);
				ff.push_back(fi);
				GetMatCarte2Oblique(my_silhouette[0], my_silhouette[1], my_silhouette[3], matC2O); // 2 x 3
				matC2O_00 = matC2O[0][0]; matC2O_01 = matC2O[0][1]; matC2O_02 = matC2O[0][2];
				matC2O_10 = matC2O[1][0]; matC2O_11 = matC2O[1][1]; matC2O_12 = matC2O[1][2];
				int counter_shadow_idx = 0;
				for (int sha_i = 0; sha_i < shadow_pool_count; sha_i++) {
					point2D newPolygon[MAX_POLYGON] = { 0 };
					int subjectPolygonSize = 6, clipPolygonSize = my_silhouette.size(), newPolygonSize = 0;
					for (int xx = 0; xx < 6; xx++) {
						subjectPolygon[xx].x = matC2O_00 * shadow_pool[sha_i][xx][0] + matC2O_01 * shadow_pool[sha_i][xx][1] + matC2O_02;
						subjectPolygon[xx].y = matC2O_10 * shadow_pool[sha_i][xx][0] + matC2O_11 * shadow_pool[sha_i][xx][1] + matC2O_12;
					}
					SutherlandHodgman(subjectPolygon, subjectPolygonSize, clipPolygon, clipPolygonSize, newPolygon, newPolygonSize);

					for (int xx = 0; xx < newPolygonSize; xx++) { // counter shadow idx += 1
						Cbs[oi].tmp_shadow_static[fi][counter_shadow_idx][xx][0] = min(max(newPolygon[xx].x, (float)0), (float)1);
						Cbs[oi].tmp_shadow_static[fi][counter_shadow_idx][xx][1] = min(max(newPolygon[xx].y, (float)0), (float)1);
					}
					if (newPolygonSize > 0) {
						Cbs[oi].shadow_poly_num[fi][counter_shadow_idx] = newPolygonSize;
						counter_shadow_idx += 1;
					}
				}
				Cbs[oi].shadow_num[fi] = counter_shadow_idx;
			}
			/*printf("Size ff: %d, ", ff.size());
			for (int i = 0; i < ff.size(); i++) {
				printf("%d ",ff[i]);
			}
			printf("\n");*/
			//printf("Total v: %d\n", face_to_light_idx);	
			if (ff.size() == 3) {
				switch (face_to_light_idx) {
				case 3: // 0 1 2 {1, 2, 3, 7, 4, 5}
					if (ff[2] == 2) {
						cube_once_3_face_idx = { 1, 2, 3, 7, 4, 5 };
					}
				case 4: // 0 1 3 {0, 4, 5, 6, 2, 3}
					if (ff[2] == 3)
						cube_once_3_face_idx = { 0, 4, 5, 6, 2, 3 };
				case 7: // 0 2 5 {0, 1, 2, 6, 7, 4} in face idx
					if (ff[2] == 5) {
						cube_once_3_face_idx = { 0, 1, 2, 6, 7, 4 };
						//printf("\nCase1 7 fi: %d\n", ff[2]);
					}
					else if (ff[2] == 4) { // 1 2 4
						cube_once_3_face_idx = { 0, 3, 7, 6, 5, 1 };
						//printf("\nCase2 7 fi: %d\n", ff[2]);
					}
					break;
				case 8: // 0 3 5 {0, 3, 7, 6, 5, 1} // 1 3 4 {0, 4, 7, 6, 2, 1}
					if (ff[2] == 5)
						cube_once_3_face_idx = { 0, 3, 7, 6, 5, 1 };
					break;
				case 11: // 2 4 5 {0, 3, 2, 6, 5, 4}
					if (ff[2] == 5)
						cube_once_3_face_idx = { 0, 3, 2, 6, 5, 4 };
					break;
				case 12: // 3 4 5 {1, 5, 4, 7, 3, 2}
					if (ff[2] == 5)
						cube_once_3_face_idx = { 1, 5, 4, 7, 3, 2 };
					break;
				}
			}
			else {
				/*printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR SIZE: %d\n", ff.size());
				for (int i = 0; i < ff.size(); i++) {
					printf("%d ", ff[i]);
				}
				printf("\n");*/ // size == 5 ??
			}
		}
		//printf("\n");
		
		for (int vi = 0; vi < cube_once_3_face_idx.size(); vi++) { // Cbs[oi].faces[fi].size() is 4 (one face has 4 points) // face's vertex
			cube_silhouette[vi][0] = verts_2d[cube_once_3_face_idx[vi]][0]; // Light-view 2D position 6 x 2 
			cube_silhouette[vi][1] = verts_2d[cube_once_3_face_idx[vi]][1];
		}

		if (face_to_light_idx != 0) {
			for (int mf = 0; mf < 6; mf++) {
				shadow_pool[shadow_pool_count][mf][0] = cube_silhouette[mf][0];
				shadow_pool[shadow_pool_count][mf][1] = cube_silhouette[mf][1];
			}
			shadow_pool_count += 1;
		}
	}
}