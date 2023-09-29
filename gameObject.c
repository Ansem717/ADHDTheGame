//---------------------------------------------------------
// file:	gameObject.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Manages the "Games" that are running
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "gameObject.h"
#include "utility.h"
#include "pinball.h"
#include "breakout.h"
#include "lanedriver.h"
#include "raftcollector.h"

Controller CONTROLLERS[4];
Game GAMES[4];

void gameManagerInits(void) {
	CONTROLLERS[0].keyLeft = KEY_A;
	CONTROLLERS[0].keyRight = KEY_S;
	CONTROLLERS[1].keyLeft = KEY_D;
	CONTROLLERS[1].keyRight = KEY_F;
	CONTROLLERS[2].keyLeft = KEY_J;
	CONTROLLERS[2].keyRight = KEY_K;
	CONTROLLERS[3].keyLeft = KEY_L;
	CONTROLLERS[3].keyRight = KEY_SEMICOLON;

	/***************\
	| GAME OBJ INIT |
	\***************/
	for (int i = 0; i < 4; i++) {
		GAMES[i].id = i;
		GAMES[i].cont = CONTROLLERS[i];
		GAMES[i].active = 1;
	}

	/**********\
	| BREAKOUT |
	\**********/
	GAMES[0].col = getColor(RED);
	GAMES[0].init = &breakoutInit;
	GAMES[0].play = &breakoutPlay;

	/*********\
	| PINBALL |
	\*********/
	GAMES[1].col = getColor(GREEN);
	GAMES[1].init = &pinballInit;
	GAMES[1].play = &pinballPlay;

	/*************\
	| LANE DRIVER |
	\*************/
	GAMES[2].col = getColor(YELLOW);
	GAMES[2].init = &laneDriverInit;
	GAMES[2].play = &laneDriverPlay;

	/****************\
	| RAFT COLLECTOR |
	\****************/
	GAMES[3].col = getColor(BLUE);
	GAMES[3].init = &raftCollectorInit;
	GAMES[3].play = &raftCollectorPlay;
}

Game getGame(int id) { return GAMES[id]; }

void setGameDimensions(int id, float x, float y, float w, float h) {
	GAMES[id].x = x;
	GAMES[id].y = y;
	GAMES[id].w = w;
	GAMES[id].h = h;
}

void setGameColor(int id, CP_Color c) { GAMES[id].col = c; }
