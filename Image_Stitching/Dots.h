#pragma once
#include <math.h>
#include <cstdlib> 
#include <ctime> 
#include "Definitions.h"
#define MIN(a,b) (a < b) ? a : b


/*
* cornerID
*
	0 ********* 1
	* 		    *
	* 		    *
	* 		    *
	2 ********* 3
*/
xy* Rand4Dots(size_t cornerID, xy* pocDot, int imgWidth, int imgHeight)
{
	xy* rDots = new xy[4];
	int centerX, centerY, areaSizeX, areaSizeY, radius;

	switch (cornerID)
	{
	case 0:
		areaSizeX = (imgWidth - 1) - pocDot->x;
		areaSizeY = (imgHeight - 1) - pocDot->y;
		centerX = pocDot->x + (int)(areaSizeX / 2);
		centerY = pocDot->y + (int)(areaSizeY / 2);
		break;

	case 1:
		areaSizeX = pocDot->x;
		areaSizeY = (imgHeight - 1) - pocDot->y;
		centerX = (int)(areaSizeX / 2);
		centerY = pocDot->y + (int)(areaSizeY / 2);
		break;

	case 2:
		areaSizeX = (imgWidth - 1) - pocDot->x;
		areaSizeY = pocDot->y;
		centerX = pocDot->x + (int)(areaSizeX / 2);
		centerY = (int)(areaSizeY / 2);
		break;

	case 3:
		areaSizeX = pocDot->x;
		areaSizeY = pocDot->y;
		centerX = (int)(areaSizeX / 2);
		centerY = (int)(areaSizeY / 2);
		break;

	default:
		return NULL;
		break;
	}

	radius = MIN((int)(areaSizeX / 2), (int)(areaSizeY / 2));
	radius--;

	if (radius < 2)
		return NULL;
	else {
		rDots[0].x = centerX + radius;
		rDots[0].y = centerY;
		rDots[1].x = centerX;
		rDots[1].y = centerY - radius;
		rDots[2].x = centerX - radius;
		rDots[2].y = centerY + 1;
		rDots[3].x = centerX + 1;
		rDots[3].y = centerY + radius;
	}
	
	return rDots;
}


/*
* cornerID
*
	0 ********* 1
	* 		    *
	* 		    *
	* 		    *
	2 ********* 3
*/
xy* MatchingDots(size_t cornerID, xy* img1Dots, xy* vec) {

	xy* rDots = new xy[4];

	switch (cornerID)
	{
	case 0:
		rDots[0].x = img1Dots[0].x - vec->x;
		rDots[0].y = img1Dots[0].y - vec->y;
		rDots[1].x = img1Dots[1].x - vec->x;
		rDots[1].y = img1Dots[1].y - vec->y;
		rDots[2].x = img1Dots[2].x - vec->x;
		rDots[2].y = img1Dots[2].y - vec->y;
		rDots[3].x = img1Dots[3].x - vec->x;
		rDots[3].y = img1Dots[3].y - vec->y;
		break;

	case 1:
		rDots[0].x = img1Dots[0].x + vec->x;
		rDots[0].y = img1Dots[0].y - vec->y;
		rDots[1].x = img1Dots[1].x + vec->x;
		rDots[1].y = img1Dots[1].y - vec->y;
		rDots[2].x = img1Dots[2].x + vec->x;
		rDots[2].y = img1Dots[2].y - vec->y;
		rDots[3].x = img1Dots[3].x + vec->x;
		rDots[3].y = img1Dots[3].y - vec->y;
		break;

	case 2:
		rDots[0].x = img1Dots[0].x - vec->x;
		rDots[0].y = img1Dots[0].y + vec->y;
		rDots[1].x = img1Dots[1].x - vec->x;
		rDots[1].y = img1Dots[1].y + vec->y;
		rDots[2].x = img1Dots[2].x - vec->x;
		rDots[2].y = img1Dots[2].y + vec->y;
		rDots[3].x = img1Dots[3].x - vec->x;
		rDots[3].y = img1Dots[3].y + vec->y;
		break;

	case 3:
		rDots[0].x = img1Dots[0].x + vec->x;
		rDots[0].y = img1Dots[0].y + vec->y;
		rDots[1].x = img1Dots[1].x + vec->x;
		rDots[1].y = img1Dots[1].y + vec->y;
		rDots[2].x = img1Dots[2].x + vec->x;
		rDots[2].y = img1Dots[2].y + vec->y;
		rDots[3].x = img1Dots[3].x + vec->x;
		rDots[3].y = img1Dots[3].y + vec->y;
		break;

	default:
		break;
	}

	return rDots;
}


/*
* cornerID
*
	0 ********* 1
	* 		    *
	* 		    *
	* 		    *
	2 ********* 3
*/
xy* PanoDots(xy* prevVec, size_t currCornerID, xy* img1Dots) {

	xy* rDots = new xy[4];

	rDots[0].x = img1Dots[0].x + prevVec->x;
	rDots[0].y = img1Dots[0].y + prevVec->y;
	rDots[1].x = img1Dots[1].x + prevVec->x;
	rDots[1].y = img1Dots[1].y + prevVec->y;
	rDots[2].x = img1Dots[2].x + prevVec->x;
	rDots[2].y = img1Dots[2].y + prevVec->y;
	rDots[3].x = img1Dots[3].x + prevVec->x;
	rDots[3].y = img1Dots[3].y + prevVec->y;

	return rDots;
}


/*
* cornerID
*
	0 ********* 1
	* 		    *
	* 		    *
	* 		    *
	2 ********* 3
*/
void UpdatePrevVec(int currCornerID, xy* prevVec, xy* currVec) {

	switch (currCornerID)
	{
	case 0:
		prevVec->x += currVec->x;
		prevVec->y += currVec->y;
		break;

	case 1:
		prevVec->x -= currVec->x;
		prevVec->y += currVec->y;
		break;

	case 2:
		prevVec->x += currVec->x;
		prevVec->y -= currVec->y;
		break;

	case 3:
		prevVec->x -= currVec->x;
		prevVec->y -= currVec->y;
		break;

	default:
		break;
	}

	if (prevVec->x < 0) prevVec->x = 0;
	if (prevVec->y < 0) prevVec->y = 0;
}