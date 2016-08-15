#pragma once
#include "Common.h"
#include <utils\Log.h>
#include "objects\Block.h"
#include <stdint.h>
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

// -------------------------------------------------------------
// Tile map
// -------------------------------------------------------------
class TileMap {

	struct Border {
		p2i pos;
		int type;
		bool inside;

		Border() : pos(0, 0), type(0), inside(true) {}
	};

public:	
	TileMap(GameContext* context) : _tiles(0) , _ctx(context) {
		_tiles = new Tile[MAX_X * MAX_Y];		
	}
	~TileMap() {}
	void render();
	void render(int squareSize, float scale);
	const uint32_t getIndex(uint32_t x, uint32_t y) const;
	Tile& get(int x, int y);
	Tile& get(const p2i& p);
	const Tile& get(int x, int y) const;
	void set(int x, int y, const Tile& tile);
	const bool isValid(int x, int y) const;
	const bool isValid(const p2i& p) const;
	bool isCoherent(int gx, int gy);
	bool isBlockAvailable(int gx, int gy);
	bool isAvailable(int gx, int gy);
	bool isFree(int gx, int gy);
	int getEdges(const p2i& p);
	void buildBorders();
	void reset();
	void load(int index);
	void save(int index);
	void setBorder(int x, int y, int index);
	
	bool copyBlock(const Block* block);
	int clearColumn(int col);
	int getFillRate();
	void getColumn(int col,int* colors);

	void removeBlock(const p2i& gridPos);
	void setBlock(const p2i& gridPos);
	void debug();
private:	
	bool matches(int x, int y, const Tile& t);
	void check(int xp, int yp, int lastDir, PointList& list, bool rec);
	int determineEdge(int x, int y, const Tile& t);
	void determineEdges();
	void setState(int x, int y, int index);
	GameContext* _ctx;
	Tile* _tiles;
	ds::Texture _filledTexture;
	ds::Texture _availableTexture;
	int _maxAvailable;
	ds::Array<Border> _border;
};

namespace map {

	p2i screen2grid(int x, int y);

	p2i screen2grid(const v2& p);

	p2i grid2screen(const p2i& p);

	bool isOuterRim(const p2i& p);

	bool is_inside(int x, int y);

	//p2i screen2grid(int x, int y);

}

