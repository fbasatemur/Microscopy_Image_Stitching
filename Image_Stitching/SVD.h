#pragma once
#include <string.h>              // required for memcpy()
#include <float.h>               // required for DBL_EPSILON
#include <math.h>                // required for fabs(), sqrt();


int Singular_Value_Decomposition(double* A, int nrows, int ncols, double* U, double* singular_values, double* V, double* dummy_array);
void Singular_Value_Decomposition_Solve(double* U, double* D, double* V, double tolerance, int nrows, int ncols, double *B, double* x);
static void Householders_Reduction_to_Bidiagonal_Form(double* A, int nrows, int ncols, double* U, double* V, double* diagonal, double* superdiagonal);
static int Givens_Reduction_to_Diagonal_Form(int nrows, int ncols, double* U, double* V, double* diagonal, double* superdiagonal);
static void Sort_by_Decreasing_Singular_Values(int nrows, int ncols, double* singular_values, double* U, double* V);