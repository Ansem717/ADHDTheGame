//---------------------------------------------------------
// file:	main.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "main.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
#define LEFT 0
#define RIGHT 1

float WIDTH, HEIGHT;
CP_Color BLACK, GRAY, LIGHT_GRAY, WHITE, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, NONE;

typedef struct {
	CP_KEY keyLeft;
	CP_KEY keyRight;
} Controller;

typedef struct {
	int id;
	float x;
	float y;
	float w;
	float h;
	Controller cont;
	CP_Color col;
	int active;
	int init;
	void (*play)(Game);
} Game;

Controller CONTROLLERS[4];
Game GAMES[4];

float margin, padding;
char debugBuffer[50] = { 0 };

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

void fill(CP_Color c) {
	CP_Settings_Fill(c);
}

void stroke(CP_Color c, float w) {
	CP_Settings_Stroke(c);
	CP_Settings_StrokeWeight(w);
}

void settingsInit(void) {
#if DEBUG
	CP_System_SetWindowSize(1440, 900);
	//CP_System_SetFrameRate(2);
#else
	CP_System_Fullscreen();
#endif

	WIDTH = CP_System_GetWindowWidth() - margin * 2;
	HEIGHT = CP_System_GetWindowHeight();

	CP_Settings_EllipseMode(CP_POSITION_CENTER);
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_LineCapMode(CP_LINE_CAP_ROUND);

	fill(WHITE);
	stroke(BLACK, 2);
	CP_Settings_TextSize(40.0f);

	CP_Settings_Save();
}

void variablesInit(void) {
	/**************\
	| SYSTEM INITS |
	\**************/
	margin = 100.0f;
	padding = 30.0f;

	BLACK = CP_Color_Create(0, 0, 0, 255);
	GRAY = CP_Color_Create(70, 70, 70, 255);
	LIGHT_GRAY = CP_Color_Create(180, 180, 180, 255);
	WHITE = CP_Color_Create(255, 255, 255, 255);
	RED = CP_Color_Create(70, 0, 0, 255);
	YELLOW = CP_Color_Create(70, 70, 0, 255);
	GREEN = CP_Color_Create(0, 70, 0, 255);
	CYAN = CP_Color_Create(0, 70, 70, 255);
	BLUE = CP_Color_Create(0, 0, 70, 255);
	MAGENTA = CP_Color_Create(70, 0, 70, 255);
	NONE = CP_Color_Create(255, 255, 255, 0);

	CONTROLLERS[0].keyLeft = KEY_A;
	CONTROLLERS[0].keyRight = KEY_S;
	CONTROLLERS[1].keyLeft = KEY_D;
	CONTROLLERS[1].keyRight = KEY_F;
	CONTROLLERS[2].keyLeft = KEY_J;
	CONTROLLERS[2].keyRight = KEY_K;
	CONTROLLERS[3].keyLeft = KEY_L;
	CONTROLLERS[3].keyRight = KEY_SEMICOLON;

	/****************\
	| BREAKOUT INITS |
	\****************/
	GAMES[0].id = 0;
	GAMES[0].cont = CONTROLLERS[0];
	GAMES[0].col = RED;
	GAMES[0].active = 1;
	GAMES[0].init = 0;
	GAMES[0].play = &breakout;

	/***************\
	| PINBALL INITS |
	\***************/
	GAMES[1].id = 1;
	GAMES[1].cont = CONTROLLERS[1];
	GAMES[1].col = GREEN;
	GAMES[1].active = 1;
	GAMES[1].init = 0;
	GAMES[1].play = &pinball;

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

	/*******************\
	| LANE DRIVER INITS |
	\*******************/
	GAMES[2].id = 2;
	GAMES[2].cont = CONTROLLERS[2];
	GAMES[2].col = BLUE;
	GAMES[2].active = 1;
	GAMES[2].init = 0;
	GAMES[2].play = &laneDriver;

	/**********************\
	| RAFT COLLECTOR INITS |
	\**********************/
	GAMES[3].id = 3;
	GAMES[3].cont = CONTROLLERS[3];
	GAMES[3].col = YELLOW;
	GAMES[3].active = 1;
	GAMES[3].init = 0;
	GAMES[3].play = &raftCollector;

}

void gameInit(void) {
	variablesInit();
	settingsInit();
}

void gameUpdate(void) {
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Translate(margin, 0);

	float gameWidth = (WIDTH - padding * 3) / 4;
	float xStep = gameWidth + padding;

	for (int i = 0; i < 4; i++) {
		//for each GAME g
		Game* g = &GAMES[i];
		if (g->active) {
			fill(LIGHT_GRAY);
			stroke(g->col, padding);
			(*g).x = xStep * g->id + padding / 2;
			g->y = padding / 2;
			g->w = gameWidth;
			g->h = HEIGHT - padding;
			CP_Graphics_DrawRect(g->x, g->y, g->w, g->h);
			g->play(&GAMES[i]);
		}
	}
}

void breakout(Game g) {

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
		paddle[LEFT].p2.x = g->x + padding / 2 + (g->w * 11 / 32) * sin(CP_Math_Radians(paddle[LEFT].theta));
		paddle[LEFT].p2.y = g->y + g->h * 12 / 16 + (g->h * 2 / 16) * -cos(CP_Math_Radians(paddle[LEFT].theta));

		paddle[RIGHT].p2.x = g->x + g->w - padding / 2 - (g->w * 11 / 32) * sin(CP_Math_Radians(paddle[RIGHT].theta));
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
		float tdxl = bouncerTriangle.p1.x - bouncerTriangle.p2.x; //Triangle Delta X Left
		float tdyl = bouncerTriangle.p1.y - bouncerTriangle.p2.y; //Triangle Delta Y Left
		CP_Vector tnl = CP_Vector_Normalize(CP_Vector_Set(-tdyl, tdxl)); //Triangle Normal Left
		float tdxr = bouncerTriangle.p3.x - bouncerTriangle.p1.x; //Triangle Delta X Right
		float tdyr = bouncerTriangle.p3.y - bouncerTriangle.p1.y; //Triangle Delta Y Right
		CP_Vector tnr = CP_Vector_Normalize(CP_Vector_Set(-tdyl, tdxl)); //Triangle Normal Right
		CP_Vector tnb = CP_Vector_Set(0, 1); //Triangle Normal Bottom :: Flat line, so the vector is 0,1




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

void laneDriver(Game g) {

}

void raftCollector(Game g) {

}

void gameExit(void) {}

void adminController(void) {
	if (CP_Input_KeyReleased(KEY_ESCAPE) || CP_Input_KeyReleased(KEY_Q)) CP_Engine_Terminate();
	if (CP_Input_KeyReleased(KEY_R)) CP_Engine_SetNextGameStateForced(gameInit, gameUpdate, gameExit);
}

int main(void) {
	CP_Engine_SetPostUpdateFunction(adminController);
	CP_Engine_SetNextGameState(gameInit, gameUpdate, gameExit);
	CP_Engine_Run();
	return 0;
}
