#pragma once
#include <iostream>
#include "SVD.h"

double** homography2d(double** points1, double** points2, int s) {
	
	// Compute H using normalized DLT 

	double** H = new double* [3];
	for (int i = 0; i < 3; i++)
		H[i] = new double[3];

	double* A, * U, * singularvalue, * V, * dummy_array, * B, * X;
	A = (double*)malloc(sizeof(double) * s * 2 * 8);
	U = (double*)malloc(sizeof(double) * s * 2 * 8);
	V = (double*)malloc(sizeof(double) * 8 * 8);
	singularvalue = (double*)malloc(sizeof(double) * 8);
	dummy_array = (double*)malloc(sizeof(double) * 8);
	X = (double*)malloc(sizeof(double) * 8);
	B = (double*)malloc(sizeof(double) * s * 2);
	double tolerance = 0;

	double a1, b1, a2, b2;
	for (int i = 0; i < s; i++) {
		a1 = (double)points2[0][i];
		b1 = (double)points2[1][i];
		a2 = (double)points1[0][i];
		b2 = (double)points1[1][i];
		B[i * 2] = a2;
		B[i * 2 + 1] = b2;
		double point[16] = { a1, b1, 1, 0, 0, 0, -a1 * a2, -b1 * a2,
			0, 0, 0, a1, b1, 1, -a1 * b2, -b1 * b2 };

		for (int j = 0; j < 16; j++)
			A[i * 16 + j] = point[j];
			//A[i * 16 + (j+8)] = point[j + 8];

	}

	int err = Singular_Value_Decomposition((double*)A, s * 2, 8, (double*)U, singularvalue, (double*)V, dummy_array);
	free(dummy_array);
	if (err < 0) {
		System::Windows::Forms::MessageBox::Show("Homography Basarisiz\n");
		return 0;
	}
	else {

		Singular_Value_Decomposition_Solve((double*)U, singularvalue, (double*)V, tolerance, s * 2, 8, B, X);

		free(A);
		free(U);
		free(V);
		free(singularvalue);
		free(B);
	}
	
	for (int i = 0; i < 2; i++)
	{
		H[i][0] = X[i * 3];
		H[i][1] = X[i * 3 + 1];
		H[i][2] = X[i * 3 + 2];
	}
	H[2][0] = X[6];
	H[2][1] = X[7];
	H[2][2] = 1;
	free(X);


	return H;
}//homography2d