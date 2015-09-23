#pragma once

#include "Constants.h"
#include <lib\DataArray.h>
#include "..\..\math\Vector.h"
#include <math\math_types.h>
#include <renderer\render_types.h>

struct Edge {

	Vector2f position;
	ds::Texture texture;
	int bits;
};

enum TileState {
	TS_EMPTY,
	TS_AVAILABLE,
	TS_MARKED,
	TS_FILLED
};

struct Tile {

	TileState state;
	uint32 color;
	float timer;
	int borders;
	bool cleared;
	bool used;
	ds::Texture texture;
	v2 borderOffset;
	v2 borderScale;

	Tile() : state(TS_EMPTY) , color(0) , timer(0.0f) , cleared(false) , used(false) , borderOffset(0,0) , borderScale(0,0) {}

};

struct Piece {

	ds::Texture texture;
	Vector2f position;
	ds::Color colorValue;

};

struct MapDefinition {

	int width;
	int height;
	int* tiles;

	MapDefinition() : width(0), height(0), tiles(0) {}
	~MapDefinition() {
		if (tiles != 0) {
			delete[] tiles;
		}
	}
	void create(int w, int h) {
		width = w;
		height = h;
		tiles = new int[w * h];
	}

	const bool isAvailable(int x, int y) const {
		if (x < 0 || x >= width) {
			return false;
		}
		if (y < 0 || y >= height) {
			return false;
		}
		return tiles[x + y * width] == 1;
	}
};


