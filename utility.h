//---------------------------------------------------------
// file:	main.h
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	header file to supplement main?
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef UTILITY_H
#define UTILITY_H

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

void	fill(CP_Color c);
void	stroke(CP_Color c, float w);

void	breakout(Game);
void	laneDriver(Game);
void	raftCollector(Game);

#endif