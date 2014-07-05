#pragma once

#include "Constants.h"
#include <lib\DataArray.h>

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
	int corners;
	bool cleared;

	Tile() : state(TS_EMPTY) , color(0) , timer(0.0f) , corners(0) , cleared(false) {}

};

struct TileMap {

	uint32 width;
	uint32 height;
	Tile* tiles;

	TileMap() : tiles(0) , width(0) , height(0) {}
	~TileMap() {
		if ( tiles ) {
			delete[] tiles;
		}
	}

	void create(uint32 w,uint32 h) {
		tiles = new Tile[w * h];
		width = w;
		height = h;
	}
	
	const uint32 getIndex(uint32 x,uint32 y) const {
		return x + y * width;
	}

	Tile& get(uint32 x,uint32 y) {
		return tiles[getIndex(x,y)];
	}

	const Tile& get(uint32 x,uint32 y) const {
		return tiles[getIndex(x,y)];
	}

	void set(uint32 x,uint32 y,const Tile& tile) {
		tiles[getIndex(x,y)] = tile;
	}

	const bool isValid(uint32 x,uint32 y) const {
		if ( x >= width ) {
			return false;
		}
		if ( y >= height ) {
			return false;
		}
		return true;
	}

	// --------------------------------------------
	//
	// --------------------------------------------
	bool isCoherent(int gx,int gy) {
		Tile& t = get(gx,gy);
		if ( t.state == TS_EMPTY || t.state == TS_AVAILABLE ) {
			return false;
		}
		int color = t.color;
		int cnt = 0;
		for ( int i = 1; i < 4; ++i ) {
			Tile& n = get(gx + BLOCK_X[i], gy + BLOCK_Y[i]);
			if ( n.state == TS_MARKED || n.state == TS_FILLED ) {
				if ( n.color == color ) {
					++cnt;
				}
			}
		}
		if ( cnt == 3 ) {
			return true;		
		}
		return false;
	}

	// --------------------------------------------
	//
	// --------------------------------------------
	bool isBlockAvailable(int gx,int gy) {
		for ( int i = 0; i < 4; ++i ) {
			if ( !isFree( gx + BLOCK_X[i], gy + BLOCK_Y[i]) ) {
				return false;
			}
		}	
		return true;
	}

	// --------------------------------------------
	//
	// --------------------------------------------
	bool isFree(int gx,int gy) {
		Tile& t = get(gx,gy);
		return t.state == TS_AVAILABLE;	
	}
};

