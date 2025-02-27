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

void LightAuto(float& LRx, float& LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float>& LightLoc, vector<float>& LightOri, vector<vector<float>>& LightP, vector<vector<float>>& LightOriMat, vector<float>& LightRay) {
	// Light Rotation
	
	/*LRx += 0.1;
	printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);*/
	
	
	/*LRx -= 1;
	printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);*/
	
	
	/*LRy += 0.1;
	printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);*/
	
	
	/*LRy -= 1;
	printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);*/

	
	LRz += 0.05;
	//printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
	if (abs(LRz - 360) < 0.5) {
		LRz = 0;
	}
	
	/*LRz -= 1;
	printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);*/
		
	LightLoc = { LTx, LTy, LTz }, LightOri = { LRx, LRy, LRz };
	LightP = RT_M(LightOri, LightLoc);
	LightRay = { 0, 0, 1 };
	LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3));

	LightRay = Squeeze(MM_Mul(LightOriMat, Unsqueeze(LightRay, 1)), 1);
}

void LightReset(float& LRx, float& LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float>& LightLoc, vector<float>& LightOri, vector<vector<float>>& LightP, vector<vector<float>>& LightOriMat, vector<float>& LightRay) {
	LRz = 20;
	LightLoc = { LTx, LTy, LTz }, LightOri = { LRx, LRy, LRz };
	LightP = RT_M(LightOri, LightLoc);
	LightRay = { 0, 0, 1 };
	LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3));

	LightRay = Squeeze(MM_Mul(LightOriMat, Unsqueeze(LightRay, 1)), 1);
}

void LightControl(int & Light_x_up, int & Light_x_do, int& Light_y_up, int & Light_y_do, int & Light_z_up, int & Light_z_do, float & LRx, float & LRy, float& LRz, float& LTx, float& LTy, float& LTz, vector<float> &LightLoc, vector<float>& LightOri, vector<vector<float>> &LightP, vector<vector<float>> &LightOriMat, vector<float>& LightRay) {
	// Light Rotation
	if (Light_x_up == 1) {
		LRx += 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_x_up = 0;
	}
	if (Light_x_do == 1) {
		LRx -= 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_x_do = 0;
	}
	if (Light_y_up == 1) {
		LRy += 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_y_up = 0;
	}
	if (Light_y_do == 1) {
		LRy -= 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_y_do = 0;
	}
	if (Light_z_up == 1) {
		LRz += 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_z_up = 0;
	}
	if (Light_z_do == 1) {
		LRz -= 1;
		printf("Light R (%lf, %lf, %lf)\n", LRx, LRy, LRz);
		Light_z_do = 0;
	}
	LightLoc = { LTx, LTy, LTz }, LightOri = { LRx, LRy, LRz };
	LightP = RT_M(LightOri, LightLoc);
	LightRay = { 0, 0, 1 };
	LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3));

	LightRay = Squeeze(MM_Mul(LightOriMat, Unsqueeze(LightRay, 1)), 1);
}

void Base_Rotate(int & Left_R, int& Right_R, float &CR, Tetris &Base, vector<PrimCube> &cubes, Tetris_ALL& Tt, vector<vector<float>> &Base_Now_Height, vector<vector<float>>& hint_location, int & collect_or_not) {
	float Rotate_Frame = 10.;
	if (Left_R == 1) {
		CR += Rotate_Frame;
		Base.RotateTetris({ 0, 0, Rotate_Frame }, cubes);
		for (int i = 0; i < 9; i++) {
			Base_Now_Height[i][0] = cubes[i].ObjTF[0][3];
			Base_Now_Height[i][1] = cubes[i].ObjTF[1][3];
		}
		if ((int)CR % 90 == 0) {
			printf("Base Turn Left  (+90), Now CR: %f\n", CR);
			RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			Left_R = 0;
		}
	}
	if (Right_R == 1) {
		CR -= Rotate_Frame;
		Base.RotateTetris({ 0, 0, -Rotate_Frame }, cubes);
		for (int i = 0; i < 9; i++) {
			Base_Now_Height[i][0] = cubes[i].ObjTF[0][3];
			Base_Now_Height[i][1] = cubes[i].ObjTF[1][3];
		}
		if ((int)CR % 90 == 0) {
			printf("Base Turn Right (-90), Now CR: %f\n", CR);
			RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			Right_R = 0;
		}
	}
	
}

void Tt_Rotate(int& xLeft, int& xRight, float& CR, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not, bool & cannot_sound) {
	float Rotate_Frame = 10.;
	if (xLeft == 1) {
		int CanR_L = 1;
		if ((int)CR % 90 == 0) {
			Tt.RotateTetris({ 0, 0, +90 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot R!\n");
				Tt.RotateTetris({ 0, 0, -90 }, cubes);
				CanR_L = 0;
				xLeft = 0;
				//cannot_sound = true;
			}
			else {
				Tt.RotateTetris({ 0, 0, -90 }, cubes);
			}
		}
		if (CanR_L == 1 && collect_or_not == 0) {
			CR += Rotate_Frame;
			Tt.RotateTetris({ 0, 0, Rotate_Frame }, cubes);
			//printf("Each Turn Left  (+10), Now CR: %f\n", CR);
			if ((int)CR % 90 == 0) {
				printf("Tetris Turn Left  (+90), Now CR: %f\n", CR);
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
				xLeft = 0;
			}
		}
		else {
			printf("Cannot R!\n");
			//cannot_sound = true;
			xLeft = 0;
		}
	}
	if (xRight == 1) {
		int CanR_R = 1;
		if ((int)CR % 90 == 0) {
			Tt.RotateTetris({ 0, 0, -90 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot R!\n");
				Tt.RotateTetris({ 0, 0, +90 }, cubes);
				CanR_R = 0;
				xRight = 0;
				//cannot_sound = true;
			}
			else {
				Tt.RotateTetris({ 0, 0, +90 }, cubes);
			}
		}
		if (CanR_R == 1 && collect_or_not == 0) {
			CR -= Rotate_Frame;
			Tt.RotateTetris({ 0, 0, -Rotate_Frame }, cubes);
			if ((int)CR % 90 == 0) {
				printf("Tetris Turn Right (-90), Now CR: %f\n", CR);
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
				xRight = 0;
			}
		}
		else {
			printf("Cannot R!\n");
			//cannot_sound = true;
			xRight = 0;
		}
	}
}

void Tt_Auto_Up_Down(float& Cube_Lowest, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not, int & Game_Score) {
	int speed = 6;
	Tt.MoveTetris({ 0, 0, (float)0.2 * speed }, cubes);
	RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
	Cube_Lowest = Tt.GetLowestHeight(cubes);
}

void Tt_Move_Up_Down(int& yClose, int& yFront, float &Cube_Lowest, float & tt, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not) {
	if (yClose == 1) {
		tt++;
		Tt.MoveTetris({ 0, 0, +0.2 * 6 }, cubes);
		RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
		Cube_Lowest = Tt.GetLowestHeight(cubes);
		//printf("Cube_Lowest H = %f\n", Cube_Lowest);
		yClose = 0;
	}
	if (yFront == 1) {
		tt--;
		Tt.MoveTetris({ 0, 0, -0.2 * 6 }, cubes);
		RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
		Cube_Lowest = Tt.GetLowestHeight(cubes);
		//printf("Cube_Lowest H = %f\n", Cube_Lowest);
		yFront = 0;
	}
}
void Tt_Move_UDLR(int& Tt_X_B, int& Tt_X_F, int& Tt_Y_B, int& Tt_Y_F, Tetris& Base, vector<PrimCube>& cubes, Tetris_ALL& Tt, vector<vector<float>>& Base_Now_Height, vector<vector<float>>& hint_location, int& collect_or_not) {
	// Move Tt
	if (Tt_X_B == 1) {
		if (collect_or_not == 0) {
			Tt.MoveTetris({ -1.2, 0, 0 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot!\n");
				Tt.MoveTetris({ +1.2, 0, 0 }, cubes);
			}
			else {
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			}
		}
		Tt_X_B = 0;
	}
	if (Tt_X_F == 1) {
		if (collect_or_not == 0) {
			Tt.MoveTetris({ +1.2, 0, 0 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot!\n");
				Tt.MoveTetris({ -1.2, 0, 0 }, cubes);
			}
			else {
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			}
		}
		Tt_X_F = 0;
	}
	if (Tt_Y_B == 1) {
		if (collect_or_not == 0) {
			Tt.MoveTetris({ 0, -1.2, 0 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot!\n");
				Tt.MoveTetris({ 0, +1.2, 0 }, cubes);
			}
			else {
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			}
		}
		Tt_Y_B = 0;
	}
	if (Tt_Y_F == 1) {
		if (collect_or_not == 0) {
			Tt.MoveTetris({ 0, +1.2, 0 }, cubes);
			if (!IsTtInsideBase(cubes, Tt, Base)) {
				printf("Cannot!\n");
				Tt.MoveTetris({ 0, -1.2, 0 }, cubes);
			}
			else {
				RenewHintLoc(cubes, Tt, Base, collect_or_not, Base_Now_Height, hint_location);
			}
		}
		Tt_Y_F = 0;
	}
}
