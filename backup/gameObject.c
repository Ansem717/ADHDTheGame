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

Controller CONTROLLERS[4];
Game GAMES[4];

void inits() {
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
	GAMES[0].col = getColor(RED, 255);
	GAMES[0].active = 1;
	GAMES[0].init = 0;
	GAMES[0].play = &breakout;

	/***************\
	| PINBALL INITS |
	\***************/
	GAMES[1].id = 1;
	GAMES[1].cont = CONTROLLERS[1];
	GAMES[1].col = getColor(GREEN, 255);
	GAMES[1].active = 1;
	GAMES[1].init = 0;
	GAMES[1].play = &pinball;

	/*******************\
	| LANE DRIVER INITS |
	\*******************/
	GAMES[2].id = 2;
	GAMES[2].cont = CONTROLLERS[2];
	GAMES[2].col = getColor(BLUE, 255);
	GAMES[2].active = 1;
	GAMES[2].init = 0;
	GAMES[2].play = &laneDriver;

	/**********************\
	| RAFT COLLECTOR INITS |
	\**********************/
	GAMES[3].id = 3;
	GAMES[3].cont = CONTROLLERS[3];
	GAMES[3].col = getColor(YELLOW, 255);
	GAMES[3].active = 1;
	GAMES[3].init = 0;
	GAMES[3].play = &raftCollector;
}

Game* getGame(int id) {
	return &GAMES[id];
}