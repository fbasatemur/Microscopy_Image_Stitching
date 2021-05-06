#include "Correlation.h"
#include "Process.h"
#include <math.h>
#include <vector>
#define NUM_THREADS 4

double* Conjugate(double* imaginary, int width, int height) {

	long imageSize = width * height;
	double* conjugated = new double[imageSize];

#pragma omp parallel num_threads(NUM_THREADS) shared(imageSize, conjugated, imaginary)
	{
#pragma omp for schedule(static) nowait
		for (long i = 0; i < imageSize; i++)
			conjugated[i] = -imaginary[i];
	}

	return conjugated;
}

void ComplexMult(double* fft1Real, double* fft1Imag, double* fft2Real, double* fft2Imag, double* outReal, double* outImag, int width, int height) {

	long imageSize = width * height;

#pragma omp parallel num_threads(NUM_THREADS) shared(imageSize, fft1Real, fft2Real, fft1Imag, fft2Imag, outReal, outImag)
	{
#pragma omp for schedule(static) nowait
		for (long i = 0; i < imageSize; i++)
		{
			outReal[i] = fft1Real[i] * fft2Real[i] - fft1Imag[i] * fft2Imag[i];
			outImag[i] = fft1Real[i] * fft2Imag[i] + fft1Imag[i] * fft2Real[i];
		}
	}
}

double* PhaseCorrelation(double* fft1Real, double* fft1Imag, double* fft2Real, double* fft2Imag, int width, int height)
{
	double* multReal, * multImag;
	long imageSize = width * height;

	multReal = new double[imageSize];
	multImag = new double[imageSize];

	double* conjugated = Conjugate(fft2Imag, width, height);
	ComplexMult(fft1Real, fft1Imag, fft2Real, conjugated, multReal, multImag, width, height);

	double norm;
#pragma parallel omp num_threads(NUM_THREADS) shared(multReal, multImag, imageSize) private(norm)
	{
#pragma omp for schedule(static) nowait
		for (long i = 0; i < imageSize; i++)
		{
			norm = sqrt(pow(multReal[i], 2) + pow(multImag[i], 2));
			multReal[i] /= norm;
			multImag[i] /= norm;
		}
	}

	double* outReal = new double[imageSize];
	double* outImag = new double[imageSize];

	IFFT2D(outReal, outImag, multReal, multImag, width, height);


	delete[] multReal; delete[] multImag;
	delete[] outImag; delete[] conjugated;

	return outReal;
}

float Correlation(BYTE* img1, BYTE* img2, int width, int height, int zoneWidth, int zoneHeight, int start1H, int start1W, int start2H, int start2W) {

	int newR1, newC1, newR2, newC2;
	unsigned int total1 = 0, total2 = 0;
	double total = 0.0F, totalSqr1 = 0.0F, totalSqr2 = 0.0F;
	double mean1, mean2;

#pragma omp parallel num_threads(NUM_THREADS) shared(zoneHeight,zoneWidth,start1H,start2H,start1W,start2W, width, img1, img2) private(newR1, newR2, newC1, newC2)
	{
#pragma omp for schedule(static) reduction(+:total1,total2) nowait
		for (int r = 0; r < zoneHeight; r++) {

			newR1 = r + start1H;
			newR2 = r + start2H;
			for (int c = 0; c < zoneWidth; c++) {

				newC1 = c + start1W;
				newC2 = c + start2W;
				total1 += unsigned int(img1[newR1 * width + newC1]);
				total2 += unsigned int(img2[newR2 * width + newC2]);
			}
		}
	}

	mean1 = double(total1 / (zoneHeight * zoneWidth));
	mean2 = double(total2 / (zoneHeight * zoneWidth));

#pragma omp parallel num_threads(NUM_THREADS) shared(zoneHeight,zoneWidth,start1H,start2H,start1W,start2W,width,img1,img2,mean1,mean2) private(newR1, newR2, newC1, newC2)
	{
#pragma omp for schedule(static) reduction(+:total,totalSqr1,totalSqr2) nowait
		for (int r = 0; r < zoneHeight; r++) {

			newR1 = r + start1H;
			newR2 = r + start2H;
			for (int c = 0; c < zoneWidth; c++) {

				newC1 = c + start1W;
				newC2 = c + start2W;
				total += ((img1[newR1 * width + newC1] - mean1) * (img2[newR2 * width + newC2] - mean2));

				totalSqr1 += pow((img1[newR1 * width + newC1] - mean1), 2);
				totalSqr2 += pow((img2[newR2 * width + newC2] - mean2), 2);
			}
		}
	}

	return float(total / sqrt(totalSqr1 + totalSqr2));
}

float Correlation(BYTE* img1, BYTE* img2, int width, int height, int zoneWidth, int zoneHeight, int start1H, int start1W, int start2H, int start2W, BYTE v) {

	int newR1, newC1, newR2, newC2;
	unsigned int total1 = 0, total2 = 0;
	double total = 0.0F, totalSqr1 = 0.0F, totalSqr2 = 0.0F;
	double mean1, mean2;


	for (int r = 0; r < zoneHeight; r++) {

		newR1 = r + start1H;
		newR2 = r + start2H;
		for (int c = 0; c < zoneWidth; c++) {

			newC1 = c + start1W;
			newC2 = c + start2W;
			total1 += unsigned int(img1[newR1 * width + newC1]);
			total2 += unsigned int(img2[newR2 * width + newC2]);
		}
	}
	mean1 = double(total1 / (zoneHeight * zoneWidth));
	mean2 = double(total2 / (zoneHeight * zoneWidth));

	for (int r = 0; r < zoneHeight; r++) {

		newR1 = r + start1H;
		newR2 = r + start2H;
		for (int c = 0; c < zoneWidth; c++) {

			newC1 = c + start1W;
			newC2 = c + start2W;
			total += ((img1[newR1 * width + newC1] - mean1) * (img2[newR2 * width + newC2] - mean2));

			totalSqr1 += pow((img1[newR1 * width + newC1] - mean1), 2);
			totalSqr2 += pow((img2[newR2 * width + newC2] - mean2), 2);

			img1[newR1 * width + newC1] = v;
			img2[newR2 * width + newC2] = v;
		}
	}

	return float(total / sqrt(totalSqr1 + totalSqr2));
}