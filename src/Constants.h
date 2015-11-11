#pragma once
#include <utils\Color.h>

const int MAX_X = 26;
const int MAX_Y = 16;
const int BORDER_SIZE = 44;

const int MAX_LEVELS = 16;

const int XM[4] = { 1, -1, 0, 0 };
const int YM[4] = { 0, 0, 1, -1 };
const int LD[4] = { 1, 0, 3, 2 };

const int SQUARE_SIZE = 36;
const float HALF_SQUARE_SIZE = 18.0f;

const int BLOCK_X[4] = {0,1,1,0};
const int BLOCK_Y[4] = {0,0,1,1};

const float PARTICLE_ANGLES[] = {
	135.0f,110.0f, 70.0f, 45.0f,
	160.0f,110.0f, 70.0f, 20.0f,
	200.0f,200.0f,340.0f,340.0f,
	225.0f,250.0f,290.0f,325.0f
};


const int MAX_SCORE_ENTRIES = 5;

const int HUD_TIMER = 5;
const int HUD_PERCENTAGE = 3;
const int HUD_SCORE = 4;

const ds::Color TILE_COLORS[] = {
	ds::Color(217, 48, 79, 255),
	ds::Color( 26, 49, 64, 255),
	ds::Color(101,159,166, 255),
	ds::Color(242,120,109, 255)
};