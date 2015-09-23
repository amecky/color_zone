#include "TileMap.h"
#include<iostream>
#include<fstream>
#include <string>
#include <sprites\SpriteBatch.h>

void TileMap::render() {
	for (int x = 0; x < _width; ++x) {
		for (int y = 0; y < _height; ++y) {
			const Tile& t = get(x, y);
			v2 p = v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
			if (t.used) {
				if (t.state == TS_AVAILABLE) {
					ds::sprites::draw(p, ds::math::buildTexture(0, 0, 36, 36));
				}
				/*
				else if (t.state == TS_MARKED) {
					ds::sprites::draw(p, ds::math::buildTexture(150, 108, 36, 36), 0.0f, 1.0f, 1.0f, BLOCK_COLORS[t.color]);
				}
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
			if ( t.borders != 0 && t.borders != 15 ) {
				int w = 44 - t.borderScale.x;
				int h = 44 - t.borderScale.y;
				p += t.borderOffset;
				ds::sprites::draw(p, ds::math::buildTexture(44, 44 * t.borders, w, h));
			}
		}
	}
	//for (size_t i = 0; i < _edges.size(); ++i) {
		//ds::sprites::draw(_edges[i].position, _edges[i].texture);
	//}
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
// load all text levels
// -------------------------------------------------------
bool TileMap::loadTextFile(int nr) {
	TileState state;
	char buffer[256];
	sprintf_s(buffer, 256, "levels\\Level%d.txt", nr);
	std::ifstream myfile(buffer);
	int y = HALF_MAX_Y - 1;
	bool first = true;
	std::string line;
	if (myfile.is_open()) {
		while (myfile) {
			getline(myfile, line);
			if (first) {
				//sprintf(level->name, "%s", line.c_str());
				first = false;
			}
			else {
				if (line.length() > 0) {
					//assert(line.length() == HALF_MAX_X);
					if (y >= 0) {
						for (int x = 0; x < HALF_MAX_X; ++x) {
							char c = line[x];
							Tile t;							
							if (c == 'x') {								
								t.state = TS_AVAILABLE;
								t.used = true;
								
							}
							else {
								t.state = TS_EMPTY;
								t.used = false;
							}
							t.borders = 0;
							set(x * 2, y * 2, t);
							set(x * 2 + 1, y * 2, t);
							set(x * 2 + 1, y * 2 + 1, t);
							set(x * 2, y * 2 + 1, t);
						}
						--y;
					}
				}
			}
		}
		myfile.close();
		determineEdges();
		return true;
	}
	else {
		LOGE << "Cannot find file: " << buffer;
	}
	return false;
}

void TileMap::reset() {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = _tiles[x + y * MAX_X];
			t.borders = 0;
			t.used = true;
			t.state = TS_AVAILABLE;
		}
	}
}

void TileMap::determineEdges() {
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			Tile& t = _tiles[x + y * MAX_X];
			t.borders = determineEdge(x, y);			
			LOG << "x: " << x << " y: " << y << " borders: " << t.borders;
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

void TileMap::save(const char* levelname, int index) {
	// char[32] name
	// int map width
	// int map height
	// int map data
	// int num_borders
	// v2 pos
	// int textureIndex
	// ....
}
