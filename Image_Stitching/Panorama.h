#include <Windows.h>
#include "Definitions.h"

xy* ReSizePanorama(double** H, int pano_Width, int pano_Height, int Width, int Height, xy& position);
void LaplacePyramid(double** H, BYTE* panoramaImg1, BYTE* img2, int pano_Width, int pano_Height, int Width, int Height, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, xy newPanoSize, xy pano_position);
BYTE* PanaromicImage(double** H, int Width, int Height, xy Size, xy position, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, int orWidth, int orHeight, bool ilkSatir, int currCornerID, xy* currVec);
//BYTE* P2(double** H, int Width, int Height, xy Size, xy position, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, int orWidth, int orHeight, bool ilkSatir, int currCornerID, xy* prevVec, xy* currVec);
