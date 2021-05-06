#pragma once
typedef unsigned char BYTE;

double* PhaseCorrelation(double* fft1Real, double* fft1Imag, double* fft2Real, double* fft2Imag, int width, int height);
float Correlation(BYTE* img1, BYTE* img2, int width, int height, int zoneWidth, int zoneHeight, int start1H, int start1W, int start2H, int start2W, BYTE v);
float Correlation(BYTE* img1, BYTE* img2, int width, int height, int zoneWidth, int zoneHeight, int start1H, int start1W, int start2H, int start2W);