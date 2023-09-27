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

float WIDTH, HEIGHT;
CP_Color BLACK, GRAY, WHITE, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA;

void settingsInit(void) {
	CP_System_Fullscreen();

	WIDTH = CP_System_GetWindowWidth();
	HEIGHT = CP_System_GetWindowHeight();

	CP_Settings_EllipseMode(CP_POSITION_CENTER);
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	BLACK = CP_Color_Create(0, 0, 0, 255);
	GRAY = CP_Color_Create(120, 120, 120, 255);
	WHITE = CP_Color_Create(255, 255, 255, 255);
	RED = CP_Color_Create(255, 0, 0, 255);
	YELLOW = CP_Color_Create(255, 255, 0, 255);
	GREEN = CP_Color_Create(0, 255, 0, 255);
	CYAN = CP_Color_Create(0, 255, 255, 255);
	BLUE = CP_Color_Create(0, 0, 255, 255);
	MAGENTA = CP_Color_Create(255, 0, 255, 255);

	CP_Settings_Fill(WHITE);
	CP_Settings_Stroke(BLACK);
	CP_Settings_StrokeWeight(10);
	CP_Settings_TextSize(40.0f);
	CP_Settings_NoTint();

	CP_Settings_Save();
}

void gameInit(void) {
	settingsInit();
}  

void gameUpdate(void) { 
	CP_Graphics_ClearBackground(BLACK);

	if (CP_Input_KeyReleased(KEY_R)) CP_Settings_Tint(RED);
	if (CP_Input_KeyReleased(KEY_G)) CP_Settings_Tint(GREEN);
	if (CP_Input_KeyReleased(KEY_B)) CP_Settings_Tint(BLUE);
	if (CP_Input_KeyReleased(KEY_C)) CP_Settings_Tint(CYAN);
	if (CP_Input_KeyReleased(KEY_Y)) CP_Settings_Tint(YELLOW);
	if (CP_Input_KeyReleased(KEY_M)) CP_Settings_Tint(MAGENTA);

	CP_Settings_Fill(GRAY);

	CP_Graphics_DrawRect(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

}

void gameExit(void) { 

}

void terminateController(void) {
	if (CP_Input_KeyReleased(KEY_ESCAPE) || CP_Input_KeyReleased(KEY_Q)) CP_Engine_Terminate();
}

int main(void)
{
	CP_Engine_SetPostUpdateFunction(terminateController);
	CP_Engine_SetNextGameState(gameInit, gameUpdate, gameExit);
	CP_Engine_Run();
	return 0;
}
