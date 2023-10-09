//---------------------------------------------------------
// file:	frog.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "frog.h"

Frog setPos(Frog f, float x, float y) {
    f.pos.x = x;
    f.pos.y = y;
    return f;
}

Frog setPos2(Frog f, CP_Vector pos) {
    f.pos = pos;
    return f;
}

Frog setDim(Frog f, float w, float h) {
    f.dim.x = w;
    f.dim.y = h;
    return f;
}

Frog setDim2(Frog f, CP_Vector dim) {
    f.dim = dim;
    return f;
}

Frog setLeg(Frog f, float length, float weight) {
    f.leg.x = length;
    f.leg.y = weight;
    return f;
}

Frog setLeg2(Frog f, CP_Vector leg) {
    f.leg = leg;
    return f;
}


float posX(Frog f) {
	return f.pos.x;
}

float posY(Frog f) {
	return f.pos.y;
}

float dimW(Frog f) {
	return f.dim.x;
}

float dimH(Frog f) {
	return f.dim.y;
}

float legL(Frog f) {
	return f.leg.x;
}

float legW(Frog f) {
	return f.leg.y;
}