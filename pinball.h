//---------------------------------------------------------
// file:	pinball.h
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Pinball Header
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef PINBALL_H
#define PINBALL_H

typedef struct {
	CP_Vector p1;
	CP_Vector p2;
	float theta;
	int triggered;
	float power;
} Paddle;

typedef struct {
	CP_Vector p1;
	CP_Vector p2;
	CP_Vector p3;
	CP_Vector center;
	float area;
} Triangle;

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

float	area				(CP_Vector p1, CP_Vector p2, CP_Vector p3);
int		isInside			(CP_Vector p1, CP_Vector p2, CP_Vector p3, CP_Vector pq);

void	pinballInitVars		(void);
void	pinball				(Game* g);

#endif
