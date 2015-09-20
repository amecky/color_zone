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
			}
		}
	}
	for (size_t i = 0; i < _edges.size(); ++i) {
		ds::sprites::draw(_edges[i].position, _edges[i].texture);
	}
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
	int y = 0;
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
					assert(line.length() == HALF_MAX_X);
					if (y < HALF_MAX_Y) {
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
							set(x * 2, y * 2, t);
							set(x * 2 + 1, y * 2, t);
							set(x * 2 + 1, y * 2 + 1, t);
							set(x * 2, y * 2 + 1, t);
						}
						++y;
					}
				}
			}
		}
		myfile.close();
		determineEdges();
		return true;
	}
	return false;
}

void TileMap::addEdge(int x, int y, int offsetX, int offsetY, const ds::Texture& t) {
	Edge e;
	e.position = convert(x, y);
	e.position.x += offsetX;
	e.position.y += offsetY;
	e.texture = t;
	_edges.push_back(e);
}

void TileMap::determineEdges() {
	for (int y = 0; y < MAX_Y; ++y) {
		int oe = determineOuterEdge(0, y);
		if (oe != 0 && oe != 15) {
			LOG << "outer e: " << 0 << " " << y << " edge: " << oe;
			if (oe == 1) {
				addEdge(0,y,-3,0,ds::math::buildTexture(280, 620, 40, 36));
			}
			if (oe == 8) {
				addEdge(0, y, 0, 3, ds::math::buildTexture(280, 252, 36, 36));
			}
			else if (oe == 5) {
				addEdge(0, y, -3, -3, ds::math::buildTexture(280, 540, 40, 40));
			}
			//else if (oe == 9) {
				//addEdge(0, y, -3, 3, ds::math::buildTexture(280, 580, 40, 40));
			//}
			//else {
				//e.position = convert(0, y);
				//e.position -= v2(4, 4);
				//e.texture = ds::math::buildTexture(284, 360, 32, 32);
			//}
		}
	}
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			int e1 = determineEdge(x, y);
			if (e1 != 0 && e1 != 15) {
				if (e1 == 5) {
					addEdge(x, y, 2, 2, ds::math::buildTexture(284, 360, 32, 32));
				}
				if (e1 == 4) {
					addEdge(x, y, 0, 2, ds::math::buildTexture(284, 396, 36, 32));
				}
				if (e1 == 6) {
					addEdge(x, y, -3, 2, ds::math::buildTexture(284, 324, 36, 32));
				}
				if (e1 == 2) {
					addEdge(x, y, -3, 0, ds::math::buildTexture(280, 468, 36, 36));
				}
				if (e1 == 1) {
					addEdge(x, y, 4, 0, ds::math::buildTexture(280, 504, 36, 36));
				}
				if (e1 == 9) {
					addEdge(x, y, 4, -3, ds::math::buildTexture(280, 216, 36, 36));
				}
				if (e1 == 8) {
					addEdge(x, y, 0, -3, ds::math::buildTexture(280, 252, 36, 36));
				}
				if (e1 == 10) {
					addEdge(x, y, -3, -3, ds::math::buildTexture(280, 180, 36, 36));
				}
				LOG << "e: " << x << " " << y << " edge: " << e1;
			}
		}
	}
}
// -------------------------------------------------------
// determine corners
// -------------------------------------------------------
int TileMap::determineEdge(int x, int y) {
	int set = 0;
	if (!isAvailable(x, y)) {
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
// -------------------------------------------------------
// determine corners
// -------------------------------------------------------
int TileMap::determineOuterEdge(int x, int y) {
	int set = 0;
	if (isAvailable(x, y)) {
		if (!isAvailable(x - 1, y)) {
			set |= 1;
		}
		if (!isAvailable(x + 1, y)) {
			set |= 2;
		}
		if (!isAvailable(x, y - 1)) {
			set |= 4;
		}
		if (!isAvailable(x, y + 1)) {
			set |= 8;
		}
	}
	return set;
}