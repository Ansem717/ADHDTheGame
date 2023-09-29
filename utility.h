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

#include "gameObject.h"

float WIDTH, HEIGHT, MARGIN, PADDING;

typedef enum COLORS {
	BLACK,
	GRAY,
	LIGHT_GRAY,
	WHITE,
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	NONE
} COLORS;

CP_Color	getColor	(COLORS c, int a);
void		fill		(CP_Color c);
void		stroke		(CP_Color c, float w);

void	breakout(Game);
void	laneDriver(Game);
void	raftCollector(Game);

#endif