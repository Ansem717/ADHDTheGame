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
// Copyright � 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gameObject.h"
#include "utility.h"
#include "pinball.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void settingsInit(void) {

#if DEBUG
	CP_System_SetWindowSize(1440, 900);
	//CP_System_SetFrameRate(2);
#else
	CP_System_Fullscreen();
#endif

	WIDTH = CP_System_GetWindowWidth() - MARGIN * 2;
	HEIGHT = CP_System_GetWindowHeight();
	MARGIN = 100.0f;
	PADDING = 30.0f;

	CP_Settings_EllipseMode(CP_POSITION_CENTER);
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_LineCapMode(CP_LINE_CAP_ROUND);

	fill(getColor(WHITE, 255));
	stroke(getColor(BLACK, 255), 2);
	CP_Settings_TextSize(40.0f);

	CP_Settings_Save();
}

void gameInit(void) {
	settingsInit();
	inits();

	for (int i = 0; i < 4; i++) {
		//for each GAME g
		Game* g = getGame(i);
		float gameWidth = (WIDTH - PADDING * 3) / 4;
		float xStep = gameWidth + PADDING;
		g->x = xStep * g->id + PADDING / 2;
		g->y = PADDING / 2;
		g->w = gameWidth;
		g->h = HEIGHT - PADDING;
		g->init(getGame(i));
	}
}

void gameUpdate(void) {
	CP_Graphics_ClearBackground(getColor(BLACK, 255));
	CP_Settings_Translate(MARGIN, 0);

	for (int i = 0; i < 4; i++) {
		//for each GAME g
		Game* g = getGame(i);
		if (g->active) {
			fill(getColor(LIGHT_GRAY, 255));
			stroke(g->col, PADDING);
			CP_Graphics_DrawRect(g->x, g->y, g->w, g->h);

			g->play(getGame(i));
		}
	}
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
