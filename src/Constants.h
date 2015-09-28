#pragma once
#include <dxstdafx.h>
#include <utils\Color.h>
#include <lib\container\GridArray.h>

const int MAX_X = 26;
const int MAX_Y = 16;
const int HALF_MAX_X = 13;
const int HALF_MAX_Y = 8;
const int BORDER_SIZE = 44;

//const float ADD_X[4] = { 0.0f, 35.0f , 35.0f ,  0.0f};
//const float ADD_Y[4] = { 0.0f,  0.0f , 35.0f , 35.0f};

const int XM[4] = { 1, -1, 0, 0 };
const int YM[4] = { 0, 0, 1, -1 };
const int LD[4] = { 1, 0, 3, 2 };

//const int START_X = 50;
//const int START_Y = 100;
const int SQUARE_SIZE = 36;

const int BLOCK_X[4] = {0,1,1,0};
const int BLOCK_Y[4] = {0,0,1,1};

//const int LASER_PIECES = 16;

//const int STARTX = 110;
//const int STARTY = 150;

const float PARTICLE_ANGLES[] = {135.0f,110.0f,70.0f,45.0f,160.0f,110.0f,70.0f,20.0f,200.0f,200.0f,340.0f,340.0f,225.0f,250.0f,290.0f,325.0f};

struct Score {

	uint32 points;
	uint32 seconds;
	uint32 coverage;
	uint32 level;

};

