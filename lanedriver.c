//---------------------------------------------------------
// file:	lanedriver.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	lanedriver game
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "lanedriver.h"
#include "utility.h"
#include <math.h>

int numberOfLanes;
float laneXPos[4];
float roadWidth, laneWidth;
float lineWidth, lineHeight, lineSpacing, lineY;
float drivingSpeed;
int lineFrequency;

void laneDriverInit(Game g) {
	lineWidth = 6; //Width of Lane Lines
	lineHeight = 50; //Height of Lane Lines
	lineFrequency = 7;
	lineSpacing = (g.h - lineFrequency * lineHeight) / lineFrequency;

	numberOfLanes = 4;
	drivingSpeed = 5;

	roadWidth = g.w - PADDING;
	laneWidth = (roadWidth - 3 * lineWidth) / numberOfLanes;
	for (int i = 0; i < numberOfLanes; i++) {
		laneXPos[i] = g.x + PADDING + (laneWidth + lineWidth) * i + laneWidth / 4;
	}

}

void laneDriverPlay(Game g) {
	
	//Draw Road
	stroke(getColor(BLACK), 0);
	fill(CP_Color_Create(30, 30, 30, 255));
	CP_Graphics_DrawRect(g.x + PADDING / 2, g.y + PADDING/2, roadWidth, g.h - PADDING);

	//Draw White Passing Line Segments
	fill(getColor(WHITE));
	for (int i = 0; i < numberOfLanes - 1; i++) {
		for (int k = 0; k < lineFrequency; k++) {
			CP_Graphics_DrawRect(laneXPos[i] + laneWidth / 2, fmod(k * (lineHeight + lineSpacing) + lineY, g.h + PADDING/2) - PADDING/2, lineWidth, lineHeight);
		}
	}

	lineY += drivingSpeed;

	//Redraw top and bottom border to cut off the scrolling elements
	stroke(g.col, 0);
	fill(g.col);
	CP_Graphics_DrawRect(g.x, g.y - PADDING / 2, g.w, PADDING);
	CP_Graphics_DrawRect(g.x, g.y + g.h - PADDING / 2, g.w, PADDING);
}
