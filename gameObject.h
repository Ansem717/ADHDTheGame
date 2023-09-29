//---------------------------------------------------------
// file:	
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	[DESCRIPTION]
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "cprocessing.h"

typedef struct Controller {
	CP_KEY keyLeft;
	CP_KEY keyRight;
} Controller;

typedef struct Game {
	int id;
	float x;
	float y;
	float w;
	float h;
	Controller cont;
	CP_Color col;
	int active;
	void (*init)(Game);
	void (*play)(Game);
} Game;

void	gameManagerInits	(void);
Game	getGame				(int id);
void	setGameDimensions	(int id, float x, float y, float w, float h);

#endif //GAME_OBJECT_H