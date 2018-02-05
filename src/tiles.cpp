#include "tiles.h"
#include "Common.h"
#include "GameSettings.h"
#include <SpriteBatchBuffer.h>
#include <diesel.h>
#include "objects\Block.h"
#include <vector>

// -------------------------------------------------------------
// Point list
// -------------------------------------------------------------
class PointList {

	typedef std::vector<p2i> Points;

public:
	PointList() {}
	~PointList() {}
	void add(int x, int y) {
		if (!contains(x, y)) {
			p2i p;
			p.x = x;
			p.y = y;
			_points.push_back(p);
		}
	}
	bool contains(int x, int y) const {
		if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
			for (size_t i = 0; i < _points.size(); ++i) {
				if (_points[i].x == x && _points[i].y == y) {
					return true;
				}
			}
		}
		return false;
	}
	const p2i& get(int index) const {
		return _points[index];
	}
	const size_t size() const {
		return _points.size();
	}
private:
	Points _points;
};

const int MARK_STEPS[] = { 0, 0, 0, 1, 1, 1, 1, 0 };

const p2i EDGE_OFFSET[] = { p2i(0, 1), p2i(1, 0), p2i(0, -1), p2i(-1, 0) };

uint32_t get_tiles_index(uint32_t x, uint32_t y) {
	return x + y * MAX_X;
}

// --------------------------------------------
// is valid
// --------------------------------------------
static bool is_valid(int x, int y) {
	if (x < 0 || x >= MAX_X) {
		return false;
	}
	if (y < 0 || y >= MAX_Y) {
		return false;
	}
	return true;
}
// --------------------------------------------
// is available
// --------------------------------------------
static bool is_available(Tile* tiles, int gx, int gy) {
	if (!is_valid(gx, gy)) {
		return false;
	}
	uint32_t idx = get_tiles_index(gx, gy);
	Tile& t = tiles[idx];
	return t.state.isSet(BIT_AVAILABLE);
}
// --------------------------------------------
// matches
// --------------------------------------------
static bool matches(Tile* tiles, int x, int y, const Tile& t) {
	if (!is_available(tiles, x, y)) {
		return false;
	}
	Tile& other = tiles[x + y * MAX_X];
	return t.color == other.color;
}


// -------------------------------------------------------
// determine edges
//   8
// 1    2
//   4
// -------------------------------------------------------
static int determineEdge(Tile* tiles, int x, int y, const Tile& t) {
	int set = 0;
	if (is_available(tiles, x, y)) {
		if (matches(tiles, x - 1, y, t)) {
			set |= 1;
		}
		if (matches(tiles, x + 1, y, t)) {
			set |= 2;
		}
		if (matches(tiles, x, y - 1, t)) {
			set |= 4;
		}
		if (matches(tiles, x, y + 1, t)) {
			set |= 8;
		}
	}
	return set;
}

// -------------------------------------------------------
// determine all edges for coherent pieces
// -------------------------------------------------------
static void determineEdges(Tile* tiles) {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = tiles[x + y * MAX_X];
			if (t.state.isSet(BIT_COHERENT)) {
				t.edges = determineEdge(tiles, x, y, t);
			}
		}
	}
}

// --------------------------------------------
//
// --------------------------------------------
static bool is_free(Tile* tiles, int gx, int gy) {
	if (!is_valid(gx, gy)) {
		return true;
	}
	Tile& t = tiles[gx + gy * MAX_X];
	return t.state.isSet(BIT_AVAILABLE) && !t.state.isSet(BIT_MARKED);
}

// --------------------------------------------
// set state
// --------------------------------------------
static void set_state(Tile* tiles, int x, int y, int index) {
	if (is_valid(x, y)) {
		tiles[get_tiles_index(x, y)].state.set(index);
	}
}

// --------------------------------------------
//
// --------------------------------------------
static bool is_block_available(Tile* tiles, int gx, int gy) {
	for (int i = 0; i < 4; ++i) {
		if (!is_free(tiles, gx + BLOCK_X[i], gy + BLOCK_Y[i])) {
			return false;
		}
	}
	return true;
}

// -------------------------------------------------------------
// check recursively to detect matching pieces
// -------------------------------------------------------------
static void check(Tile* tiles, int xp, int yp, int lastDir, PointList& list, bool rec) {
	if (is_valid(xp, yp)) {
		Tile& t = tiles[get_tiles_index(xp, yp)];
		int color = t.color;
		if (color != -1) {
			for (int i = 0; i < 4; ++i) {
				if (i != lastDir) {
					int sx = xp + XM[i];
					int sy = yp + YM[i];
					if (is_available(tiles, sx, sy)) {
						Tile& nt = tiles[get_tiles_index(sx, sy)];
						int nc = nt.color;
						if (nc != -1) {
							while (color == nc && color != -1) {
								bool recheck = !list.contains(sx, sy);
								list.add(sx, sy);
								if (recheck && rec) {
									check(tiles, sx, sy, LD[i], list, rec);
								}
								sx += XM[i];
								sy += YM[i];
								if (is_valid(sx, sy)) {
									Tile& npe = tiles[get_tiles_index(sx, sy)];
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

bool copy_block(Tile* tiles, const Block* block) {
	const p2i& p = block->position;
	int xp = (p.x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
	int yp = (p.y - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
	if (is_block_available(tiles, xp, yp)) {
		for (int i = 0; i < 4; ++i) {
			int cx = xp + MARK_STEPS[i * 2];
			int cy = yp + MARK_STEPS[i * 2 + 1];
			uint32_t idx = get_tiles_index(cx, cy);
			Tile& t = tiles[idx];
			t.state.set(BIT_MARKED);
			t.color = block->colors[i];
			PointList list;
			check(tiles, cx, cy, -1, list, true);
			list.add(cx, cy);
			if (list.size() > 2) {
				//LOG << "connected";
				t.state.set(BIT_COHERENT);
				for (size_t j = 0; j < list.size(); ++j) {
					const p2i& p = list.get(j);
					//LOG << j << " = " << p.x << " " << p.y;
					set_state(tiles, p.x, p.y, BIT_COHERENT);
				}
			}
		}
		determineEdges(tiles);
		return true;
	}
	else {
		//LOG << "Block is not available";
	}
	return false;
}

// ---------------------------------------------------------------
// render tiles
// ---------------------------------------------------------------
void render_tiles(Tile* tiles, SpriteBatchBuffer* buffer, int squareSize, float scale, ds::Color* colors, GameSettings* settings) {
	int sx = (1280 - squareSize * MAX_X) / 2;
	int sy = (720 - squareSize * MAX_Y) / 2;
	ds::Color clr = ds::Color(128, 128, 128, 255);
	ds::vec4 tex = ds::vec4(0, 100, 36, 36);
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			tex = ds::vec4(0, 100, 36, 36);
			clr = ds::Color(255, 255, 255, 255);
			uint32_t idx = get_tiles_index(x, y);
			const Tile& t = tiles[idx];
			ds::vec2 p = ds::vec2(sx + x * squareSize, sy + y * squareSize);
			if (t.state.isSet(BIT_AVAILABLE)) {
				if (t.state.isSet(BIT_COHERENT)) {
					if (t.edges > 0) {
						clr = colors[t.color];
						int left = t.edges * 36;
						tex = ds::vec4(left, 100, 36, 36);
					}
				}
				else if (t.state.isSet(BIT_MARKED)) {
					clr = colors[t.color];
				}
				else if (t.state.isSet(BIT_FILLED)) {
					tex = ds::vec4(0, 0, 36, 36);
					clr = settings->grid.filledColor;
				}
				else {
					tex = ds::vec4(0, 0, 36, 36);
					clr = settings->grid.backgroundColor;
				}
				buffer->add(p, tex, ds::vec2(scale, scale), 0.0f, clr);
			}
			if (t.borders != -1) {
				int left = 44 * t.borders;
				tex = ds::vec4(left, 44, 44, 44);
				buffer->add(p, tex, ds::vec2(scale, scale), 0.0f, settings->grid.borderColor);
			}
		}
	}
}

// --------------------------------------------
// get fill rate
// --------------------------------------------
int calculate_fill_rate(Tile* tiles) {
	int count = 0;
	int maxAvailable = 0;
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			uint32_t idx = get_tiles_index(x, y);
			const Tile& t = tiles[idx];
			if (t.state.isSet(BIT_AVAILABLE)) {
				++maxAvailable;
			}
			if (t.state.isSet(BIT_FILLED)) {
				++count;
			}
		}
	}
	if (maxAvailable == 0) {
		return 0;
	}
	float per = static_cast<float>(count) / static_cast<float>(maxAvailable) * 100.0f;
	return static_cast<int>(per);
}
