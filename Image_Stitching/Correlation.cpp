#include "Correlation.h"
#include "Process.h"
#include <math.h>
#include <vector>


double* Conjugate(double* imaginary, int width, int height) {

	long imageSize = width * height;
	double* conjugated = new double[imageSize];

#pragma omp parallel num_threads(NUM_THREADS) shared(imageSize, conjugated, imaginary)
	{
#pragma omp for schedule(dynamic) nowait
		for (long i = 0; i < imageSize; i++)
			conjugated[i] = -imaginary[i];
	}

	return conjugated;
}

void ComplexMult(double* fft1Real, double* fft1Imag, double* fft2Real, double* fft2Imag, double* outReal, double* outImag, int width, int height) {

	long imageSize = width * height;

#pragma omp parallel num_threads(NUM_THREADS) shared(imageSize, fft1Real, fft2Real, fft1Imag, fft2Imag, outReal, outImag)
	{
#pragma omp for schedule(dynamic) nowait
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
#pragma omp for schedule(dynamic) nowait
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
#pragma omp for schedule(dynamic) reduction(+:total1,total2) nowait
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
#pragma omp for schedule(dynamic) reduction(+:total,totalSqr1,totalSqr2) nowait
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

/*
* cornerID
*
	0 ********* 1
	* 			*
	* 			*
	* 			*
	2 ********* 3
*/
int ZoneDetection(BYTE* img1, BYTE* img2, int width, int height, xy* vec, xy* pocDot) {

	float* zoneCorr = new float[4]();

	/*
	* Zone

		*************
		*		|	*
		*	3	| 2 *
		*_______x___*
		*	1	| 0 *
		*************

		0: pocDot noktasinin sag alt bolgesi
		1: pocDot noktasinin sol alt bolgesi
		2: pocDot noktasinin sag ust bolgesi
		3: pocDot noktasinin sol ust bolgesi

	*/

#pragma omp parallel sections num_threads(NUM_THREADS)
	{
#pragma omp section
		{
			// bolge 0 eslesme
			int zoneWidth = width - pocDot->x;
			int zoneHeight = height - pocDot->y;
			if (zoneWidth != 0 && zoneHeight != 0) {
				int start1Row = pocDot->y;
				int start1Col = pocDot->x;
				int start2Row = 0;
				int start2Col = 0;
				zoneCorr[0] = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
			}
		}

#pragma omp section
		{
			// bolge 1 eslesme
			int zoneWidth = pocDot->x;
			int zoneHeight = height - pocDot->y;
			if (zoneWidth != 0 && zoneHeight != 0) {
				int start1Row = pocDot->y;
				int start1Col = 0;
				int start2Row = 0;
				int start2Col = width - pocDot->x;
				zoneCorr[1] = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
			}
		}
#pragma omp section
		{
			// bolge 2 eslesme
			int zoneWidth = width - pocDot->x;
			int zoneHeight = pocDot->y;
			if (zoneWidth != 0 && zoneHeight != 0) {
				int start1Row = 0;
				int start1Col = pocDot->x;
				int start2Row = height - pocDot->y;
				int start2Col = 0;
				zoneCorr[2] = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
			}
		}
#pragma omp section
		{
			// bolge 3 eslesme
			int zoneWidth = pocDot->x;
			int zoneHeight = pocDot->y;
			if (zoneWidth != 0 && zoneHeight != 0) {
				int start1Row = 0;
				int start1Col = 0;
				int start2Row = height - pocDot->y;
				int start2Col = width - pocDot->x;
				zoneCorr[3] = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
			}
		}
	}

	float max = -100000000.0;
	int zoneID;
	for (int i = 0; i < 4; i++) {
		if (zoneCorr[i] > max) {
			max = zoneCorr[i];
			zoneID = i;
		}
	}

	switch (zoneID)
	{
	case 0:
		vec->x = pocDot->x;
		vec->y = pocDot->y;
		return 0;
		break;
	case 1:
		vec->x = width - pocDot->x;
		vec->y = pocDot->y;
		return 1;
		break;
	case 2:
		vec->x = pocDot->x;
		vec->y = height - pocDot->y;
		return 2;
		break;
	case 3:
		vec->x = width - pocDot->x;
		vec->y = height - pocDot->y;
		return 3;
		break;
	default:
		break;
	}
}

/*float Correlation(BYTE* img1, BYTE* img2, int width, int height, int zoneWidth, int zoneHeight, int start1H, int start1W, int start2H, int start2W, BYTE v) {

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
}*/