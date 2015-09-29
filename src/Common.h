#pragma once

#include "Constants.h"
#include <renderer\render_types.h>
#include "SettingsConverter.h"
#include "FileManager.h"
#include <math\Bitset.h>

enum GameMode {
	GM_COVERAGE,
	GM_TIMER
};

struct Highscore {

	int score;
	int fillrate;
	int minutes;
	int seconds;
	int level;
	GameMode mode;
};

struct GameContext {

	int levelIndex;
	int score;
	int fillRate;
	bool resume;
	GameSettings* settings;
	Filesystem filesystem;
	GameMode gameMode;
	Highscore currentScore;
};

// Bits
const int BIT_AVAILABLE = 1;
const int BIT_MARKED    = 2;
const int BIT_COHERENT  = 3;
const int BIT_FILLED    = 4;

struct Tile {

	ds::Bits state;
	int color;
	int borders;
	int edges;
	ds::Texture texture;

	Tile() : color(-1) , edges(0) , borders(0) {}

};
