//---------------------------------------------------------
// file:	main.h
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	header file to supplement main?
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef UTILITY_H
#define UTILITY_H

#define LEFT 0
#define RIGHT 1

#include "cprocessing.h"

float WIDTH, HEIGHT, MARGIN, PADDING;

typedef enum COLORS {
	BLACK,
	GRAY,
	LIGHT_GRAY,
	WHITE,
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	NONE
} COLORS;

typedef struct Triangle {
	CP_Vector p1;
	CP_Vector p2;
	CP_Vector p3;
	CP_Vector center;
	float area;
} Triangle;

CP_Color	getColorWithAlpha			(COLORS c, int a);
CP_Color	getColor					(COLORS c);
void		fill						(CP_Color c);
void		stroke						(CP_Color c, float w);
Triangle	newTriangleFromFloats		(float x1, float y1, float x2, float y2, float x3, float y3);
Triangle	newTriangleFromCPVectors	(CP_Vector p1, CP_Vector p2, CP_Vector p3);
float		areaOfThreePoints			(CP_Vector p1, CP_Vector p2, CP_Vector p3);
int			isInsideTriangle			(Triangle t, CP_Vector pq);

#endif