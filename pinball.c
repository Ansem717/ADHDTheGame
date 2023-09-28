//---------------------------------------------------------
// file:	pinball.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	functionality for the pinball portion of the ADHD game
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "utility.h"
#include "pinball.h"

/*****************\
| UTILITY HELPERS |
\*****************/
float area(CP_Vector p1, CP_Vector p2, CP_Vector p3) {
	return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0);
}

int isInside(CP_Vector p1, CP_Vector p2, CP_Vector p3, CP_Vector pq) {
	//Calculate area of triangle
	float A = area(p1, p2, p3);

	//Calculate the areas of the three triangles by replacing one vertex with the point in question
	float A1 = area(pq, p2, p3);
	float A2 = area(p1, pq, p3);
	float A3 = area(p1, p2, pq);

	//if the sum of the triangles formed by the point and edge equal the area of the original triangle,
	// we have collision.
	return (A + 1 >= A1 + A2 + A3);
}

/**************\
| PINBALL GAME |
\**************/

void pinballInitVars() {
	bouncerRadius = 20;
	bouncerPower = -7;

	paddleRadius = 20;
	paddlePowerRest = -6;
	paddlePowerHit = -19;
	paddleMaxTheta = 120;
	paddleMinTheta = 60;
	paddleAccSpeed = 16;
	paddleDecSpeed = 5;

	gravity = 0.6;
	terminalVelocity = 13;

	pinballRadius = 13;
	pinballPos = CP_Vector_Zero();
	pinballVel = CP_Vector_Zero();
	pinballCol = GRAY;
	pinballStroke = BLACK;
	pinballAlpha = 255;
	pinballJerkPower = -5;
	pinballJerkTimeIncrement = 15;
	pinballTimeSinceLastJerk = 0;
	pinballTimeOfDeath = 0;
	pinballRespawnHoldTime = 1;
}

void pinball(Game* g) {
	if (g->init == 0) {
		//BOUNCERS
		bouncerCenter[0].x = g->x + g->w * 5 / 16;
		bouncerCenter[0].y = g->y + g->h * 6 / 32;
		bouncerCenter[1].x = g->x + g->w * 11 / 16;
		bouncerCenter[1].y = g->y + g->h * 6 / 32;
		bouncerCenter[2].x = g->x + g->w * 2 / 4;
		bouncerCenter[2].y = g->y + g->h * 11 / 32;

		bouncerTriangle.p1.x = g->x + g->w * 8 / 16;
		bouncerTriangle.p1.y = g->y + g->h * 18 / 32;
		bouncerTriangle.p2.x = g->x + g->w * 7 / 16;
		bouncerTriangle.p2.y = g->y + g->h * 20 / 32;
		bouncerTriangle.p3.x = g->x + g->w * 9 / 16;
		bouncerTriangle.p3.y = g->y + g->h * 20 / 32;
		bouncerTriangle.center.x = g->x + g->w * 8 / 16;
		bouncerTriangle.center.y = g->y + g->h * 77 / 128;

		bouncerTriangle.area = area(bouncerTriangle.p1, bouncerTriangle.p2, bouncerTriangle.p3);


		//PADDLES
		paddle[LEFT].theta = paddleMaxTheta;
		paddle[LEFT].p1.x = g->x + padding / 2;
		paddle[LEFT].p1.y = g->y + g->h * 12 / 16;
		paddle[LEFT].triggered = 0;
		paddle[LEFT].power = paddlePowerRest;

		paddle[RIGHT].theta = paddleMaxTheta;
		paddle[RIGHT].p1.x = g->x + g->w - padding / 2;
		paddle[RIGHT].p1.y = g->y + g->h * 12 / 16;
		paddle[RIGHT].triggered = 0;
		paddle[RIGHT].power = paddlePowerRest;

		//PINBALL
		pinballPos.x = CP_Random_RangeFloat(bouncerCenter[0].x - pinballRadius, bouncerCenter[1].x + pinballRadius);
		pinballPos.y = g->y + g->h * 1 / 10;

		pinballTimeSinceLastJerk = CP_System_GetSeconds();

		g->init = 1;
	} else {
		//BOUNCERS
		stroke(BLACK, 4);
		fill(g->col);
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
		paddle[LEFT].p2.x = g->x + padding / 2 + (g->w * 23 / 64) * sin(CP_Math_Radians(paddle[LEFT].theta));
		paddle[LEFT].p2.y = g->y + g->h * 12 / 16 + (g->h * 2 / 16) * -cos(CP_Math_Radians(paddle[LEFT].theta));

		paddle[RIGHT].p2.x = g->x + g->w - padding / 2 - (g->w * 23 / 64) * sin(CP_Math_Radians(paddle[RIGHT].theta));
		paddle[RIGHT].p2.y = g->y + g->h * 12 / 16 + (g->h * 2 / 16) * -cos(CP_Math_Radians(paddle[RIGHT].theta));

		for (int i = 0; i < 2; i++) {
			stroke(BLACK, paddleRadius * 2);
			CP_Graphics_DrawLine(paddle[i].p1.x, paddle[i].p1.y, paddle[i].p2.x, paddle[i].p2.y);

			if (paddle[i].triggered) {
				if (paddle[i].theta > paddleMinTheta) { paddle[i].theta -= paddleAccSpeed; } else { paddle[i].triggered = 0; }
			} else if (paddle[i].theta < paddleMaxTheta) paddle[i].theta += paddleDecSpeed;

			//Circles on paddle for style and to get MiddleX MiddleY
			fill(g->col);
			stroke(BLACK, 0);
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

			CP_Vector normal = CP_Vector_Normalize(CP_Vector_Set(-deltaY, deltaX));
			CP_Vector pinballPointClosestToPaddle = CP_Vector_Add(pinballPos, CP_Vector_Scale(normal, pinballRadius));

			int condition = (i == LEFT) ? pinballPointClosestToPaddle.x < paddle[i].p2.x : pinballPointClosestToPaddle.x > paddle[i].p2.x;
			paddle[i].power = (paddle[i].theta < paddleMaxTheta) ? paddlePowerHit : paddlePowerRest;

			if (condition) {
				//Pinball is aiming for the flat surface of the paddle
				if (pinballPointClosestToPaddle.y > (slope * pinballPointClosestToPaddle.x) + yint) {
					pinballVel = CP_Vector_Scale(normal, paddle[i].power);
					pinballPos = CP_Vector_Subtract(pinballPos, normal);
				}
			} else {
				//Pinball is aiming for the curved head
				float dist = CP_Vector_Distance(pinballPos, paddle[i].p2);
				float oppLen = paddle[i].p2.y - pinballPos.y;
				float adjLen = paddle[i].p2.x - pinballPos.x;
				float theta = atan2f(oppLen, adjLen);

				if (dist < pinballRadius + paddleRadius) {
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
		float dist = CP_Vector_Distance(pinballPos, bouncerTriangle.center);
		float yLen = bouncerTriangle.center.y - pinballPos.y;
		float xLen = bouncerTriangle.center.x - pinballPos.x;
		float triTheta = atan2f(yLen, xLen);

		CP_Vector closest = CP_Vector_Add(
			pinballPos,
			CP_Vector_Set(
				pinballRadius * cos(triTheta),
				pinballRadius * sin(triTheta)
			)
		);

		if (isInside(bouncerTriangle.p1, bouncerTriangle.p2, bouncerTriangle.p3, closest)) {
			pinballVel.y = bouncerPower * sin(triTheta);
			pinballVel.x = bouncerPower * cos(triTheta);
		}

		//WALL COLLISION
		if (pinballPos.x < g->x + padding / 2 + pinballRadius) {
			pinballVel.x *= -1;
			pinballPos.x += (g->x + padding / 2 + pinballRadius) - pinballPos.x + 1;
		}
		if (pinballPos.x > g->x + g->w - padding / 2 - pinballRadius) {
			pinballVel.x *= -1;
			pinballPos.x -= pinballPos.x - (g->x + g->w - padding / 2 - pinballRadius) + 1;
		}
		if (pinballPos.y < g->y + padding / 2 + pinballRadius) {
			pinballVel.y *= -1;
			pinballVel.y += (g->y + padding / 2 + pinballRadius) - pinballPos.y + 1;
		}
		if (pinballPos.y > g->y + g->h - padding / 2 - pinballRadius) {
			pinballVel.y *= -1;
			pinballPos.y -= pinballPos.y - (g->y + g->h - padding / 2 - pinballRadius) + 1;
		}

		//LOSS CONDITION
		if (pinballPos.y > g->y + g->h * 13 / 16) {
			//Rounded estimate from lowest point of paddle
			//Pinball has gone too far

			pinballPos.y += pinballVel.y;
			pinballAlpha -= 30;
			pinballCol = CP_Color_Create(70, 70, 70, pinballAlpha);
			pinballStroke = CP_Color_Create(0, 0, 0, pinballAlpha);

			//TODO: Decrement Death. If deaths == 0, lose the game.

			if (pinballAlpha <= 0) {
				//Fade complete. Restart pinball location. 
				pinballPos.x = CP_Random_RangeFloat(bouncerCenter[0].x - pinballRadius, bouncerCenter[1].x + pinballRadius);
				pinballPos.y = g->y + g->h * 1 / 10;

				pinballCol = GRAY;
				pinballStroke = BLACK;

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

			//PINBALL JERK
			//To prevent the pinball from locking in a perfect angle
			//The pinball randomly jerks up
			if (pinballTimeSinceLastJerk + pinballJerkTimeIncrement < CP_System_GetSeconds()) {
				//Time to jerk!
				pinballVel.x += pinballJerkPower;
				pinballTimeSinceLastJerk = CP_System_GetSeconds();
			}
		}



		//PINBALL CONTROLS
		if (CP_Input_KeyTriggered(g->cont.keyLeft) && paddle[LEFT].theta >= paddleMaxTheta) paddle[LEFT].triggered = 1;
		if (CP_Input_KeyTriggered(g->cont.keyRight) && paddle[RIGHT].theta >= paddleMaxTheta) paddle[RIGHT].triggered = 1;

	}
}