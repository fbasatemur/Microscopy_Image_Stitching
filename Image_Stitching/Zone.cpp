#include "Zone.h"
#include "Correlation.h"
#define NUM_THREADS 4

//bool IsLittleStep(BYTE* img1, BYTE* img2, int width, int height, int cornerID, xy* vec) {
//
//	float zone1Corr = 0.0F, zone2Corr = 0.0F;
//	int start1Row, start1Col, start2Row, start2Col;
//	int zoneWidth, zoneHeight;
//
//	switch (cornerID)
//	{
//	case 0:
//		// little step contol
//		zoneWidth = width - vec->x;
//		zoneHeight = height - vec->y;
//		start1Row = vec->y;
//		start1Col = vec->x;
//		start2Row = 0;
//		start2Col = 0;
//		zone1Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//
//		// big step control
//		zoneWidth = vec->x;
//		zoneHeight = vec->y;
//		start1Row = 0;
//		start1Col = 0;
//		start2Row = height - vec->y;
//		start2Col = width - vec->x;
//		zone2Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//		break;
//
//	case 1:
//		// little step contol
//		zoneWidth = width - vec->x;
//		zoneHeight = height - vec->y;
//		start1Row = vec->y;
//		start1Col = 0;
//		start2Row = 0;
//		start2Col = vec->x;
//		zone1Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//
//		// big step control
//		zoneWidth = vec->x;
//		zoneHeight = vec->y;
//		start1Row = 0;
//		start1Col = width - vec->x;
//		start2Row = height - vec->y;
//		start2Col = 0;
//		zone2Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//		break;
//
//	case 2:
//		// little step control
//		zoneWidth = width - vec->x;
//		zoneHeight = height - vec->y;
//		start1Row = 0;
//		start1Col = vec->x;
//		start2Row = vec->y;
//		start2Col = 0;
//		zone1Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//
//		// big step control
//		zoneWidth = vec->x;
//		zoneHeight = vec->y;
//		start1Row = height - vec->y;
//		start1Col = 0;
//		start2Row = 0;
//		start2Col = width - vec->x;
//		zone2Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//		break;
//
//	case 3:
//		// little step control
//		zoneWidth = width - vec->x;
//		zoneHeight = height - vec->y;
//		start1Row = 0;
//		start1Col = 0;
//		start2Row = vec->y;
//		start2Col = vec->x;
//		zone1Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//
//		// big step control
//		zoneWidth = vec->x;
//		zoneHeight = vec->y;
//		start1Row = height - vec->y;
//		start1Col = width - vec->x;
//		start2Row = 0;
//		start2Col = 0;
//		zone2Corr = Correlation(img1, img2, width, height, zoneWidth, zoneHeight, start1Row, start1Col, start2Row, start2Col);
//		break;
//
//	default:
//		break;
//	}
//
//	return zone1Corr >= zone2Corr;
//}
//
//int BigStepUpdate(int currCornerID, xy* currVec, int width, int height) {
//
//	currVec->x = width - currVec->x;
//	currVec->y = height - currVec->y;
//	
//	switch (currCornerID)
//	{
//	case 0:
//		return 3;
//
//	case 1:
//		return 2;
//	
//	case 2:
//		return 1;
//
//	case 3:
//		return 0;
//	}
//}


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