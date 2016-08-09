#include "TileMap.h"
#include<iostream>
#include<fstream>
#include <string>

const int MARK_STEPS[] = { 0, 0, 0, 1, 1, 1, 1, 0 };

// --------------------------------------------
// copy column
// --------------------------------------------
bool TileMap::copyBlock(const Block& block) {
	v2 p = block.getPosition();
	int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
	int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
	if (isBlockAvailable(xp, yp)) {
		for (int i = 0; i < 4; ++i) {
			int cx = xp + MARK_STEPS[i * 2];
			int cy = yp + MARK_STEPS[i * 2 + 1];
			Tile& t = get(cx,cy);
			t.state.set(BIT_MARKED);
			t.color = block.getColor(i);
			PointList list;
			check(cx, cy, -1, list, true);
			list.add(cx, cy);
			if (list.size() > 2) {
				//LOG << "connected";
				t.state.set(BIT_COHERENT);
				for (size_t j = 0; j < list.size(); ++j){
					const p2i& p = list.get(j);
					//LOG << j << " = " << p.x << " " << p.y;
					setState(p.x, p.y, BIT_COHERENT);
				}
			}
		}	
		determineEdges();
		return true;
	}
	else {
		LOG << "Block is not available";
	}
	return false;
}

// --------------------------------------------
// get column
// --------------------------------------------
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
		if (t.state.isSet(BIT_AVAILABLE)) {
			if (!t.state.isSet(BIT_FILLED)) {
				if (t.state.isSet(BIT_COHERENT)) {
					++ret;
					t.state.set(BIT_FILLED);
				}
			}
			t.state.clear(BIT_MARKED);
			t.state.clear(BIT_COHERENT);
			//t.borders = -1;
			t.edges = -1;
		}
	}
	return ret;
}

// --------------------------------------------
// render
// --------------------------------------------
void TileMap::render() {	
	render(SQUARE_SIZE, 1.0f);
}

// --------------------------------------------
// render
// --------------------------------------------
void TileMap::render(int squareSize,float scale) {
	int startX = (1024 - squareSize * MAX_X) / 2;
	int startY = ( 768 - squareSize * MAX_Y) / 2;
	ds::Color clr = ds::Color::WHITE;
	ds::Texture tex;
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			clr = ds::Color::WHITE;
			const Tile& t = get(x, y);
			v2 p = v2(startX + x * squareSize, startY + y * squareSize);
			if (t.state.isSet(BIT_AVAILABLE)) {	
				if (t.state.isSet(BIT_COHERENT)) {
					if (t.edges > 0) {
						clr = TILE_COLORS[t.color];
						tex = math::buildTexture(168, 410 + t.edges * 36, 36, 36);
					}
				}
				else if (t.state.isSet(BIT_MARKED)) {
					tex = math::buildTexture(168, 200 + t.color * 36, 36, 36);
				}
				else if (t.state.isSet(BIT_FILLED)) {
					tex = math::buildTexture(0, 150, 36, 36);
				}
				else {
					tex = math::buildTexture(0, 0, 36, 36);
				}

				sprites->draw(p, tex, 0.0f, v2(scale, scale), clr);

			}
		}
	}
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			const Tile& t = get(x, y);
			v2 p = v2(startX + x * squareSize, startY + y * squareSize);
			if (t.borders != -1) {
				sprites->draw(p, math::buildTexture(44, 44 * t.borders, BORDER_SIZE, BORDER_SIZE), 0.0f, v2(scale, scale));
			}
		}
	}
}

// --------------------------------------------
// get index
// --------------------------------------------
const uint32_t TileMap::getIndex(uint32_t x, uint32_t y) const {
	return x + y * MAX_X;
}

// --------------------------------------------
// get
// --------------------------------------------
Tile& TileMap::get(int x, int y) {
	return _tiles[getIndex(x, y)];
}

// --------------------------------------------
// get - const version
// --------------------------------------------
const Tile& TileMap::get(int x, int y) const {
	return _tiles[getIndex(x, y)];
}

// --------------------------------------------
// set
// --------------------------------------------
void TileMap::set(int x, int y, const Tile& tile) {
	_tiles[getIndex(x, y)] = tile;
}

// --------------------------------------------
// set state
// --------------------------------------------
void TileMap::setState(int x, int y,int index) {
	if (isValid(x, y)) {
		_tiles[getIndex(x, y)].state.set(index);
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

const bool TileMap::isValid(const p2i& p) const {
	return isValid(p.x, p.y);
}
// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isCoherent(int gx, int gy) {
	Tile& t = get(gx, gy);
	if (!t.state.isSet(BIT_AVAILABLE)) {
		return false;
	}
	int color = t.color;
	int cnt = 0;
	for (int i = 1; i < 4; ++i) {
		Tile& n = get(gx + BLOCK_X[i], gy + BLOCK_Y[i]);
		if (n.state.isSet(BIT_MARKED) || n.state.isSet(BIT_FILLED)) {
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
			if (t.state.isSet(BIT_FILLED)) {
				++count;
			}
		}
	}
	LOG << "max: " << _maxAvailable << " current: " << count;
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

// --------------------------------------------
// is available
// --------------------------------------------
bool TileMap::isAvailable(int gx, int gy){
	if (!isValid(gx, gy)) {
		return false;
	}
	Tile& t = get(gx, gy);
	return t.state.isSet(BIT_AVAILABLE);
}

// --------------------------------------------
//
// --------------------------------------------
bool TileMap::isFree(int gx, int gy) {
	if (!isValid(gx, gy)) {
		return true;
	}
	Tile& t = get(gx, gy);
	return t.state.isSet(BIT_AVAILABLE) && !t.state.isSet(BIT_MARKED);
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
			t.edges = -1;
			t.state.clear();
			t.state.set(BIT_AVAILABLE);
		}
	}
	_maxAvailable = MAX_X * MAX_Y;
}

// -------------------------------------------------------
// determine all edges for coherent pieces
// -------------------------------------------------------
void TileMap::determineEdges() {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = _tiles[x + y * MAX_X];
			if (t.state.isSet(BIT_COHERENT)) {
				t.edges = determineEdge(x, y, t);
			}
		}
	}
}

// --------------------------------------------
// matches
// --------------------------------------------
bool TileMap::matches(int x, int y, const Tile& t) {
	if (!isAvailable(x, y)) {
		return false;
	}
	Tile& other = _tiles[x + y * MAX_X];
	return t.color == other.color;
}

// -------------------------------------------------------
// determine edges
//   8
// 1    2
//   4
// -------------------------------------------------------
int TileMap::determineEdge(int x, int y,const Tile& t) {
	int set = 0;
	if (isAvailable(x, y)) {
		if (matches(x - 1, y, t)) {
			set |= 1;
		}
		if (matches(x + 1, y, t)) {
			set |= 2;
		}
		if (matches(x, y - 1, t)) {
			set |= 4;
		}
		if (matches(x, y + 1, t)) {
			set |= 8;
		}
	}
	return set;
}

// --------------------------------------------
// set borders
// --------------------------------------------
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
	LOG << "loading level: " << index;
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
				if (t.state.isSet(BIT_AVAILABLE)) {
					++_maxAvailable;
				}
				set(x, y, t);
			}
		}
		fclose(f);
	}	
	LOG << "max available: " << _maxAvailable;
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

namespace map {

	// --------------------------------------------
	// convert screen pos to aligned screen pos
	// --------------------------------------------
	v2 convert(const v2& p) {

		int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
		int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
		if (xp < 0) {
			xp = 0;
		}
		if (xp >= (MAX_X - 1)) {
			xp = MAX_X - 2;
		}
		if (yp < 0) {
			yp = 0;
		}
		if (yp >= (MAX_Y - 1)) {
			yp = MAX_Y - 2;
		}
		return convert(xp, yp);
	}

	// --------------------------------------------
	// convert screen pos to grid pos
	// --------------------------------------------
	v2 convert(int x, int y) {
		return v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
	}

	// --------------------------------------------
	// convert to grid pos
	// --------------------------------------------
	p2i convertToGridPos(int x, int y) {
		int xp = (x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
		int yp = (y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
		if (xp < 0) {
			xp = 0;
		}
		if (xp >(MAX_X - 1)) {
			xp = MAX_X - 1;
		}
		if (yp < 0) {
			yp = 0;
		}
		if (yp >(MAX_Y - 1)) {
			yp = MAX_Y - 1;
		}
		return p2i(xp, yp);
	}
}