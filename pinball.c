//---------------------------------------------------------
// file:	pinball.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	functionality for the pinball portion of the ADHD game
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "pinball.h"
#include "utility.h"

CP_Vector bouncerCenter[3];
float bouncerRadius, bouncerPower;
Triangle bouncerTriangle;

Paddle paddle[2];
float paddleMaxTheta, paddleMinTheta;
float paddleRadius, paddlePowerRest, paddlePowerHit;
float paddleAccSpeed, paddleDecSpeed;

CP_Vector pinballPos, pinballVel;
float pinballRadius, pinballJerkPower, pinballJerkTimeIncrement, pinballTimeSinceLastJerk;
CP_Color pinballCol, pinballStroke;
int pinballAlpha;
float pinballTimeOfDeath, pinballRespawnHoldTime;
float gravity, terminalVelocity;

void pinballInit(Game g) {
	/**********\
	| BOUNCERS |
	\**********/
	bouncerCenter[0].x = g.x + g.w * 5 / 16;
	bouncerCenter[0].y = g.y + g.h * 6 / 32;
	bouncerCenter[1].x = g.x + g.w * 11 / 16;
	bouncerCenter[1].y = g.y + g.h * 6 / 32;
	bouncerCenter[2].x = g.x + g.w * 2 / 4;
	bouncerCenter[2].y = g.y + g.h * 11 / 32;

	bouncerTriangle = newTriangleFromFloats(
		g.x + g.w * 8 / 16,
		g.y + g.h * 18 / 32,
		g.x + g.w * 7 / 16,
		g.y + g.h * 20 / 32,
		g.x + g.w * 9 / 16,
		g.y + g.h * 20 / 32
	);

	bouncerRadius = 20;
	bouncerPower = -7;

	/*********\
	| PADDLES |
	\*********/

	paddleRadius = 20;
	paddlePowerRest = -6;
	paddlePowerHit = -19;
	paddleMaxTheta = 120;
	paddleMinTheta = 60;
	paddleAccSpeed = 16;
	paddleDecSpeed = 5;

	paddle[LEFT].theta = paddleMaxTheta;
	paddle[LEFT].p1.x = g.x + PADDING / 2;
	paddle[LEFT].p1.y = g.y + g.h * 12 / 16;
	paddle[LEFT].triggered = 0;
	paddle[LEFT].power = paddlePowerRest;

	paddle[RIGHT].theta = paddleMaxTheta;
	paddle[RIGHT].p1.x = g.x + g.w - PADDING / 2;
	paddle[RIGHT].p1.y = g.y + g.h * 12 / 16;
	paddle[RIGHT].triggered = 0;
	paddle[RIGHT].power = paddlePowerRest;


	/*********\
	| PINBALL |
	\*********/
	pinballRadius = 13;
	pinballPos = CP_Vector_Zero();
	pinballVel = CP_Vector_Zero();
	pinballCol = getColor(GRAY);
	pinballStroke = getColor(BLACK);
	pinballAlpha = 255;
	pinballJerkPower = -5;
	pinballJerkTimeIncrement = 15;
	pinballTimeSinceLastJerk = 0;
	pinballTimeOfDeath = 0;
	pinballRespawnHoldTime = 1;

	pinballPos.x = CP_Random_RangeFloat(bouncerCenter[0].x - pinballRadius, bouncerCenter[1].x + pinballRadius);
	pinballPos.y = g.y + g.h * 1 / 10;

	pinballTimeSinceLastJerk = CP_System_GetSeconds();

	/*********\
	| PHYSICS |
	\*********/
	gravity = 0.6;
	terminalVelocity = 13;
}

void pinballPlay(Game g) {}
