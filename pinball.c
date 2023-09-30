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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
		g.y + g.h * 16 / 32,
		g.x + g.w * 13 / 32,
		g.y + g.h * 19 / 32,
		g.x + g.w * 19 / 32,
		g.y + g.h * 19 / 32
	);

	bouncerRadius = 20;
	bouncerPower = -7;

	/*********\
	| PADDLES |
	\*********/

	paddleRadius = 20;
	paddlePowerRest = -4;
	paddlePowerHit = -23;
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
	pinballRadius = 11;
	pinballPos = CP_Vector_Zero();
	pinballVel = CP_Vector_Zero();
	pinballCol = getColor(GRAY);
	pinballStroke = getColor(BLACK);
	pinballAlpha = 255;
	//pinballJerkPower = -5;
	//pinballJerkTimeIncrement = 15;
	//pinballTimeSinceLastJerk = 0;
	pinballTimeOfDeath = 0;
	pinballRespawnHoldTime = 1;

	pinballPos.x = CP_Random_RangeFloat(bouncerCenter[0].x - pinballRadius, bouncerCenter[1].x + pinballRadius);
	pinballPos.y = g.y + g.h * 1 / 10;

	//pinballTimeSinceLastJerk = CP_System_GetSeconds();

	/*********\
	| PHYSICS |
	\*********/
	gravity = 0.9f;
	terminalVelocity = 13;
}

void pinballPlay(Game g) {
	//BOUNCERS
	stroke(getColor(BLACK), 4);
	fill(g.col);
	for (int i = 0; i < 3; i++) {
		CP_Graphics_DrawCircle(bouncerCenter[i].x, bouncerCenter[i].y, bouncerRadius * 2);
	}
	CP_Graphics_DrawTriangle(
		bouncerTriangle.p1.x,
		bouncerTriangle.p1.y,
		bouncerTriangle.p2.x,
		bouncerTriangle.p2.y,
		bouncerTriangle.p3.x,
		bouncerTriangle.p3.y
	);

	//PADDLES
	paddle[LEFT].p2.x = g.x + PADDING / 2 + (g.w * 23 / 64) * sin(CP_Math_Radians(paddle[LEFT].theta));
	paddle[LEFT].p2.y = g.y + g.h * 12 / 16 + (g.h * 2 / 16) * -cos(CP_Math_Radians(paddle[LEFT].theta));

	paddle[RIGHT].p2.x = g.x + g.w - PADDING / 2 - (g.w * 23 / 64) * sin(CP_Math_Radians(paddle[RIGHT].theta));
	paddle[RIGHT].p2.y = g.y + g.h * 12 / 16 + (g.h * 2 / 16) * -cos(CP_Math_Radians(paddle[RIGHT].theta));

	for (int i = 0; i < 2; i++) {
		stroke(getColor(BLACK), paddleRadius * 2);
		CP_Graphics_DrawLine(paddle[i].p1.x, paddle[i].p1.y, paddle[i].p2.x, paddle[i].p2.y);

		if (paddle[i].triggered) {
			if (paddle[i].theta > paddleMinTheta) { paddle[i].theta -= paddleAccSpeed; } else { paddle[i].triggered = 0; }
		} else if (paddle[i].theta < paddleMaxTheta) paddle[i].theta += paddleDecSpeed;

		//Circles on paddle for style and to get MiddleX MiddleY
		fill(g.col);
		stroke(getColor(BLACK), 0);
		CP_Vector middle = CP_Vector_Set(
			(paddle[i].p2.x - paddle[i].p1.x) / 2 + paddle[i].p1.x,
			(paddle[i].p2.y - paddle[i].p1.y) / 2 + paddle[i].p1.y
		);
		CP_Graphics_DrawCircle(middle.x, middle.y, 16);
		CP_Graphics_DrawCircle(paddle[i].p1.x, paddle[i].p1.y, 16);
		CP_Graphics_DrawCircle(paddle[i].p2.x, paddle[i].p2.y, 16);

		//PADDLE COLLISION
		float deltaX = (i == LEFT) ? paddle[i].p2.x - paddle[i].p1.x : paddle[i].p1.x - paddle[i].p2.x;
		float deltaY = (i == LEFT) ? paddle[i].p2.y - paddle[i].p1.y : paddle[i].p1.y - paddle[i].p2.y;
		float slope = deltaY / deltaX;
		float yint = (paddle[i].p1.y - paddleRadius) - (slope * paddle[i].p1.x);
		float yint2 = (paddle[i].p1.y + paddleRadius) - (slope * paddle[i].p1.x);

		CP_Vector normal = CP_Vector_Normalize(CP_Vector_Set(-deltaY, deltaX));
		CP_Vector pinballPointClosestToPaddle = CP_Vector_Add(pinballPos, CP_Vector_Scale(normal, pinballRadius));

		int condition = (i == LEFT) ? pinballPointClosestToPaddle.x < paddle[i].p2.x : pinballPointClosestToPaddle.x > paddle[i].p2.x;
		paddle[i].power = (paddle[i].theta < paddleMaxTheta) ? paddlePowerHit : paddlePowerRest;

		if (condition) {
			//Pinball is aiming for the flat surface of the paddle
			if (pinballPointClosestToPaddle.y >= (slope * pinballPointClosestToPaddle.x) + yint &&
				pinballPointClosestToPaddle.y <= (slope * pinballPointClosestToPaddle.x) + yint2 + pinballRadius) {
				pinballVel = CP_Vector_Scale(normal, paddle[i].power);
				pinballPos = CP_Vector_Subtract(pinballPos, normal);
			}
		} else {
			//Pinball is aiming for the curved head
			float dist = CP_Vector_Distance(pinballPos, paddle[i].p2);
			float oppLen = paddle[i].p2.y - pinballPos.y;
			float adjLen = paddle[i].p2.x - pinballPos.x;
			float theta = atan2f(oppLen, adjLen);

			if (dist < pinballRadius*1.3 + paddleRadius) {
				pinballVel.y = paddle[i].power * sin(theta);
				pinballVel.x = paddle[i].power * cos(theta);
				pinballPos.y += paddle[i].power * sin(theta);
				pinballPos.x += paddle[i].power * cos(theta);
			}
		}

	}

	//PINBALL
	stroke(pinballStroke, 2);
	fill(pinballCol);
	CP_Graphics_DrawCircle(pinballPos.x, pinballPos.y, pinballRadius * 2);

	//BOUNCER COLLISION
	for (int i = 0; i < 3; i++) {
		float dist = CP_Vector_Distance(pinballPos, bouncerCenter[i]);
		float oppLen = bouncerCenter[i].y - pinballPos.y;
		float adjLen = bouncerCenter[i].x - pinballPos.x;
		float theta = atan2f(oppLen, adjLen);

		if (dist < pinballRadius + bouncerRadius) {
			pinballVel.y = bouncerPower * sin(theta);
			pinballVel.x = bouncerPower * cos(theta);
		}
	}

	//Bouncer TRIANGLE collision
	float yLen = bouncerTriangle.center.y - pinballPos.y;
	float xLen = bouncerTriangle.center.x - pinballPos.x;
	float triTheta = atan2f(yLen, xLen);

	CP_Vector closest = CP_Vector_Add(pinballPos, CP_Vector_Set(pinballRadius * cos(triTheta), pinballRadius * sin(triTheta)));

	//y = acos((a2 + b2 - c2) / (2ab))
	//I am measuring the angle between BASE and CENTER to get the TOP RIGHT angle.
	float a = CP_Vector_Distance(bouncerTriangle.p2, bouncerTriangle.p3); // length of base
	float b = CP_Vector_Distance(bouncerTriangle.p2, bouncerTriangle.center); // length towards center
	float c = CP_Vector_Distance(bouncerTriangle.p3, bouncerTriangle.center); // length towards center, opposite
	float deflectThetaTriangle = acos(  (pow(a,2.0) + pow(b,2.0) - pow(c,2.0)) / (2.0*a*b) ); //Should be the angle to deflect the ball if it hits the RIGHT triangle face

	if (isInsideTriangle(bouncerTriangle, closest)) {
		if (isInsideTriangle(newTriangleFromCPVectors(bouncerTriangle.center, bouncerTriangle.p2, bouncerTriangle.p3), closest)) {
			//The ball hit the BOTTOM of the triangle. 
			pinballVel.y = bouncerPower * -1;
			pinballVel.x *= -1;
		} else if (isInsideTriangle(newTriangleFromCPVectors(bouncerTriangle.center, bouncerTriangle.p1, bouncerTriangle.p2), closest)) {
			//The ball hit the TOP LEFT of the triangle
			pinballVel.y = bouncerPower * sin(deflectThetaTriangle);
			pinballVel.x = bouncerPower * cos(deflectThetaTriangle);
		} else {
			//The ball hit the triangle but didn't hit the bottom or the left? it hit the right.
			pinballVel.y = bouncerPower * sin(deflectThetaTriangle);
			pinballVel.x = bouncerPower * -cos(deflectThetaTriangle);
		}
		pinballPos = CP_Vector_Add(pinballPos, pinballVel);
	}


	//WALL COLLISION
	if (pinballPos.x < g.x + PADDING / 2 + pinballRadius) {
		pinballVel.x *= -1;
		pinballPos.x += (g.x + PADDING / 2 + pinballRadius) - pinballPos.x + 1;
	}
	if (pinballPos.x > g.x + g.w - PADDING / 2 - pinballRadius) {
		pinballVel.x *= -1;
		pinballPos.x -= pinballPos.x - (g.x + g.w - PADDING / 2 - pinballRadius) + 1;
	}
	if (pinballPos.y < g.y + PADDING / 2 + pinballRadius) {
		pinballVel.y *= -1;
		pinballVel.y += (g.y + PADDING / 2 + pinballRadius) - pinballPos.y + 1;
	}
	if (pinballPos.y > g.y + g.h - PADDING / 2 - pinballRadius) {
		pinballVel.y *= -1;
		pinballPos.y -= pinballPos.y - (g.y + g.h - PADDING / 2 - pinballRadius) + 1;
	}

	//LOSS CONDITION
	stroke(getColor(RED), 2);
	CP_Graphics_DrawLine(g.x, g.y + g.h * 53 / 64, g.x + g.w, g.y + g.h * 53 / 64);

	if (pinballPos.y > g.y + g.h * 53 / 64) {
		//Rounded estimate from lowest point of paddle
		//Pinball has gone too far

		pinballPos.y += pinballVel.y;
		pinballAlpha -= 30;
		pinballCol = getColorWithAlpha(GRAY, pinballAlpha);
		pinballStroke = getColorWithAlpha(BLACK, pinballAlpha);

		//TODO: Decrement Death. If deaths == 0, lose the game.

		if (pinballAlpha <= 0) {
			//Fade complete. Restart pinball location. 
			pinballPos.x = CP_Random_RangeFloat(bouncerCenter[0].x - pinballRadius, bouncerCenter[1].x + pinballRadius);
			pinballPos.y = g.y + g.h * 1 / 10;

			pinballCol = getColorWithAlpha(GRAY, 255);
			pinballStroke = getColorWithAlpha(BLACK, 255);

		}
	} else if (pinballAlpha <= 0) {
		//Should only happen if death is done fading out. 
		//I cannot wait inside of that conditional because I've already moved the pinball.
		//However, I can avoid referencing alpha by using GRAY and BLACK, letting Alpha
		// mimic as a conditional as well.

		//Hold the ball for a second so the player is ready
		pinballTimeOfDeath = (pinballTimeOfDeath == 0) ? CP_System_GetSeconds() : pinballTimeOfDeath;
		if (CP_System_GetSeconds() > pinballTimeOfDeath + pinballRespawnHoldTime) {
			pinballAlpha = 255;
			pinballTimeOfDeath = 0;
		}
	} else {
		//PINBALL MOTION
		pinballPos = CP_Vector_Add(pinballPos, pinballVel);
		if (pinballVel.y <= terminalVelocity) {
			pinballVel.y += gravity;
		} else {
			pinballVel.y = terminalVelocity;
		}

		/*PINBALL JERK
		//To prevent the pinball from locking in a perfect angle
		//The pinball randomly jerks up
		if (pinballTimeSinceLastJerk + pinballJerkTimeIncrement < CP_System_GetSeconds()) {
			//Time to jerk!
			pinballVel.x += pinballJerkPower;
			pinballVel.y += pinballJerkPower;
			pinballTimeSinceLastJerk = CP_System_GetSeconds();
		}*/
	}



	//PINBALL CONTROLS
	if (CP_Input_KeyTriggered(g.cont.keyLeft) && paddle[LEFT].theta >= paddleMaxTheta) paddle[LEFT].triggered = 1;
	if (CP_Input_KeyTriggered(g.cont.keyRight) && paddle[RIGHT].theta >= paddleMaxTheta) paddle[RIGHT].triggered = 1;

}
