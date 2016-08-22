#pragma once

#include "Constants.h"
#include <renderer\render_types.h>
#include <math\Bitset.h>
#include <string>
#include <dialogs\GUIDialog.h>
#include "utils\GameSettings.h"
#include <renderer\SpriteSheet.h>

struct Highscore {

	int score;
	int fillrate;
	int minutes;
	int seconds;
	int level;
	char name[12];

	Highscore() : score(0), fillrate(0), minutes(0), seconds(0), level(0) {}
};

class Levels;

struct GameContext {

	int levelIndex;
	int score;
	int fillRate;
	bool resume;
	Highscore currentScore;
	std::string name;
	ds::GUIDialog* hud;
	GameSettings* settings;
	ds::Color colors[8];
	Levels* levels;
	ds::SpriteSheet* spriteSheet;

	// http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
	void pick_colors() {
		// prepare colors
		float goldenRatioConjugate = 0.618033988749895f;
		float currentHue = math::random(0.0f, 1.0f);
		for (int i = 0; i < 8; i++) {
			ds::HSL hslColor = ds::HSL(currentHue * 360.0f, 50.0f, 50.0f);
			colors[i] = ds::color::hsl2rgb(hslColor);
			currentHue += goldenRatioConjugate;
			if (currentHue > 1.0f) {
				currentHue -= 1.0f;
			}
		}
	}
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

	Tile() : color(-1) , edges(0) , borders(0) {}

};
