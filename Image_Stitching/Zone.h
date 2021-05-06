#pragma once
#include "xy.h"
typedef unsigned char BYTE;

//bool IsLittleStep(BYTE* img1, BYTE* img2, int width, int height, int cornerID, xy* vec);
//int BigStepUpdate(int currCornerID, xy* currVec, int width, int height);
int ZoneDetection(BYTE* img1, BYTE* img2, int width, int height, xy* vec, xy* pocDot);