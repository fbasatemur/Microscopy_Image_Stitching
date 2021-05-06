#include <Windows.h>
#include "xy.h"
typedef signed short int BYTE2;		// -32768 to 32767

xy* ReSizePanorama(double** H, int pano_Width, int pano_Height, int Width, int Height, xy& position);
void LaplacePyramid(double** H, BYTE* panoramaImg1, BYTE* img2, int pano_Width, int pano_Height, int Width, int Height, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, xy newPanoSize, xy pano_position);
//BYTE* L2(double** H, BYTE* panoramaImg1, BYTE* img2, int pano_Width, int pano_Height, int Width, int Height, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, xy newPanoSize, xy pano_position);
//void LaplacePyramid(double** H, BYTE * Raw_Intensity, BYTE * Raw_Intensity2, int Width, int Height, BYTE* LaplacePyramid1[], BYTE* LaplacePyramid2[], int%width, int% height, xy Size, xy position);
BYTE* PanoToResized(BYTE* Raw, int Width, int Height, xy Size, xy position);
BYTE* ImageToResized(double** H, BYTE* Raw, int Width, int Height, xy Size, xy position);
BYTE* BoyutAyarla(double** H, BYTE* Raw, int Width, int Height, xy Size, xy position, int img);
BYTE* BoyutGenislet(BYTE* Raw, int% width, int% height, int Width, int Height);
BYTE* GaussPyramid(BYTE* Raw, int% width, int% height);
BYTE2* GaussPyramidBYTE2(BYTE* Raw, int% width, int% height);
template<typename T>
BYTE2* Difference(BYTE* Gauss1, T* Gauss2, int width, int height);
template<typename T>
BYTE2* Expand(T* Gauss, int width, int height);
void GoruntuDuzelt(BYTE* Raw, xy Size, BYTE* tempRaw, int Width, int Height);
BYTE GetMeanPiksel(BYTE* Raw, int i, int j, xy Size, int rgb);
BYTE* PanaromicImage(double** H, int Width, int Height, xy Size, xy position, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, int orWidth, int orHeight, bool ilkSatir, int currCornerID, xy* prevVec, xy* currVec);
BYTE* P2(double** H, int Width, int Height, xy Size, xy position, BYTE2** LaplacePyramid1, BYTE2** LaplacePyramid2, int& width, int& height, int orWidth, int orHeight, bool ilkSatir, int currCornerID, xy* prevVec, xy* currVec);
