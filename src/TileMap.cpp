#include "TileMap.h"
#include<iostream>
#include<fstream>
#include <string>
#include <sprites\SpriteBatch.h>

const int MARK_STEPS[] = { 0, 0, 0, 1, 1, 1, 1, 0 };

bool TileMap::copyBlock(const Block& block) {
	v2 p = block.getPosition();
	int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
	int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
	if (isBlockAvailable(xp, yp)) {
		for (int i = 0; i < 4; ++i) {
			int cx = xp + MARK_STEPS[i * 2];
			int cy = yp + MARK_STEPS[i * 2 + 1];
			Tile& t = get(cx,cy);
			t.state = TS_MARKED;
			t.color = block.getColor(i);
			PointList list;
			check(cx, cy, -1, list, true);
			list.add(cx, cy);
			if (list.size() > 2) {
				//LOG << "connected";
				t.state = TS_COHERENT;
				for (size_t j = 0; j < list.size(); ++j){
					const Point& p = list.get(j);
					//LOG << j << " = " << p.x << " " << p.y;
					setState(p.x, p.y, TS_COHERENT);
				}
			}
		}		
		return true;
	}
	return false;
}

void TileMap::getColumn(int col, int* colors) {
	for (int y = 0; y < MAX_Y; ++y) {
		Tile& t = get(col, y);
		colors[y] = t.color;		
	}
}
// --------------------------------------------
// clear column
// --------------------------------------------
int TileMap::clearColumn(int col) {
	int ret = 0;
	for (int y = 0; y < MAX_Y; ++y) {
		Tile& t = get(col, y);
		t.color = -1;
		if (t.used) {
			if (t.state == TS_COHERENT) {
				++ret;
				t.state = TS_FILLED;
			}
			else if ( t.state == TS_MARKED ) {
				t.state = TS_AVAILABLE;
			}
		}
	}
	return ret;
}

// --------------------------------------------
// render
// --------------------------------------------
void TileMap::render() {
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			const Tile& t = get(x, y);
			v2 p = v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
			if (t.used) {
				if (t.state == TS_AVAILABLE) {
					ds::sprites::draw(p, ds::math::buildTexture(0, 0, 36, 36));
				}				
				else if (t.state == TS_MARKED) {
					ds::sprites::draw(p, ds::math::buildTexture(168, t.color * 36, 36, 36));
				}
				else if (t.state == TS_COHERENT) {
					ds::sprites::draw(p, ds::math::buildTexture(168, t.color * 36, 36, 36),0.0f,0.5f,0.5f);
				}				
				else if (t.state == TS_FILLED) {
					ds::sprites::draw(p, ds::math::buildTexture(0, 150, 36, 36));					
				}
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

// --------------------------------------------
// convert screen pos to aligned screen pos
// --------------------------------------------
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

// --------------------------------------------
// convert screen pos to grid pos
// --------------------------------------------
v2 TileMap::convert(int x, int y) {
	return v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
}

const uint32 TileMap::getIndex(uint32 x, uint32 y) const {
	return x + y * MAX_X;
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

// --------------------------------------------
// set state
// --------------------------------------------
void TileMap::setState(int x, int y, TileState state) {
	if (isValid(x, y)) {
		_tiles[getIndex(x, y)].state = state;
	}
}

// --------------------------------------------
// is valid
// --------------------------------------------
const bool TileMap::isValid(int x, int y) const {
	if (x < 0 || x >= MAX_X) {
		return false;
	}
	if (y < 0 || y >= MAX_Y) {
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
// get fill rate
// --------------------------------------------
int TileMap::getFillRate() {
	int count = 0;
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			const Tile& t = get(x, y);
			if (t.state == TS_FILLED) {
				++count;
			}
		}
	}
	LOGC("TileMap") << "max: " << _maxAvailable << " current: " << count;
	float per = static_cast<float>(count) / static_cast<float>(_maxAvailable) * 100.0f;
	return static_cast<int>(per);
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
	return t.state != TS_EMPTY;
}
// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isFree(int gx, int gy) {
	if (!isValid(gx, gy)) {
		return true;
	}
	Tile& t = get(gx, gy);
	return t.state == TS_AVAILABLE || t.state == TS_FILLED;
}

// -------------------------------------------------------
// reset
// -------------------------------------------------------
void TileMap::reset() {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = _tiles[x + y * MAX_X];
			t.borders = -1;
			t.color = -1;
			t.used = true;
			t.state = TS_AVAILABLE;
		}
	}
	_maxAvailable = 0;
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
	LOGC("TileMap") << "loading level: " << index;
	char buffer[128];
	sprintf_s(buffer, 128, "levels\\L%d", index);
	reset();
	FILE* f = fopen(buffer, "rb");
	if (f) {
		for (int y = 0; y < MAX_Y; ++y) {
			for (int x = 0; x < MAX_X; ++x) {
				Tile t;
				fread(&t, sizeof(Tile), 1, f);
				// make sure color is not set
				t.color = -1;
				if (t.state == TS_AVAILABLE) {
					++_maxAvailable;
				}
				set(x, y, t);
			}
		}
		fclose(f);
	}	
	LOGC("TileMap") << "max available: " << _maxAvailable;
}

bool TileMap::matches(int x, int y, const Tile& t) {
	return true;
}
// -------------------------------------------------------------
// check recursivley to detect matching pieces
// -------------------------------------------------------------
void TileMap::check(int xp, int yp, int lastDir, PointList& list, bool rec) {
	if (isValid(xp, yp)) {
		Tile& t = get(xp,yp);
		int color = t.color;
		if (color != -1) {
			for (int i = 0; i < 4; ++i) {
				if (i != lastDir) {
					int sx = xp + XM[i];
					int sy = yp + YM[i];
					if (isAvailable(sx, sy)) {
						Tile& nt = get(sx, sy);
						int nc = nt.color;
						if (nc != -1) {
							while (color == nc && color != -1) {
								bool recheck = !list.contains(sx, sy);
								list.add(sx, sy);
								if (recheck && rec) {
									check(sx, sy, LD[i], list, rec);
								}
								sx += XM[i];
								sy += YM[i];
								if (isValid(sx, sy)) {
									Tile& npe = get(sx, sy);
									nc = npe.color;
								}
								else {
									nc = -1;
								}
							}
						}
					}
				}
			}
		}
	}
}