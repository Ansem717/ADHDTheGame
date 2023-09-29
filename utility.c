//---------------------------------------------------------
// file:	utility.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Utility functions
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "utility.h"

CP_Color getColorWithAlpha(COLORS c, int a) {
	switch (c) {
	case BLACK:
		return CP_Color_Create(0, 0, 0, a);
	case GRAY:
		return CP_Color_Create(140, 140, 140, a);
	case LIGHT_GRAY:
		return CP_Color_Create(180, 180, 180, a);
	case WHITE:
		return CP_Color_Create(255, 255, 255, a);
	case RED:
		return CP_Color_Create(140, 0, 0, a);
	case YELLOW:
		return CP_Color_Create(140, 140, 0, a);
	case GREEN:
		return CP_Color_Create(0, 140, 0, a);
	case CYAN:
		return CP_Color_Create(0, 140, 140, a);
	case BLUE:
		return CP_Color_Create(0, 0, 140, a);
	case MAGENTA:
		return CP_Color_Create(140, 0, 140, a);
	case NONE:
		return CP_Color_Create(255, 255, 255, 0);
	default:
		return CP_Color_Create(0, 0, 0, a);
	}
}

CP_Color getColor(COLORS c) {
	return getColorWithAlpha(c, 255);
}

void fill(CP_Color c) { CP_Settings_Fill(c); }

void stroke(CP_Color c, float w) {
	CP_Settings_Stroke(c);
	CP_Settings_StrokeWeight(w);
}
