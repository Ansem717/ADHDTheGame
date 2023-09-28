//---------------------------------------------------------
// file:	utility.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Utility functions
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "utility.h"

void fill(CP_Color c) {
	CP_Settings_Fill(c);
}

void stroke(CP_Color c, float w) {
	CP_Settings_Stroke(c);
	CP_Settings_StrokeWeight(w);
}