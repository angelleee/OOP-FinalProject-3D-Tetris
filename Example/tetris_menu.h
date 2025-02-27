#ifndef __TETRIS_MENU_H__
#define __TETRIS_MENU_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "linalg.h"
#include "geometry.h"
#include "model.h"

using namespace std;

struct Tetris_Menu {
	vector<int> ids;// = { 0, 1, 2, 3 }; // {0 ,1 ,2 ,3}
	vector<vector<float>> ObjTF = GetIdentity(4);; // 3 x 4 // R | T in world Coordinate
	vector<vector<float>> CubeLocs; // N x 3
	vector<float> center = { 0, 0, 0 }; // init
	float cube_H = -4.6;
	float each_c2cDis = 0.7;
	float cube_H_E = -4.6 + each_c2cDis * 3;
	float cube_H_2 = +2.6;
	float cube_S_H = +4.2; //QAQ
	vector<vector<float>> base_loc = { { 1, 1, -1 }, // 9 x 3 
										// T 9 cubes [z (-)4.6 is same]
										{ -(float)0.5*each_c2cDis, -(float)0.5 * each_c2cDis, cube_H  }, // row 1
										{ +(float)0.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H  },
										{ +(float)1.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H  },
										{ +(float)2.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H  },
										{ +(float)3.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H  },
										{ +(float)1.5 * each_c2cDis, +(float)0.5 * each_c2cDis, cube_H  }, // row 2
										{ +(float)1.5 * each_c2cDis, +(float)1.5 * each_c2cDis, cube_H  }, // row 3
										{ +(float)1.5 * each_c2cDis, +(float)2.5 * each_c2cDis, cube_H  }, // row 4
										{ +(float)1.5 * each_c2cDis, +(float)3.5 * each_c2cDis, cube_H  }, // row 5
										/*// T 20 cubes [z (-)4.6 is same]
										{ -each_c2cDis, -each_c2cDis, cube_H  },
										{   0    , -each_c2cDis, cube_H  },
										{ +each_c2cDis, -each_c2cDis, cube_H },
										{ +2*each_c2cDis, -each_c2cDis, cube_H },
										{ +3*each_c2cDis, -each_c2cDis, cube_H },
										{ +4*each_c2cDis, -each_c2cDis, cube_H },//
										{ -each_c2cDis, 0	  , cube_H},
										{ 	0	 , 0      , cube_H },
										{ +each_c2cDis, 0      , cube_H },
										{ +2*each_c2cDis, 0      , cube_H },
										{ +3*each_c2cDis, 0      , cube_H },
										{ +4*each_c2cDis, 0      , cube_H },
										{ +each_c2cDis, +each_c2cDis, cube_H  },
										{ +2*each_c2cDis, +each_c2cDis, cube_H  },
										{ +each_c2cDis, +2*each_c2cDis, cube_H  },
										{ +2 * each_c2cDis, +2*each_c2cDis, cube_H  },
										{ +each_c2cDis, +3*each_c2cDis, cube_H  },
										{ +2 * each_c2cDis, +3*each_c2cDis, cube_H  },
										{ +each_c2cDis, +4*each_c2cDis, cube_H  },
										{ +2 * each_c2cDis, +4*each_c2cDis, cube_H  },*/
										// E Type1 13 cubes [x is same]
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis }, // row 1
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)2.5 * each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis },
										//{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 4 * each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis }, // row 2
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis}, // row 3
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)2.5 * each_c2cDis},
										//{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + 3 * each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3 * each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis }, // row 4
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis}, // row 5
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)2.5 * each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis},
										//{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 4 * each_c2cDis},
										/*// E Type2 16 cubes [x is same]
										{ +(float)6.5 * each_c2cDis, -each_c2cDis *each_c2cDis, cube_H_E  }, // row 1
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 2*each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 3*each_c2cDis },
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 4*each_c2cDis }, 
										{ +(float)6.5 * each_c2cDis, -each_c2cDis* each_c2cDis + each_c2cDis, cube_H_E }, // row 2
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E }, // row 3
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + 2*each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + 3*each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3*each_c2cDis, cube_H_E }, // row 4
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E }, // row 5
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 2*each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 3*each_c2cDis},
										{ +(float)6.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 4*each_c2cDis},*/
										// T Type1 9 cubes [z (+)2.6 is same]
										{ -(float)0.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H_2  }, // row 1
										{ +(float)0.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H_2  },
										{ +(float)1.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H_2  },
										{ +(float)2.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H_2  },
										{ +(float)3.5 * each_c2cDis, -(float)0.5 * each_c2cDis, cube_H_2  },
										{ +(float)1.5 * each_c2cDis, +(float)0.5 * each_c2cDis, cube_H_2  }, // row 2
										{ +(float)1.5 * each_c2cDis, +(float)1.5 * each_c2cDis, cube_H_2  }, // row 3
										{ +(float)1.5 * each_c2cDis, +(float)2.5 * each_c2cDis, cube_H_2  }, // row 4
										{ +(float)1.5 * each_c2cDis, +(float)3.5 * each_c2cDis, cube_H_2  }, // row 5
										/*// T Type2 20 cubes [z (+)2.6 is same]
										{ -each_c2cDis, -each_c2cDis, cube_H_2 },
										{   0    , -each_c2cDis, cube_H_2 },
										{ +each_c2cDis, -each_c2cDis, cube_H_2 },
										{ +2 * each_c2cDis, -each_c2cDis, cube_H_2 },
										{ +3 * each_c2cDis, -each_c2cDis, cube_H_2 },
										{ +4 * each_c2cDis, -each_c2cDis, cube_H_2 },//
										{ -each_c2cDis, 0	  , cube_H_2},
										{ 	0	 , 0      , cube_H_2 },
										{ +each_c2cDis, 0      , cube_H_2 },
										{ +2 * each_c2cDis, 0      , cube_H_2 },
										{ +3 * each_c2cDis, 0      , cube_H_2 },
										{ +4 * each_c2cDis, 0      , cube_H_2},
										{ +each_c2cDis, +each_c2cDis, cube_H_2 },
										{ +2 * each_c2cDis, +each_c2cDis, cube_H_2 },
										{ +each_c2cDis, +2 * each_c2cDis, cube_H_2 },
										{ +2 * each_c2cDis, +2 * each_c2cDis, cube_H_2 },
										{ +each_c2cDis, +3 * each_c2cDis, cube_H_2 },
										{ +2 * each_c2cDis, +3 * each_c2cDis, cube_H_2 },
										{ +each_c2cDis, +4 * each_c2cDis, cube_H_2 },
										{ +2 * each_c2cDis, +4 * each_c2cDis, cube_H_2 },*/
										// R Type1 14
										/*{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 4 * each_c2cDis }, // row 1
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 3 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 2 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + 1 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + each_c2cDis, cube_H_E + 4 * each_c2cDis }, // row 2
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + each_c2cDis, cube_H_E + 0 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + 4 * each_c2cDis }, // row 3
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + 3 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2*each_c2cDis, cube_H_E + 2 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + 1 * each_c2cDis }, 
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3 * each_c2cDis, cube_H_E + 4 * each_c2cDis }, // row 4
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3 * each_c2cDis, cube_H_E + 2 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 4 * each_c2cDis }, // row 5
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + 1 * each_c2cDis }, */
										// R Type2 12 [x is same]
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis }, // row 1
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)2.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis }, // row 2
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis }, // row 3
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)2.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 2 * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3 * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis }, // row 4
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 3 * each_c2cDis, cube_H_E + (float)1.5 * each_c2cDis },
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)3.5 * each_c2cDis }, // row 5
										{ -(float)3.5 * each_c2cDis, -each_c2cDis * each_c2cDis + 4 * each_c2cDis, cube_H_E + (float)0.5 * each_c2cDis },
										// I 15 [y (-2.6) is same]
										//{ -(float)0.5*each_c2cDis, -cube_H_2, cube_H + 8 * each_c2cDis }, // row 1
										{ (float)0.5 * each_c2cDis , -cube_H_2, cube_H + 8 * each_c2cDis },
										{ +(float)1.5*each_c2cDis, -cube_H_2, cube_H + 8 * each_c2cDis },
										{ +(float)2.5*each_c2cDis, -cube_H_2, cube_H + 8 * each_c2cDis },
										//{ +(float)3.5*each_c2cDis, -cube_H_2, cube_H + 8 * each_c2cDis },
										{ +(float)1.5 * each_c2cDis, -cube_H_2, cube_H + 7 * each_c2cDis }, // row 2
										{ +(float)1.5 * each_c2cDis, -cube_H_2, cube_H + 6 * each_c2cDis }, // row 3
										{ +(float)1.5 * each_c2cDis, -cube_H_2, cube_H + 5 * each_c2cDis }, // row 4
										{ +(float)1.5 * each_c2cDis, -cube_H_2, cube_H + 4 * each_c2cDis }, // row 5
										//{ -(float)0.5 * each_c2cDis, -cube_H_2, cube_H + 3 * each_c2cDis }, // row 6
										{ (float)0.5 * each_c2cDis , -cube_H_2, cube_H + 3 * each_c2cDis },
										{ +(float)1.5 * each_c2cDis, -cube_H_2, cube_H + 3 * each_c2cDis },
										{ +(float)2.5 * each_c2cDis, -cube_H_2, cube_H + 3 * each_c2cDis },
										//{ +(float)3.5 * each_c2cDis, -cube_H_2, cube_H + 3 * each_c2cDis },
										// S 13 [y (+6.5) is same]
										//{ -(float)0.5 * each_c2cDis, cube_S_H, cube_H + 3 * each_c2cDis }, // row 1
										{ +(float)0.5 * each_c2cDis, cube_S_H, cube_H + 3 * each_c2cDis },
										{ +(float)1.5 * each_c2cDis, cube_S_H, cube_H + 3 * each_c2cDis },
										{ +(float)2.5 * each_c2cDis, cube_S_H, cube_H + 3 * each_c2cDis },
										{ +(float)3.5 * each_c2cDis, cube_S_H, cube_H + 3 * each_c2cDis },
										{ -(float)0.5 * each_c2cDis, cube_S_H, cube_H + 4 * each_c2cDis }, // row 2
										{ +(float)0.5 * each_c2cDis, cube_S_H, cube_H + 5 * each_c2cDis }, // row 3
										{ +(float)1.5 * each_c2cDis, cube_S_H, cube_H + 5 * each_c2cDis }, 
										{ +(float)2.5 * each_c2cDis, cube_S_H, cube_H + 5 * each_c2cDis }, 
										{ +(float)3.5 * each_c2cDis, cube_S_H, cube_H + 6 * each_c2cDis }, // row 4
										{ -(float)0.5 * each_c2cDis, cube_S_H, cube_H + 7 * each_c2cDis }, // row 5
										{ +(float)0.5 * each_c2cDis, cube_S_H, cube_H + 7 * each_c2cDis }, 
										{ +(float)1.5 * each_c2cDis, cube_S_H, cube_H + 7 * each_c2cDis }, 
										{ +(float)2.5 * each_c2cDis, cube_S_H, cube_H + 7 * each_c2cDis }, 
										//
										{ (float)1, (float)1, (float)-3.75},   // T1
										{ (float)3.75, (float)1, (float)-1 },  // E
										{ (float)1, (float)1, (float)+1.75 },  // T2								
										{ (float)-1.75, (float)1, (float)-1 }, // R
										{ (float)1, (float)-1.75, (float)-1 }, // I
										{ (float)1, (float)3.75, (float)-1 },  // S
	};

	void Collect_Ids(const vector<int>& drop_ids);
	void SetCenter(vector<float> T_center);
	void SetAmbColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetCubeId(vector<int> cube_ids);
	void SetAmbCoef(float coef, vector<PrimCube>& Cbs);
	void SetLambColor(vector<float> color, vector<PrimCube>& Cbs);
	void SetLambCoef(float coef, vector<PrimCube>& Cbs);
	void AlterSize(float sx, float sy, float sz, vector<PrimCube>& Cbs);
	void MoveTetris(vector<float> move_vec, vector<PrimCube>& Cbs);
	void RotateTetris(vector<float> euler_ang, vector<PrimCube>& Cbs);
	void SetLocation(vector<float> T_center, vector<PrimCube>& Cbs);
	void SetOrientation(vector<float> euler_ang, vector<PrimCube>& Cbs);
	void Reset(vector<int> cube_ids, vector<float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	Tetris_Menu(vector<int> cube_ids, vector <float> T_center, vector<float> ori, vector<float> size, vector<float> lamb_color, float lamb_coef, vector<float> amb_color, float amb_coef, vector<PrimCube>& Cbs);
	~Tetris_Menu() {};
};

#endif // __TETRIS_MENU_H__
