#include "TileMap.h"
#include<iostream>
#include<fstream>
#include <string>
#include <sprites\SpriteBatch.h>

bool TileMap::copyBlock(const Block& block) {
	v2 p = block.getPosition();
	int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
	int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
	if (isBlockAvailable(xp, yp)) {
		Tile& t1 = get(xp, yp);
		t1.state = TS_MARKED;
		t1.color = block.getColor(0);
		Tile& t2 = get(xp + 1, yp);
		t2.state = TS_MARKED;
		t2.color = block.getColor(3);
		Tile& t3 = get(xp, yp + 1);
		t3.state = TS_MARKED;
		t3.color = block.getColor(1);
		Tile& t4 = get(xp + 1, yp + 1);
		t4.state = TS_MARKED;
		t4.color = block.getColor(2);
		return true;
	}
	return false;
}

void TileMap::render() {
	for (int x = 0; x < _width; ++x) {
		for (int y = 0; y < _height; ++y) {
			const Tile& t = get(x, y);
			v2 p = v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
			if (t.used) {
				if (t.state == TS_AVAILABLE) {
					ds::sprites::draw(p, ds::math::buildTexture(0, 0, 36, 36));
				}				
				else if (t.state == TS_MARKED) {
					ds::sprites::draw(p, ds::math::buildTexture(168, t.color * 36, 36, 36));
				}
				/*
				else if (t.state == TS_FILLED) {
					int offset = t.edges * 36;
					if (t.edges > 0) {
						ds::sprites::draw(p, ds::math::buildTexture(190, offset, 36, 36), 0.0f, 1.0f, 1.0f, BLOCK_COLORS[t.color]);
					}
					else {
						ds::sprites::draw(p, ds::math::buildTexture(190, 540, 36, 36), 0.0f, 1.0f, 1.0f, BLOCK_COLORS[t.color]);
					}
				}
				*/
			}
		}
	}
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			const Tile& t = get(x, y);
			v2 p = v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
			if ( t.borders != -1 ) {
				ds::sprites::draw(p, ds::math::buildTexture(44, 44 * t.borders, BORDER_SIZE, BORDER_SIZE));
			}
		}
	}
}

v2 TileMap::convert(const v2& p) {
	int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
	int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
	if (xp < 0) {
		xp = 0;
	}
	if (xp >= (MAX_X-1)) {
		xp = MAX_X - 2;
	}
	if (yp < 0) {
		yp = 0;
	}
	if (yp >= (MAX_Y-1)) {
		yp = MAX_Y - 2;
	}
	return convert(xp, yp);
}

v2 TileMap::convert(int x, int y) {
	return v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
}

const uint32 TileMap::getIndex(uint32 x, uint32 y) const {
	return x + y * _width;
}

Tile& TileMap::get(int x, int y) {
	return _tiles[getIndex(x, y)];
}

const Tile& TileMap::get(int x, int y) const {
	return _tiles[getIndex(x, y)];
}

void TileMap::set(int x, int y, const Tile& tile) {
	_tiles[getIndex(x, y)] = tile;
}

const bool TileMap::isValid(int x, int y) const {
	if (x < 0 || x >= _width) {
		return false;
	}
	if (y < 0 || y >= _height) {
		return false;
	}
	return true;
}

// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isCoherent(int gx, int gy) {
	Tile& t = get(gx, gy);
	if (t.state == TS_EMPTY || t.state == TS_AVAILABLE) {
		return false;
	}
	int color = t.color;
	int cnt = 0;
	for (int i = 1; i < 4; ++i) {
		Tile& n = get(gx + BLOCK_X[i], gy + BLOCK_Y[i]);
		if (n.state == TS_MARKED || n.state == TS_FILLED) {
			if (n.color == color) {
				++cnt;
			}
		}
	}
	if (cnt == 3) {
		return true;
	}
	return false;
}

// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isBlockAvailable(int gx, int gy) {
	for (int i = 0; i < 4; ++i) {
		if (!isFree(gx + BLOCK_X[i], gy + BLOCK_Y[i])) {
			return false;
		}
	}
	return true;
}

bool TileMap::isAvailable(int gx, int gy){
	if (!isValid(gx, gy)) {
		return false;
	}
	Tile& t = get(gx, gy);
	return t.state == TS_AVAILABLE;
}
// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isFree(int gx, int gy) {
	if (!isValid(gx, gy)) {
		return true;
	}
	Tile& t = get(gx, gy);
	return t.state == TS_AVAILABLE;
}

// -------------------------------------------------------
// reset
// -------------------------------------------------------
void TileMap::reset() {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = _tiles[x + y * MAX_X];
			t.borders = -1;
			t.used = true;
			t.state = TS_AVAILABLE;
		}
	}
}

// -------------------------------------------------------
// determine corners
// -------------------------------------------------------
int TileMap::determineEdge(int x, int y) {
	int set = 0;
	if (isAvailable(x, y)) {
		if (isAvailable(x - 1, y)) {
			set |= 1;
		}
		if (isAvailable(x + 1, y)) {
			set |= 2;
		}
		if (isAvailable(x, y - 1)) {
			set |= 4;
		}
		if (isAvailable(x, y + 1)) {
			set |= 8;
		}
	}
	return set;
}

void TileMap::setBorder(int x, int y, int index) {
	if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
		_tiles[x + y * MAX_X].borders = index;
	}
}

// --------------------------------------------
// save
// --------------------------------------------
void TileMap::save(int index) {
	char buffer[128];
	sprintf_s(buffer, 128, "levels\\L%d", index);
	FILE* f = fopen(buffer, "wb");
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			const Tile& t = get(x, y);
			fwrite(&t, sizeof(Tile), 1, f);
		}
	}
	fclose(f);
}

// --------------------------------------------
// load
// --------------------------------------------
void TileMap::load(int index) {
	char buffer[128];
	sprintf_s(buffer, 128, "levels\\L%d", index);
	reset();
	FILE* f = fopen(buffer, "rb");
	if (f) {
		for (int y = 0; y < MAX_Y; ++y) {
			for (int x = 0; x < MAX_X; ++x) {
				Tile t;
				fread(&t, sizeof(Tile), 1, f);
				set(x, y, t);
			}
		}
	}
	fclose(f);
}