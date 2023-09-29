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
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "utility.h"

void gameInit(void) {

	CP_System_SetWindowSize(1440, 900);

	MARGIN = 100.0f;
	PADDING = 30.0f;
	WIDTH = CP_System_GetWindowWidth() - MARGIN * 2;
	HEIGHT = CP_System_GetWindowHeight();

	CP_Settings_EllipseMode(CP_POSITION_CENTER);
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_LineCapMode(CP_LINE_CAP_ROUND);

	fill(getColor(LIGHT_GRAY));
	stroke(getColor(RED), 2);
	CP_Settings_TextSize(40.0f);

	CP_Settings_Save();

}

void gameUpdate(void) {
	CP_Graphics_ClearBackground(getColor(BLUE));
	CP_Settings_Translate(MARGIN, 0);

	CP_Graphics_DrawCircle(WIDTH / 2, HEIGHT / 2, 100);

}

void gameExit(void) {

}

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
