//---------------------------------------------------------
// file:	lanedriver.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	lanedriver game - it's actually inverse frogger
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

CP_Vector carPos;
float carWidth, carHeight;
float carTheta, carTargetTheta, carTurnSpeed, carMoveSpeed;
int currentLane, carTurnState;

void laneDriverInit(Game g) {
	lineWidth = 6; //Width of Lane Lines
	lineHeight = 50; //Height of Lane Lines
	lineFrequency = 7;
	lineSpacing = (g.h - lineFrequency * lineHeight) / lineFrequency;

	numberOfLanes = 4;
	drivingSpeed = 5;

	roadWidth = g.w - PADDING;
	laneWidth = roadWidth / numberOfLanes;
	for (int i = 0; i < numberOfLanes; i++) {
		laneXPos[i] = g.x + laneWidth / 2 + PADDING / 2 + laneWidth * i;
	}

	currentLane = CP_Random_RangeInt(0, numberOfLanes - 1);
	carPos.x = laneXPos[currentLane];
	carPos.y = g.y + g.h * 25 / 32;
	carWidth = laneWidth * 3 / 5;
	carHeight = 80;
	carTheta = 0;
	carTargetTheta = 24;
	carTurnSpeed = 8;
	carMoveSpeed = 6;
	carTurnState = 0;
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
			CP_Graphics_DrawRect(
				laneXPos[i] + laneWidth/2 - lineWidth/2, 
				fmod(k * (lineHeight + lineSpacing) + lineY, g.h + PADDING/2) - PADDING/2,
				lineWidth, 
				lineHeight
			);
		}
	}
	lineY += drivingSpeed;

	//CAR
	if (carTurnState != 0) {
		//it's equal to -1 if it's left, and +1 if its right. 

		if (fabs(carPos.x - laneXPos[currentLane + carTurnState]) > carMoveSpeed * 2) {
			//First, currentLane + carTurnState will either be cur-1 or cur+1, meaning the lane directly left or right of me.
			//Sub that into the laneXPos array to identify the x value of the target lane.
			//Take my current position and subtract the target. This will return the distacne. 
			//FABS means it won't matter if I'm to the left (smaller value) or right (greater value)
			//If my distance is greater than a certain margin of error, keep going.
			carPos.x += carMoveSpeed * carTurnState;

			if (fabs(carTheta) < carTargetTheta) {
				//The car should start turning towards it's necessary angle.
				//Using absolute value to ensure I have a limit even if I'm negative.
				carTheta += carTurnSpeed * carTurnState;
			}
			
		} else {
			//We've arrived! Let's set values to default.
			carPos.x = laneXPos[currentLane + carTurnState];
			carTheta -= carTurnSpeed * carTurnState; //ease back to 0.
			if (fabs(carTheta) < carTurnSpeed * 2) {
				//within the acceptable margin, now we can set stuff to 0.
				currentLane += carTurnState;
				carTheta = 0;
				carTurnState = 0;
			}
		}
		
	}

	fill(g.col);
	stroke(getColor(BLACK), 2);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRectAdvanced(carPos.x, carPos.y, carWidth, carHeight, carTheta, 10);
	CP_Settings_RectMode(CP_POSITION_CORNER);

	//Redraw top and bottom border to cut off the scrolling elements
	stroke(g.col, 0);
	fill(g.col);
	CP_Graphics_DrawRect(g.x, g.y - PADDING / 2, g.w, PADDING);
	CP_Graphics_DrawRect(g.x, g.y + g.h - PADDING / 2, g.w, PADDING);

	//Controller
	if (CP_Input_KeyTriggered(g.cont.keyLeft) && carTheta == 0 && currentLane > 0) carTurnState = -1;
	if (CP_Input_KeyTriggered(g.cont.keyRight) && carTheta == 0 && currentLane < numberOfLanes - 1) carTurnState = 1;
}