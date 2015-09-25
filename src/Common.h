#pragma once

#include "Constants.h"
//#include <lib\DataArray.h>
//#include "..\..\math\Vector.h"
//#include <math\math_types.h>
#include <renderer\render_types.h>
#include "SettingsConverter.h"

struct GameContext {

	int levelIndex;
	int score;
	int fillRate;
	GameSettings* settings;
};

enum TileState {
	TS_EMPTY,
	TS_AVAILABLE,
	TS_MARKED,
	TS_COHERENT,
	TS_FILLED
};

struct Tile {

	TileState state;
	uint32 color;
	int borders;
	int edges;
	bool used;
	ds::Texture texture;

	Tile() : state(TS_EMPTY) , color(0) , used(false) , edges(0) , borders(0) {}

};
