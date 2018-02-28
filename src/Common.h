#pragma once
#include <diesel.h>
#include "colors.h"
#include "utils/Bitset.h"
#include "utils\GameTimer.h"

const int MAX_LEVELS = 10;
const int MAX_X = 28;
const int MAX_Y = 16;
const int BORDER_SIZE = 44;
const int SQUARE_SIZE = 36;
const int HALF_SQUARE_SIZE = 18;
const int START_X = (1280 - SQUARE_SIZE * MAX_X) / 2;
const int START_Y = (720 - SQUARE_SIZE * MAX_Y) / 2;

const int XM[4] = { 1, -1, 0, 0 };
const int YM[4] = { 0, 0, 1, -1 };
const int LD[4] = { 1, 0, 3, 2 };

const int BLOCK_X[4] = { 0,1,1,0 };
const int BLOCK_Y[4] = { 0,0,1,1 };

struct GameSettings;
class SpriteBatchBuffer;
class TileMap;
struct LevelData;
// Bits
const int BIT_AVAILABLE = 1;
const int BIT_MARKED = 2;
const int BIT_COHERENT = 3;
const int BIT_FILLED = 4;

struct Tile {

	ds::Bits state;
	int color;
	int borders;
	int edges;

	Tile() : color(-1), edges(0), borders(0) {}

};


struct GameContext {

	int score;
	int fillRate;
	ds::Color colors[8];
	GameSettings* settings;

	// http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
	void pick_colors() {
		// prepare colors
		float goldenRatioConjugate = 0.618033988749895f;
		float currentHue = ds::random(0.0f, 1.0f);
		for (int i = 0; i < 8; i++) {
			HSL hslColor = HSL(currentHue * 360.0f, 50.0f, 50.0f);
			colors[i] = color::hsl2rgb(hslColor);
			currentHue += goldenRatioConjugate;
			if (currentHue > 1.0f) {
				currentHue -= 1.0f;
			}
		}
	}
};

