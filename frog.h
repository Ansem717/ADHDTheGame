//---------------------------------------------------------
// file:	frog.h
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef FROG_H
#define FROG_H

#include "utility.h";

typedef struct Frog {
	CP_Vector pos;
	CP_Vector dim;
	CP_Vector leg; //x is length, y is weight
	int movingDirection;
	int isAlive;
	float timeOfDeath;
	float timeToWaitBeforeRespawning;
	float respawnChance;
} Frog;

void setPos(Frog f, float x, float y);
void setPos2(Frog f, CP_Vector pos);
void setDim(Frog f, float w, float h);
void setDim2(Frog f, CP_Vector dim);
void setLeg(Frog f, float length, float weight);
void setLeg2(Frog f, CP_Vector leg);
float posX(Frog f);
float posY(Frog f);
float dimW(Frog f);
float dimH(Frog f);
float legL(Frog f);
float legW(Frog f);


#endif