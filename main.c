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

float WIDTH, HEIGHT;
CP_Color BLACK, GRAY, LIGHT_GRAY, WHITE, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA;

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

CP_Vector bouncerCenter[4];
float bouncerSize, bouncerPower;
float paddleLTheta, paddleRTheta, paddleMaxTheta, paddleMinTheta;
int paddleLTriggered, paddleRTriggered;
float paddleAccSpeed, paddleDecSpeed;
CP_Vector pinballPos, pinballVel;
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
	CP_System_SetFrameRate(10);
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

	bouncerSize = 80;
	bouncerPower = -10;

	paddleLTheta = 120;
	paddleRTheta = 120;
	paddleMaxTheta = 120;
	paddleMinTheta = 60;
	paddleLTriggered = paddleRTriggered = 0;
	paddleAccSpeed = 16;
	paddleDecSpeed = 5;

	gravity = 0.5;
	terminalVelocity = 8;
	pinballPos = CP_Vector_Zero();
	pinballVel = CP_Vector_Zero();

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
		bouncerCenter[0].x = g->x + g->w * 1 / 4;
		bouncerCenter[0].y = g->y + g->h * 6 / 32;
		bouncerCenter[1].x = g->x + g->w * 3 / 4;
		bouncerCenter[1].y = g->y + g->h * 6 / 32;
		bouncerCenter[2].x = g->x + g->w * 2 / 4;
		bouncerCenter[2].y = g->y + g->h * 12 / 32;
		bouncerCenter[3].x = g->x + g->w * 8 / 16;
		bouncerCenter[3].y = g->y + g->h * 77 / 128;

		//PINBALL
		pinballPos.x = g->x + g->w * 1 / 2 + 10;
		pinballPos.y = g->y + g->h * 1 / 6;

		g->init = 1;
	} else {
		//BOUNCERS
		stroke(BLACK, 4);
		fill(g->col);
		for (int i = 0; i < 3; i++) {
			CP_Graphics_DrawCircle(bouncerCenter[i].x, bouncerCenter[i].y, bouncerSize);
		}
		CP_Graphics_DrawTriangle(
			g->x + g->w * 8 / 16,
			g->y + g->h * 18 / 32,
			g->x + g->w * 7 / 16,
			g->y + g->h * 20 / 32,
			g->x + g->w * 9 / 16,
			g->y + g->h * 20 / 32
		);

		//PADDLES
		stroke(BLACK, 40);
		if (paddleLTriggered) {
			if (paddleLTheta > paddleMinTheta) { paddleLTheta -= paddleAccSpeed; } else { paddleLTriggered = 0; }
		} else if (paddleLTheta < paddleMaxTheta) paddleLTheta += paddleDecSpeed;

		if (paddleRTriggered) {
			if (paddleRTheta > paddleMinTheta) { paddleRTheta -= paddleAccSpeed; } else { paddleRTriggered = 0; }
		} else if (paddleRTheta < paddleMaxTheta) paddleRTheta += paddleDecSpeed;

		CP_Graphics_DrawLine(
			g->x + padding / 2,
			g->y + g->h * 12 / 16,
			g->x + padding / 2 + (g->w * 3 / 8) * sin(CP_Math_Radians(paddleLTheta)),
			g->y + g->h * 12 / 16 + (g->h * 2 / 16) * -cos(CP_Math_Radians(paddleLTheta))
		);

		CP_Graphics_DrawLine(
			g->x + g->w - padding / 2,
			g->y + g->h * 12 / 16,
			g->x + g->w - padding / 2 - (g->w * 3 / 8) * sin(CP_Math_Radians(paddleRTheta)),
			g->y + g->h * 12 / 16 + (g->h * 2 / 16) * -cos(CP_Math_Radians(paddleRTheta))
		);


		//PINBALL
		stroke(BLACK, 2);
		fill(GRAY);
		float circleRadius = 15;
		CP_Graphics_DrawCircle(pinballPos.x, pinballPos.y, circleRadius * 2);
		for (int i = 0; i < 3; i++) {
			float dist = CP_Vector_Distance(pinballPos, bouncerCenter[i]);
			float oppLen = bouncerCenter[i].y - pinballPos.y;
			float adjLen = bouncerCenter[i].x - pinballPos.x;
			float hypo = sqrt(oppLen * oppLen + adjLen * adjLen);
			float theta = acos(adjLen / hypo);

			char buffer[50] = { 0 };
			sprintf_s(buffer, _countof(buffer), "%.2f", pinballVel.y);
			fill(LIGHT_GRAY);
			CP_Graphics_DrawRect(WIDTH * 1 / 6, HEIGHT * 1 / 3, WIDTH * 1 / 7, HEIGHT * 1 / 5);
			fill(BLACK);
			CP_Font_DrawText(buffer, WIDTH * 1 / 5, HEIGHT / 2);

			//CP_Graphics_DrawTriangle(pinballPos.x, pinballPos.y, bouncerCenter[i].x, bouncerCenter[i].y, bouncerCenter[i].x, pinballPos.y);
			if (dist < circleRadius + (bouncerSize / 2)) {
				pinballVel.y = bouncerPower * sin(theta);
				pinballVel.x = bouncerPower * cos(theta);

			}
		}

		if (pinballPos.x < g->x + padding / 2 + circleRadius ||
			pinballPos.x > g->x + g->w - padding / 2 - circleRadius) pinballVel.x *= -1;
		if (pinballPos.y < g->y + padding / 2 + circleRadius ||
			pinballPos.y > g->y + g->h - padding / 2 - circleRadius) pinballVel.y *= -1;

		pinballPos = CP_Vector_Add(pinballPos, pinballVel);
		if (pinballVel.y <= terminalVelocity) {
			pinballVel.y += gravity;
		} else {
			pinballVel.y = terminalVelocity;
		}

		/*if (pinballVel.x < 0) {
			pinballVel.x += gravity;
		} else if (pinballVel.x > 0) {
			pinballVel.x -= gravity;
		}*/


		if (CP_Input_KeyTriggered(g->cont.keyLeft) && paddleLTheta >= paddleMaxTheta) paddleLTriggered = 1;
		if (CP_Input_KeyTriggered(g->cont.keyRight) && paddleRTheta >= paddleMaxTheta) paddleRTriggered = 1;
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
