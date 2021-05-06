#pragma once
#include <Windows.h>
struct imagener {
	double* Real;
	double* Im;
	int w;
	int h;
};
void FFT2D(BYTE* img, double* Output_real, double* Output_img, int width, int height);
void IFFT2D(double* Output_real, double* Output_imag, double* Input_real, double* Input_imag, int width, int height);