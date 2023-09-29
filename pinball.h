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

#include "cprocessing.h"
#include "gameObject.h"

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

float	area				(CP_Vector p1, CP_Vector p2, CP_Vector p3);
int		isInside			(CP_Vector p1, CP_Vector p2, CP_Vector p3, CP_Vector pq);

void	pinballInitVars		(Game* g);
void	pinball				(Game* g);

#endif
