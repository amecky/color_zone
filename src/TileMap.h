#pragma once
#include "Common.h"
#include <utils\Log.h>
#include "Block.h"

// -------------------------------------------------------------
// Point
// -------------------------------------------------------------
struct Point {
	int x;
	int y;
};

// -------------------------------------------------------------
// Point list
// -------------------------------------------------------------
class PointList {

	typedef std::vector<Point> Points;

public:
	PointList() {}
	~PointList() {}
	void add(int x, int y) {
		if (!contains(x, y)) {
			Point p;
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
	const Point& get(int index) const {
		return _points[index];
	}
	const size_t size() const {
		return _points.size();
	}
private:
	Points _points;
};

// -------------------------------------------------------------
// Tile map
// -------------------------------------------------------------
class TileMap {

public:	
	TileMap() : _tiles(nullptr) {
		_tiles = std::make_unique<Tile[]>(MAX_X * MAX_Y);
		_startX = (1024 - SQUARE_SIZE * MAX_X) / 2;
		_startY = (768 - SQUARE_SIZE * MAX_Y) / 2;
	}
	~TileMap() {
		LOG << "deleting TileMap";		
	}
	void render();
	void render(int squareSize, float scale);
	const uint32 getIndex(uint32 x, uint32 y) const;
	Tile& get(int x, int y);
	const Tile& get(int x, int y) const;
	void set(int x, int y, const Tile& tile);
	const bool isValid(int x, int y) const;
	bool isCoherent(int gx, int gy);
	bool isBlockAvailable(int gx, int gy);
	bool isAvailable(int gx, int gy);
	bool isFree(int gx, int gy);
	void reset();
	void load(int index);
	void save(int index);
	void setBorder(int x, int y, int index);
	v2 convert(const v2& p);
	bool copyBlock(const Block& block);
	int clearColumn(int col);
	int getFillRate();
	void getColumn(int col,int* colors);
	v2 convert(int x, int y);
	v2 convertToGridPos(int x, int y);
private:	
	bool matches(int x, int y, const Tile& t);
	void check(int xp, int yp, int lastDir, PointList& list, bool rec);
	int determineEdge(int x, int y);
	
	void setState(int x, int y, TileState state);
	
	std::unique_ptr<Tile[]> _tiles;
	ds::Texture _filledTexture;
	ds::Texture _availableTexture;
	int _maxAvailable;
	int _startX;
	int _startY;
};

