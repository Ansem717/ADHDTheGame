//---------------------------------------------------------
// file:	utility.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Utility functions
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "utility.h"
#include <math.h>

CP_Color getColorWithAlpha(COLORS c, int a) {
	switch (c) {
	case BLACK:
		return CP_Color_Create(0, 0, 0, a);
	case GRAY:
		return CP_Color_Create(140, 140, 140, a);
	case LIGHT_GRAY:
		return CP_Color_Create(180, 180, 180, a);
	case WHITE:
		return CP_Color_Create(255, 255, 255, a);
	case RED:
		return CP_Color_Create(140, 0, 0, a);
	case YELLOW:
		return CP_Color_Create(140, 140, 0, a);
	case GREEN:
		return CP_Color_Create(0, 140, 0, a);
	case CYAN:
		return CP_Color_Create(0, 140, 140, a);
	case BLUE:
		return CP_Color_Create(0, 0, 140, a);
	case MAGENTA:
		return CP_Color_Create(140, 0, 140, a);
	case NONE:
		return CP_Color_Create(255, 255, 255, 0);
	default:
		return CP_Color_Create(0, 0, 0, a);
	}
}

CP_Color getColor(COLORS c) {
	return getColorWithAlpha(c, 255);
}

void fill(CP_Color c) { CP_Settings_Fill(c); }

void stroke(CP_Color c, float w) {
	CP_Settings_Stroke(c);
	CP_Settings_StrokeWeight(w);
}

Triangle newTriangleFromFloats(float x1, float y1, float x2, float y2, float x3, float y3) {
	return newTriangleFromCPVectors(CP_Vector_Set(x1, y1), CP_Vector_Set(x2, y2), CP_Vector_Set(x3, y3));
}

Triangle newTriangleFromCPVectors(CP_Vector p1, CP_Vector p2, CP_Vector p3) {
	Triangle t = { 0 };
	t.p1 = p1;
	t.p2 = p2;
	t.p3 = p3;
	t.center = CP_Vector_Set(
		(t.p1.x + t.p2.x + t.p3.x) / 3,
		(t.p1.y + t.p2.y + t.p3.y) / 3
	);
	t.area = areaOfThreePoints(t.p1, t.p2, t.p3);
	return t;
}

float areaOfThreePoints(CP_Vector p1, CP_Vector p2, CP_Vector p3) { return fabs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0); }

int isInsideTriangle(Triangle t, CP_Vector pq) {
	//Calculate the areas of the three triangles by replacing one vertex with the point in question
	float A1 = areaOfThreePoints(pq, t.p2, t.p3);
	float A2 = areaOfThreePoints(t.p1, pq, t.p3);
	float A3 = areaOfThreePoints(t.p1, t.p2, pq);

	//if the sum of the areas are within area of the original triangle,
	// we have collision. t.area + 5% for a little margin of acceptance?
	return (t.area*1.05 >= A1 + A2 + A3);
}