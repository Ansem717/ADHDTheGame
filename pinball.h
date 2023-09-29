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

#include "gameObject.h"

typedef struct {
	CP_Vector p1;
	CP_Vector p2;
	float theta;
	int triggered;
	float power;
} Paddle;

void pinballInit(Game g);
void pinballPlay(Game g);

#endif
